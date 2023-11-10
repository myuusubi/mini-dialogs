"""
----------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
----------------------------------------------------------------------------------
Copyright 2023 Mewsubi

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this 
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors 
   may be used to endorse or promote products derived from this software without 
   specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE
----------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)

This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.

In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------
"""

import os

if not os.path.exists( os.path.join( "tinyfd", "tinyfiledialogs.h" ) ):
	print( "[ERROR] Could not find \"tinyfd\" submodule\n" )
	print( "Try \"git submodule update --init --recursive\"" )
	exit( 1 )

tinyfd_header_file = open( os.path.join( "tinyfd", "tinyfiledialogs.h" ), "r" )
tinyfd_header_text = tinyfd_header_file.read()
tinyfd_header_file.close()

tinyfd_source_file = open( os.path.join( "tinyfd", "tinyfiledialogs.c" ), "r" )
tinyfd_source_text = tinyfd_source_file.read()
tinyfd_source_file.close()

LICENSE = ""
LICENSE_PARSED = False

def minify_text( text ):
	global LICENSE, LICENSE_PARSED

	stripped = ""
	text_length = len( text )
	ci = 0

	in_string = False
	in_comment = False
	while ci < text_length:
		c = text[ci]

		# Strip /* */ comments from the code
		if in_comment:
			# Check to see if the comment has ended
			if c == '*' and text[ci : ci + 2] == "*/":
				in_comment = False
				ci += 1
				if not LICENSE_PARSED:
					LICENSE_PARSED = True
					LICENSE = "/*" + LICENSE + "*/"
			elif not LICENSE_PARSED:
				LICENSE += c
		else:
			if in_string:
				if c == '\\':
					# Check for multiline strings
					if text[ci : ci + 2] == '\\\n':
						# Strip the multiline string
						c = ""
						ci += 1
				elif c == '"':
					in_string = False
			else:
				# Check to see if a comment has begun
				if c == '/' and text[ci : ci + 2] == "/*":
					in_comment = True
					c = ''
					ci += 1
				elif c == '"': # Check to see if a string has begun
					in_string = True
			if c == '\\':
				# Consume the entire escape sequence
				stripped += c
				ci += 1
				if ci < text_length:
					c = text[ci]
				else:
					c = ''
			stripped += c
		ci += 1
	return stripped

# Constants for valid tokens
ORD_a = ord( 'a' )
ORD_z = ord( 'z' )
ORD_A = ord( 'A' )
ORD_Z = ord( 'Z' )
ORD_0 = ord( '0' )
ORD_9 = ord( '9' )
ORD_UNDERSCORE = ord( '_' )

def valid_token_char( c ):
	if c.isspace():
		return False
	o = ord( c )
	if o == ORD_UNDERSCORE:
		return True
	elif o >= ORD_0 and o <= ORD_9:
		return True
	elif o >= ORD_A and o <= ORD_Z:
		return True
	elif o >= ORD_a and o <= ORD_z:
		return True
	return False


# TODO: This can break if we find a token inside a string (e.g. "malloc")
def replace_token( line, t, tr ):
	# Check if the token exists in the line
	if line.find( t ) < 0:
		return line

	# Split the line on the token
	s = line.split( t )
	s_len = len( s )
	token_count = s_len - 1

	if token_count == 0:
		# This should never happen
		return line

	# Store the lengths in advance
	lens = [0] * s_len
	for i in range( s_len ):
		lens[i] = len( s[i] )

	# Create array to store whether a token is valid
	valid = [False] * token_count

	# Validate that the tokens actually match
	for i in range( token_count ):
		suffix = s[i + 1]
		suffix_len = lens[i + 1]

		# Any valid token char as a suffix means the match is invalid
		if suffix_len > 0 and valid_token_char( suffix[0] ):
			continue

		prefix_len = lens[i]
		if prefix_len > 0:
			prefix = s[i]
			prefix_start = None
			for j in range( prefix_len ):
				p = prefix[prefix_len - 1 - j]
				if not valid_token_char( p ):
					break
				prefix_start = p
			if prefix_start:
				o = ord( prefix_start )
				# A prefix has to have a non-numeric start to be valid (Literals like will not count 1u)
				if o >= ORD_0 and o <= ORD_9:
					pass
				else:
					# If the prefix is a valid token, then the match we found is invalid
					continue

		valid[i] = True

	joined = ""
	for i in range( s_len ):
		joined += s[i]
		if i >= token_count:
			break
		if valid[i]:
			joined += tr
		else:
			joined += t
	return joined

extern_symbols = {}

def clean_line_header( line ):
	global extern_symbols

	# Hacky way to locate the declarations
	if line.find( "(" ) >= 0:
		# Convert all declarations to static to prevent exporting the symbols
		return "static inline " + line

	sline = line.strip()
	# Hacky way to locate the extern variables
	if sline.find( "extern" ) == 0 and sline.find( ";" ) == len( sline ) - 1:
		sline = sline.split()
		symbol_raw = sline[-1]
		symbol = ""
		for c in symbol_raw:
			if not valid_token_char( c ):
				break
			symbol += c
		if len( symbol ) > 0:
			extern_symbols[symbol] = 0
			return ""
	return line

extern_symbol_list_len = 0
extern_symbol_list = []
extern_symbol_lens = []

def clean_line_source( line ):
	global extern_symbol_list

	orig_line = line
	line = replace_token( line, "malloc", "MDLG_MALLOC" )
	line = replace_token( line, "realloc", "MDLG_REALLOC" )
	line = replace_token( line, "free", "MDLG_FREE" )

	# Hacky way to check if we replaced a token, since none of our other checks should have them
	if line != orig_line:
		return line

	inc_line = line.strip()
	if inc_line[0] == '#':
		if inc_line[0:8] == "#include":
			inc_line = inc_line.split()
			if len( inc_line ) >= 2:
				if inc_line[1] == "\"tinyfiledialogs.h\"":
					return ""
		return line

	# Hacky way to identify the assignments
	for si in range( extern_symbol_list_len ):
		symbol = extern_symbol_list[si]
		symbol_idx = line.find( symbol )
		if symbol_idx < 0:
			continue
		symbol_len = extern_symbol_lens[si]
		# Hacky way to identify the global assignments
		symbol_idx_2 = line[symbol_idx + symbol_len:].find( symbol )
		if symbol_idx_2 >= 0:
			symbol_string = '"' + symbol + '"'
			if ( line[symbol_idx - 1:symbol_idx + symbol_len + 1] == symbol_string ) or ( line[symbol_idx_2 - 1:symbol_idx_2 + symbol_len + 1] == symbol_string ):
				return ""
			continue
		equal_idx = line.find( "=" )
		if equal_idx < 0:
			continue
		if equal_idx < symbol_idx + symbol_len:
			continue
		if line[equal_idx + 1:].find( "=" ) >= 0:
			continue
		for ci in range( symbol_idx + symbol_len, equal_idx ):
			c = line[ci]
			if c.isspace():
				continue
			if c == '[' or c == ']':
				continue
			o = ord( c )
			if ORD_0 <= o <= ORD_9:
				continue
			return line
		return "static const " + line

	return line

def condense_newlines( text, is_header = True ):
	condensed = ""
	text_length = len( text )
	last_empty = 0

	line = ""
	line_empty = True

	ci = 0
	while ci < text_length:
		c = text[ci]
		line += c
		if not c.isspace():
			line_empty = False
		if c == '\n':
			if line_empty:
				if not last_empty:
					condensed += '\n'
				last_empty = True
			else:
				if is_header:
					line = clean_line_header( line )
				else:
					line = clean_line_source( line )
				condensed += line
				last_empty = False
			line = ""
			line_empty = True
		ci += 1
	return condensed

def make_line_static( text ):
	stext = text.strip()
	if len( stext ) == 0:
		return ""
	if stext[0] == '#':
		return text
	if stext.find( '(' ) < 0:
		return text
	if stext[:6] == "static":
		return text
	return "static " + text[text.find( stext ):]

def make_static( text ):
	scope_line = ""
	scope = 0

	in_string = False

	static_text = ""

	text_length = len( text )
	ci = 0
	while ci < text_length:
		c = text[ci]
		if in_string:
			static_text += c
			if c == '\\':
				ci += 1
				c = text[ci]
				static_text += c
			elif c == '"':
				in_string = False
			ci += 1
			continue

		if c == '"':
			static_text += make_line_static( scope_line ) + '"'
			scope_line = ""
			in_string = True
			ci += 1
			continue

		if c == '\\':
			static_text += c
			ci += 1
			c = text[ci]
			static_text += c
			if scope == 0:
				scope_line += '\\' + c
			ci += 1
			continue
		
		if c == '{':
			if scope == 0:
				static_text += make_line_static( scope_line )
				scope_line = ""
			scope += 1
		elif c == '}':
			scope -= 1
			if scope == 0:
				static_text += '}'
				scope_line = ""
		elif scope <= 0:
			if c == '\n':
				static_text += make_line_static( scope_line ) + "\n"
				scope_line = ""
			else:
				scope_line += c
		if scope > 0:
			static_text += c
		ci += 1
	return static_text

# Remove comments & multi-line strings from header & source
parsed_header = minify_text( tinyfd_header_text )
parsed_source = minify_text( tinyfd_source_text )

# Remove extraneous newlines and remove extern variables & change functions to static
parsed_header = condense_newlines( parsed_header, is_header = True )

# Add the extern symbols we found to the global list
for symbol in extern_symbols:
	extern_symbol_list.append( symbol )
	extern_symbol_lens.append( len( symbol ) )

extern_symbol_list_len = len( extern_symbol_list )

# Remove extraneous newlines and change extern variables to const
parsed_source = condense_newlines( parsed_source, is_header = False )
parsed_source = make_static( parsed_source )

# Read the MiniDLG header into the beginning of the flat file
mini_dlg_header_file = open( os.path.join( "src", "mini_dlg.h" ), "r" )
mini_dlg_text = mini_dlg_header_file.read()
mini_dlg_header_file.close()

# Add the TinyFD License
mini_dlg_text += LICENSE

# Add the MINI_DLG_IMPLEMENTATION macro
mini_dlg_text += """
#ifdef MINI_DLG_IMPLEMENTATION
"""

# Append the TinyFD header
mini_dlg_text += parsed_header

# Make sure that TinyFD will compile properly on C++ sources
mini_dlg_text += """
#ifdef __cplusplus
extern "C" {
#endif
"""

# Append the TinyFD source
mini_dlg_text += parsed_source

# Close the C++ macro check, and close the implementation check
mini_dlg_text += """
#ifdef __cplusplus
}
#endif

#endif
"""

# Read the MiniDLG source into the end of the flat file
mini_dlg_source_file = open( os.path.join( "src", "mini_dlg.inc" ), "r" )
mini_dlg_text += mini_dlg_source_file.read()
mini_dlg_source_file.close()

# Write the flat file to disk
mini_dlg_file = open( os.path.join( "src", "releases", "mini_dlg_flat.h" ), "w" )
mini_dlg_file.write( mini_dlg_text )
mini_dlg_file.close()
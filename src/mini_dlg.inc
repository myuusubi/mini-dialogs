/*
----------------------------------------------------------------------------------
The flattened code for Tiny File Dialogs ends here. The rest is MiniDLG code
----------------------------------------------------------------------------------
*/

#ifdef MINI_DLG_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifdef _WIN32
#define MDLG_CONCAT_2_HELPER(a, b) a ## b
#define MDLG_CONCAT_2(a, b) MDLG_CONCAT_2_HELPER(a, b)

#define MDLG_WIDEN(x) MDLG_CONCAT_2(L, x)
#else
#define MDLG_WIDEN(x) x
#endif

static MDLGChar const* const MDLG_MSG_STRINGS[3] = {
	MDLG_WIDEN("ok"),
	MDLG_WIDEN("okcancel"),
	MDLG_WIDEN("yesno")
};

static MDLGChar const* const MDLG_ICON_STRINGS[4] = {
	MDLG_WIDEN("info"),
	MDLG_WIDEN("warning"),
	MDLG_WIDEN("error"),
	MDLG_WIDEN("question")
};

#undef MDLG_WIDEN

#ifdef _WIN32

#undef MDLG_CONCAT_2
#undef MDLG_CONCAT_2_HELPER

static inline wchar_t* mdlg_utf16_allocate_from_utf8( char const* string_utf8 ) {
	if( string_utf8 == ( char const* ) 0 ) {
		return ( wchar_t* ) 0;
	}

	// Get the size of the UTF-8 string in bytes (without NULL terminator)
	size_t bytes_len = 0;
	for( ; string_utf8[bytes_len] != '\0'; ++bytes_len ) {
		// Do nothing
	}

	int buffer_size = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, string_utf8, bytes_len, ( LPWSTR ) 0, 0 );
	if( buffer_size == 0 ) {
		return ( wchar_t* ) 0;
	}

	wchar_t* string_utf16 = ( wchar_t* ) MDLG_MALLOC( sizeof( wchar_t ) * ( buffer_size + 1 ) );
	int new_buffer_size = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, string_utf8, bytes_len, string_utf16, buffer_size );

	// Should this ever be true?
	if( new_buffer_size == 0 || buffer_size != new_buffer_size ) {
		MDLG_FREE( string_utf16 );
		return ( wchar_t* ) 0;
	}

	string_utf16[buffer_size] = L'\0';

	return string_utf16;
}

static inline void mdlg_utf16_deallocate( wchar_t* string_utf16 ) {
	MDLG_FREE( string_utf16 );
}

#endif

int mini_dlg_message_box( char const* title, char const* msg, int msg_type, int icon_type, int default_button ) {
	MDLGChar const* msg_type_string = MDLG_MSG_STRINGS[0];
	MDLGChar const* icon_type_string = MDLG_ICON_STRINGS[0];

	switch( msg_type ) {
		case MDLG_MSG_OKCANCEL:
		case MDLG_MSG_YESNO:
			msg_type_string = MDLG_MSG_STRINGS[msg_type];
			break;
	}

	switch( icon_type ) {
		case MDLG_ICON_WARNING:
		case MDLG_ICON_ERROR:
		case MDLG_ICON_QUESTION:
			icon_type_string = MDLG_ICON_STRINGS[icon_type];
			break;
	}

	// MDLG_BUTTON_NO only corresponds to "no" when using "yesnocancel", but we don't have that implemented
	if( default_button == MDLG_BUTTON_NO ) {
		default_button = MDLG_BUTTON_CANCEL;
	}

#ifdef _WIN32
	wchar_t* w_title = mdlg_utf16_allocate_from_utf8( title );
	wchar_t* w_msg = mdlg_utf16_allocate_from_utf8( msg );

	int result = tinyfd_messageBoxW( w_title, w_msg, msg_type_string, icon_type_string, default_button );

	mdlg_utf16_deallocate( w_title );
	mdlg_utf16_deallocate( w_msg );

	return result;
#else
	return tinyfd_messageBox( w_title, w_msg, msg_type_string, icon_type_string, default_button );
#endif
}

MDLGChar* mini_dlg_file_opener( char const* title, MDLGChar const* default_path, int filter_count, char const* const* filters, char const* filter_desc ) {
#ifdef _WIN32
	wchar_t* w_title = mdlg_utf16_allocate_from_utf8( title );
	wchar_t* w_filter_desc = mdlg_utf16_allocate_from_utf8( filter_desc );

	wchar_t** w_filters = ( wchar_t** ) 0;
	if( filter_count > 0 ) {
		w_filters = ( wchar_t** ) MDLG_MALLOC( sizeof( wchar_t* ) * filter_count );
		for( int f = 0; f < filter_count; ++f ) {
			w_filters[f] = mdlg_utf16_allocate_from_utf8( filters[f] );
		}
	}

	wchar_t* result = tinyfd_openFileDialogW( w_title, default_path, filter_count, w_filters, w_filter_desc, 0 );

	mdlg_utf16_deallocate( w_title );
	mdlg_utf16_deallocate( w_filter_desc );

	for( int f = 0; f < filter_count; ++f ) {
		mdlg_utf16_deallocate( w_filters[f] );
	}
	MDLG_FREE( w_filters );

	return result;
#else
	if( filter_count == 0 ) {
		filters = ( char const* const* ) 0;
	}

	return tinyfd_openFileDialog( title, default_path, filter_count, filters, filter_desc, 0 );
#endif
}

#ifdef __cplusplus
}
#endif

#endif

/*
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
*/
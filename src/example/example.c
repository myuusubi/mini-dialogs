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

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”AND
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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "mini_dlg.h"

static inline MDLGChar* path_allocate_copy( MDLGChar* path );
static inline void path_deallocate( MDLGChar* path );

MDLGChar* path_allocate_copy( MDLGChar* path ) {
	if( path == ( MDLGChar* ) 0 ) {
		return ( MDLGChar* ) 0;
	}

	size_t path_length = 0;
	for( ; path[path_length] != MDLG_NULL_TERM; ++path_length ) {

	}

	MDLGChar* new_path = malloc( sizeof( MDLGChar ) * ( path_length + 1 ) );
	memcpy( new_path, path, sizeof( MDLGChar ) * path_length );
	new_path[path_length] = MDLG_NULL_TERM;

	return new_path;
}

void path_deallocate( MDLGChar* path ) {
	free( path );
}

int main() {
	int result_int;
	MDLGChar* result_path;

	// Demonstrate a basic message box
	mini_dlg_message_box( "Example Error Box", "Hello World", MDLG_MSG_OK, MDLG_ICON_ERROR, MDLG_BUTTON_OK );

	uint32_t counter = 0;

	// Demonstrate option checking
	do {
		result_int = mini_dlg_message_box( "Example Options", "You must click yes", MDLG_MSG_YESNO, MDLG_ICON_INFO, MDLG_BUTTON_NO );
		++counter;
	} while( result_int != MDLG_BUTTON_YES );

	if( counter <= 1 ) {
		mini_dlg_message_box( "Achievement: Obedience", "You are too obedient...", MDLG_MSG_OK, MDLG_ICON_WARNING, MDLG_BUTTON_OK );
	} else if( counter == 2 ) {
		mini_dlg_message_box( "Achievement: Balance", "Perfectly balanced.", MDLG_MSG_OK, MDLG_ICON_INFO, MDLG_BUTTON_OK );
	} else {
		char buffer[256];
		sprintf( buffer, "You pressed No %u times... Why?", counter );
		mini_dlg_message_box( "Achievement: Rebellious", buffer, MDLG_MSG_OK, MDLG_ICON_QUESTION, MDLG_BUTTON_OK );
	}

	// Demonstrate a basic file opener with no parameters
	result_path = mini_dlg_file_opener( "File Opener", ( MDLGChar const* ) 0, 0, ( char const* const* ) 0, ( char const* ) 0 );

	// Copy the path of the last file, and use it as the default
	MDLGChar* default_path = path_allocate_copy( result_path );
	result_path = mini_dlg_file_opener( "Default Path", default_path, 0, ( char const* const* ) 0, ( char const* ) 0 );
	path_deallocate( default_path );

	// Demonstrate using a single filter
	char const* txt_filter = "*.txt";
	result_path = mini_dlg_file_opener( ".txt Opener", ( MDLGChar const* ) 0, 1, &txt_filter, "Text Document (.txt)" );

	// Demonstrate usage of multiple filters
	char const* const image_filter[] = {
		"*.jpg",
		"*.jpeg",
		"*.png",
	};
	size_t const image_filter_len = sizeof( image_filter ) / sizeof( image_filter[0] );
	result_path = mini_dlg_file_opener( "Image Opener", ( MDLGChar const* ) 0, image_filter_len, image_filter, ( char const* ) 0 );

	return 0;
}
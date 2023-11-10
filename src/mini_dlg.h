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

#ifndef OXY_MINI_DLG_H
#define OXY_MINI_DLG_H

// Message Box Option Types
#define MDLG_MSG_OK           0
#define MDLG_MSG_OKCANCEL     1
#define MDLG_MSG_YESNO        2

// Message Box Icon Types
#define MDLG_ICON_INFO        0
#define MDLG_ICON_WARNING     1
#define MDLG_ICON_ERROR       2
#define MDLG_ICON_QUESTION    3

// Message Box Button Types
#define MDLG_BUTTON_CANCEL    0
#define MDLG_BUTTON_OK        1
#define MDLG_BUTTON_YES       1
#define MDLG_BUTTON_NO        2

#ifdef __cplusplus
extern "C" {
#endif

// Path Types
#ifdef _WIN32
// On Windows, we ALWAYS return wide paths, to account for UCS-2
typedef wchar_t MDLGChar;
#define MDLG_NULL_TERM ( L'\0' )
#else
// On other platforms wchar_t will likely by 32-bit. Use UTF-8 instead
typedef char MDLGChar;
#define MDLG_NULL_TERM ( '\0' )
#endif

int mini_dlg_message_box( char const* title, char const* msg, int msg_type, int icon_type, int default_button );

MDLGChar* mini_dlg_file_opener( char const* title, MDLGChar const* default_path, int filter_count, char const* const* filters, char const* filter_desc );

#ifdef __cplusplus
}
#endif

#endif

#ifdef MINI_DLG_IMPLEMENTATION

// Check if any of the 3 memory operations (malloc/realloc/free) are undefined
#if !defined( MDLG_MALLOC ) || !defined( MDLG_REALLOC ) || !defined( MDLG_FREE )

// Undefine MALLOC if it exists
#ifdef MDLG_MALLOC
#undef MDLG_MALLOC
#endif

// Undefine REALLOC if it exists
#ifdef MDLG_REALLOC
#undef MDLG_REALLOC
#endif

// Undefine FREE if it exists
#ifdef MDLG_FREE
#undef MDLG_FREE
#endif

#endif

// We only check for MALLOC existence, since if just 1 is undefined, all of them will be
#ifndef MDLG_MALLOC

// Define all 3 memory operations
#define MDLG_MALLOC(X) malloc( X )
#define MDLG_REALLOC(X, Y) realloc( X, Y )
#define MDLG_FREE(X) free( X )

#endif

#endif

/*
----------------------------------------------------------------------------------
The flattened code for Tiny File Dialogs begins below
----------------------------------------------------------------------------------
*/


/*
----------------------------------------------------------------------------------
The flattened code for Tiny File Dialogs ends here. The rest is MiniDLG code
----------------------------------------------------------------------------------
*/

#ifdef MINI_DLG_IMPLEMENTATION

static char const* const MDLG_MSG_STRINGS[3] = {
	"ok",
	"okcancel",
	"yesno"
};

static char const* const MDLG_ICON_STRINGS[4] = {
	"info",
	"warning",
	"error",
	"question"
};

int mini_dlg_message_box( char const* title, char const* msg, int msg_type, int icon_type, int default_button ) {
	char const* msg_type_string = MDLG_MSG_STRINGS[0];
	char const* icon_type_string = MDLG_ICON_STRINGS[0];

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
			icon_type_string = MDLG_ICON_STRINGS[msg_type];
			break;
	}

	// MDLG_BUTTON_NO only corresponds to "no" when using "yesnocancel", but we don't have that implemented
	if( default_button == MDLG_BUTTON_NO ) {
		default_button = MDLG_BUTTON_CANCEL;
	}

	return tinyfd_messageBox( title, msg, msg_type_string, icon_type_string, default_button );
}

MDLGChar* mini_dlg_file_opener( char const* title, MDLGChar const* default_path, int filter_count, char const* const* filters, char const* filter_desc ) {
	return tinyfd_openFileDialog( title, default_path, filter_count, filters, filter_desc, 0 );
}

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
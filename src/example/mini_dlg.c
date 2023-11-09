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

#include <stddef.h>
#include <stdint.h>

static inline void* debug_malloc( size_t size, const char* file, const char* function, int line );
static inline void* debug_realloc( void* ptr, size_t size, const char* file, const char* function, int line );
static inline void debug_free( void* ptr, const char* file, const char* function, int line );

// Here is an example of how to use the MDLG memory operation macros
#define MDLG_MALLOC(X) debug_malloc( X, __FILE__, __FUNCTION__, __LINE__ )
#define MDLG_REALLOC(X, Y) debug_realloc( X, Y, __FILE__, __FUNCTION__, __LINE__ )
#define MDLG_FREE(X) debug_free( X, __FILE__, __FUNCTION__, __LINE__ )

// Here is an example usage of the MINI_DLG_IMPLEMENTATION macro
#define MINI_DLG_IMPLEMENTATION
#include "../mini_dlg_flat.h"
#undef MINI_DLG_IMPLEMENTATION

// Malloc with extra arguments for logging information
void* debug_malloc( size_t size, const char* file, const char* function, int line ) {
   void* result = malloc( size );

   char buffer[512];
   sprintf( buffer, "[MEMORY] malloc(%zu), %s, %s:%i, ptr = %p\n", size, file, function, line, result );
   fputs( buffer, stdout );

   return result;
}

// Realloc with extra arguments for logging information
void* debug_realloc( void* ptr, size_t size, const char* file, const char* function, int line ) {
   void* result = realloc( ptr, size );

   char buffer[512];
   sprintf( buffer, "[MEMORY] realloc(%p, %zu), %s, %s:%i, ptr = %p\n", ptr, size, file, function, line, result );
   fputs( buffer, stdout );

   return result;
}

// Free with extra arguments for logging information
void debug_free( void* ptr, const char* file, const char* function, int line ) {
   free( ptr );

   char buffer[512];

   uintptr_t p = ( uintptr_t ) ptr;

   sprintf( buffer, "[MEMORY] free(0x%lx), %s, %s:%i\n", p, file, function, line );
   fputs( buffer, stdout );
}
/*
* libtcod 1.4.1
* Copyright (c) 2008,2009 J.C.Wilk
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of J.C.Wilk may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY J.C.WILK ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL J.C.WILK BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _TCOD_PATH_H
#define _TCOD_PATH_H

typedef float (*TCOD_path_func_t)( int xFrom, int yFrom, int xTo, int yTo, void *user_data );
typedef void *TCOD_path_t;

TCODLIB_API TCOD_path_t TCOD_path_new_using_map(TCOD_map_t map, float diagonalCost);
TCODLIB_API TCOD_path_t TCOD_path_new_using_function(int map_width, int map_height, TCOD_path_func_t func, void *user_data, float diagonalCost);

TCODLIB_API bool TCOD_path_compute(TCOD_path_t path, int ox,int oy, int dx, int dy);
TCODLIB_API bool TCOD_path_walk(TCOD_path_t path, int *x, int *y, bool recalculate_when_needed);
TCODLIB_API bool TCOD_path_is_empty(TCOD_path_t path);
TCODLIB_API int TCOD_path_size(TCOD_path_t path);
TCODLIB_API void TCOD_path_get(TCOD_path_t path, int index, int *x, int *y);
TCODLIB_API void TCOD_path_get_origin(TCOD_path_t path, int *x, int *y);
TCODLIB_API void TCOD_path_get_destination(TCOD_path_t path, int *x, int *y);
TCODLIB_API void TCOD_path_delete(TCOD_path_t path);

#endif

/*
===========================================================================

Return to Castle Wolfenstein multiplayer GPL Source Code
Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company. 

This file is part of the Return to Castle Wolfenstein multiplayer GPL Source Code (RTCW MP Source Code).  

RTCW MP Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RTCW MP Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RTCW MP Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the RTCW MP Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the RTCW MP Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/*
** QGL_WIN.C
**
** This file implements the operating system binding of GL to QGL function
** pointers.  When doing a port of Quake3 you must implement the following
** two functions:
**
** QGL_Init() - loads libraries, assigns function pointers, etc.
** QGL_Shutdown() - unloads libraries, NULLs function pointers
*/
#include <float.h>
#include "../renderer/tr_local.h"
#include "glw_win.h"

void QGL_EnableLogging( qboolean enable );

int ( WINAPI * qwglSwapIntervalEXT )( int interval );
BOOL ( WINAPI * qwglGetDeviceGammaRamp3DFX )( HDC, LPVOID );
BOOL ( WINAPI * qwglSetDeviceGammaRamp3DFX )( HDC, LPVOID );

int ( WINAPI * qwglChoosePixelFormat )( HDC, CONST PIXELFORMATDESCRIPTOR * );
int ( WINAPI * qwglDescribePixelFormat )( HDC, int, UINT, LPPIXELFORMATDESCRIPTOR );
int ( WINAPI * qwglGetPixelFormat )( HDC );
BOOL ( WINAPI * qwglSetPixelFormat )( HDC, int, CONST PIXELFORMATDESCRIPTOR * );
BOOL ( WINAPI * qwglSwapBuffers )( HDC );

BOOL ( WINAPI * qwglCopyContext )( HGLRC, HGLRC, UINT );
HGLRC ( WINAPI * qwglCreateContext )( HDC );
HGLRC ( WINAPI * qwglCreateLayerContext )( HDC, int );
BOOL ( WINAPI * qwglDeleteContext )( HGLRC );
HGLRC ( WINAPI * qwglGetCurrentContext )( VOID );
HDC ( WINAPI * qwglGetCurrentDC )( VOID );
PROC ( WINAPI * qwglGetProcAddress )( LPCSTR );
BOOL ( WINAPI * qwglMakeCurrent )( HDC, HGLRC );
BOOL ( WINAPI * qwglShareLists )( HGLRC, HGLRC );
BOOL ( WINAPI * qwglUseFontBitmaps )( HDC, DWORD, DWORD, DWORD );

BOOL ( WINAPI * qwglUseFontOutlines )( HDC, DWORD, DWORD, DWORD, FLOAT,
				FLOAT, int, LPGLYPHMETRICSFLOAT );

BOOL ( WINAPI * qwglDescribeLayerPlane )( HDC, int, int, UINT,
				LPLAYERPLANEDESCRIPTOR );
int ( WINAPI * qwglSetLayerPaletteEntries )( HDC, int, int, int,
											 CONST COLORREF * );
int ( WINAPI * qwglGetLayerPaletteEntries )( HDC, int, int, int,
			   COLORREF * );
BOOL ( WINAPI * qwglRealizeLayerPalette )( HDC, int, BOOL );
BOOL ( WINAPI * qwglSwapLayerBuffers )( HDC, UINT );

#   pragma warning (disable : 4113 4133 4047 )
#   define GPA( a ) GetProcAddress( glw_state.hinstOpenGL, a )

/*
** QGL_Init
**
** This is responsible for binding our qgl function pointers to
** the appropriate GL stuff.  In Windows this means doing a
** LoadLibrary and a bunch of calls to GetProcAddress.  On other
** operating systems we need to do the right thing, whatever that
** might be.
*/
qboolean QGL_Init( const char *dllname ) {
	char systemDir[1024];
	char libName[1024];

	GetSystemDirectory( systemDir, sizeof( systemDir ) );

	assert( glw_state.hinstOpenGL == 0 );

	ri.Printf( PRINT_ALL, "...initializing QGL\n" );

	if ( dllname[0] != '!' && strstr( "dllname", ".dll" ) == NULL ) {
		Com_sprintf( libName, sizeof( libName ), "%s\\%s", systemDir, dllname );
	} else
	{
		Q_strncpyz( libName, dllname, sizeof( libName ) );
	}

	ri.Printf( PRINT_ALL, "...calling LoadLibrary( '%s.dll' ): ", libName );

	if ( ( glw_state.hinstOpenGL = LoadLibrary( dllname ) ) == 0 ) {
		ri.Printf( PRINT_ALL, "failed\n" );
		return qfalse;
	}
	ri.Printf( PRINT_ALL, "succeeded\n" );

	qwglCopyContext = (BOOL(WINAPI*)(HGLRC, HGLRC, UINT))GPA("wglCopyContext");
	qwglCreateContext = (HGLRC(WINAPI*)(HDC))GPA("wglCreateContext");
	qwglCreateLayerContext = (HGLRC(WINAPI*)(HDC, int))GPA("wglCreateLayerContext");
	qwglDeleteContext = (BOOL(WINAPI*)(HGLRC))GPA("wglDeleteContext");
	qwglDescribeLayerPlane = (BOOL(WINAPI*)(HDC, int, int, UINT, LAYERPLANEDESCRIPTOR*))GPA("wglDescribeLayerPlane");
	qwglGetCurrentContext = (HGLRC(WINAPI*)(void))GPA("wglGetCurrentContext");
	qwglGetCurrentDC = (HDC(WINAPI*)(void))GPA("wglGetCurrentDC");
	qwglGetLayerPaletteEntries = (int (WINAPI*)(HDC, int, int, int, COLORREF*))GPA("wglGetLayerPaletteEntries");
	qwglGetProcAddress = (PROC(WINAPI*)(LPCSTR))GPA("wglGetProcAddress");
	qwglMakeCurrent = (BOOL(WINAPI*)(HDC, HGLRC))GPA("wglMakeCurrent");
	qwglRealizeLayerPalette = (BOOL(WINAPI*)(HDC, int, BOOL))GPA("wglRealizeLayerPalette");
	qwglSetLayerPaletteEntries = (int (WINAPI*)(HDC, int, int, int, const COLORREF*))GPA("wglSetLayerPaletteEntries");
	qwglShareLists = (BOOL(WINAPI*)(HGLRC, HGLRC))GPA("wglShareLists");
	qwglSwapLayerBuffers = (BOOL(WINAPI*)(HDC, UINT))GPA("wglSwapLayerBuffers");
	qwglUseFontBitmaps = (BOOL(WINAPI*)(HDC, DWORD, DWORD, DWORD))GPA("wglUseFontBitmapsA");
	qwglUseFontOutlines = (BOOL(WINAPI*)(HDC, DWORD, DWORD, DWORD, FLOAT, FLOAT, int, LPGLYPHMETRICSFLOAT))GPA("wglUseFontOutlinesA");
	qwglChoosePixelFormat = (int (WINAPI*)(HDC, const PIXELFORMATDESCRIPTOR*))GPA("wglChoosePixelFormat");
	qwglDescribePixelFormat = (int (WINAPI*)(HDC, int, UINT, LPPIXELFORMATDESCRIPTOR))GPA("wglDescribePixelFormat");
	qwglGetPixelFormat = (int (WINAPI*)(HDC))GPA("wglGetPixelFormat");
	qwglSetPixelFormat = (BOOL(WINAPI*)(HDC, int, const PIXELFORMATDESCRIPTOR*))GPA("wglSetPixelFormat");
	qwglSwapBuffers = (BOOL(WINAPI*)(HDC))GPA("wglSwapBuffers");

	qwglSwapIntervalEXT = 0;
	qglActiveTextureARB = 0;
	qglClientActiveTextureARB = 0;
	qglMultiTexCoord2fARB = 0;
	qglLockArraysEXT = 0;
	qglUnlockArraysEXT = 0;
	qwglGetDeviceGammaRamp3DFX = NULL;
	qwglSetDeviceGammaRamp3DFX = NULL;

	return qtrue;
}

void QGL_EnableLogging( qboolean enable ) {

}

void QGL_Shutdown(void) {

}
#pragma warning (default : 4113 4133 4047 )




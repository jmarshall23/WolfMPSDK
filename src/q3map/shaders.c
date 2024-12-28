/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include <string.h>
#include <math.h>
#include "cmdlib.h"
#include "mathlib.h"
#include "imagelib.h"
#include "scriplib.h"

#include "../common/qfiles.h"
#include "../common/surfaceflags.h"

#include "shaders.h"
#include "../common/pakstuff.h"
#include "jpeglib.h"


// 5% backsplash by default
#define	DEFAULT_BACKSPLASH_FRACTION		0.05
#define	DEFAULT_BACKSPLASH_DISTANCE		24


#define	MAX_SURF_INFO	4096

shaderInfo_t	defaultInfo;
shaderInfo_t	shaderInfo[MAX_SURF_INFO];
int				numShaderInfo;


typedef struct {
	char	*name;
	int		contentFlags;
	int		contentFlagsClear;
	int		surfaceFlags;
	int		surfaceFlagsClear;
	int		compileFlags;
	int		compileFlagsClear;
} infoParm_t;

#define W_SURF_VERTEXLIT            ( SURF_POINTLIGHT | SURF_NOLIGHTMAP )

infoParm_t	infoParms[] = {
	/* name				contentFlags				contentFlagsClear			surfaceFlags				surfaceFlagsClear			compileFlags				compileFlagsClear */

		/* default */
		{ "default",        CONTENTS_SOLID,               -1,                         0,                          -1,                         C_SOLID,                    -1 },


		/* ydnar */
		{ "lightgrid",      0,                          0,                          0,                          0,                          C_LIGHTGRID,                0 },
		{ "antiportal",     0,                          0,                          0,                          0,                          C_ANTIPORTAL,               0 },
		{ "skip",           0,                          0,                          0,                          0,                          C_SKIP,                     0 },


		/* compiler */
		{ "origin",         CONTENTS_ORIGIN,              CONTENTS_SOLID,               0,                          0,                          C_ORIGIN | C_TRANSLUCENT,   C_SOLID },
		{ "areaportal",     CONTENTS_AREAPORTAL,          CONTENTS_SOLID,               0,                          0,                          C_AREAPORTAL | C_TRANSLUCENT,   C_SOLID },
		{ "trans",          CONTENTS_TRANSLUCENT,         0,                          0,                          0,                          C_TRANSLUCENT,              0 },
		{ "detail",         CONTENTS_DETAIL,              0,                          0,                          0,                          C_DETAIL,                   0 },
		{ "structural",     CONTENTS_STRUCTURAL,          0,                          0,                          0,                          C_STRUCTURAL,               0 },
		{ "hint",           0,                          0,                          SURF_HINT,                0,                          C_HINT,                     0 },
		{ "nodraw",         0,                          0,                          SURF_NODRAW,              0,                          C_NODRAW,                   0 },

		{ "alphashadow",    0,                          0,                          SURF_ALPHASHADOW,         0,                          C_ALPHASHADOW | C_TRANSLUCENT,  0 },
		{ "lightfilter",    0,                          0,                          SURF_LIGHTFILTER,         0,                          C_LIGHTFILTER | C_TRANSLUCENT,  0 },
		{ "nolightmap",     0,                          0,                          W_SURF_VERTEXLIT,           0,                          C_VERTEXLIT,                0 },
		{ "pointlight",     0,                          0,                          W_SURF_VERTEXLIT,           0,                          C_VERTEXLIT,                0 },


		/* game */
		{ "nonsolid",       0,                          CONTENTS_SOLID,               SURF_NONSOLID,            0,                          0,                          C_SOLID },

		{ "trigger",        CONTENTS_TRIGGER,             CONTENTS_SOLID,               0,                          0,                          C_TRANSLUCENT,              C_SOLID },

		{ "water",          CONTENTS_WATER,               CONTENTS_SOLID,               0,                          0,                          C_LIQUID | C_TRANSLUCENT,   C_SOLID },
		{ "slag",           CONTENTS_SLIME,               CONTENTS_SOLID,               0,                          0,                          C_LIQUID | C_TRANSLUCENT,   C_SOLID },
		{ "lava",           CONTENTS_LAVA,                CONTENTS_SOLID,               0,                          0,                          C_LIQUID | C_TRANSLUCENT,   C_SOLID },

		{ "playerclip",     CONTENTS_PLAYERCLIP,          CONTENTS_SOLID,               0,                          0,                          C_DETAIL | C_TRANSLUCENT,   C_SOLID },
		{ "monsterclip",    CONTENTS_MONSTERCLIP,         CONTENTS_SOLID,               0,                          0,                          C_DETAIL | C_TRANSLUCENT,   C_SOLID },
		{ "clipmissile",    CONTENTS_MISSILECLIP,         CONTENTS_SOLID,               0,                          0,                          C_DETAIL | C_TRANSLUCENT,   C_SOLID },
		{ "clipshot",       CONTENTS_CLIPSHOT,            CONTENTS_SOLID,               0,                          0,                          C_DETAIL | C_TRANSLUCENT,   C_SOLID },
		{ "nodrop",         CONTENTS_NODROP,              CONTENTS_SOLID,               0,                          0,                          C_TRANSLUCENT,              C_SOLID },

		{ "clusterportal",  CONTENTS_CLUSTERPORTAL,       CONTENTS_SOLID,               0,                          0,                          C_TRANSLUCENT,              C_SOLID },
		{ "donotenter",     CONTENTS_DONOTENTER,          CONTENTS_SOLID,               0,                          0,                          C_TRANSLUCENT,              C_SOLID },
		{ "nonotenterlarge",CONTENTS_DONOTENTER_LARGE,    CONTENTS_SOLID,               0,                          0,                          C_TRANSLUCENT,              C_SOLID },

		{ "fog",            CONTENTS_FOG,                 CONTENTS_SOLID,               0,                          0,                          C_FOG,                      C_SOLID },
		{ "sky",            0,                          0,                          SURF_SKY,                 0,                          C_SKY,                      0 },

		{ "slick",          0,                          0,                          SURF_SLICK,               0,                          0,                          0 },

		{ "noimpact",       0,                          0,                          SURF_NOIMPACT,            0,                          0,                          0 },
		{ "nomarks",        0,                          0,                          SURF_NOMARKS,             0,                          C_NOMARKS,                  0 },
		{ "ladder",         0,                          0,                          SURF_LADDER,              0,                          0,                          0 },
		{ "nodamage",       0,                          0,                          SURF_NODAMAGE,            0,                          0,                          0 },
		{ "nosteps",        0,                          0,                          SURF_NOSTEPS,             0,                          0,                          0 },
		{ "nodlight",       0,                          0,                          SURF_NODLIGHT,            0,                          0,                          0 },


		/* materials */
		{ "metal",          0,                          0,                          SURF_METAL,               0,                          0,                          0 },
		{ "metalsteps",     0,                          0,                          SURF_METAL,               0,                          0,                          0 },
		{ "glass",          0,                          0,                          SURF_GLASS,               0,                          0,                          0 },
		{ "ceramic",        0,                          0,                          SURF_CERAMIC,             0,                          0,                          0 },
		{ "woodsteps",      0,                          0,                          SURF_WOOD,                0,                          0,                          0 },
		{ "grasssteps",     0,                          0,                          SURF_GRASS,               0,                          0,                          0 },
		{ "gravelsteps",    0,                          0,                          SURF_GRAVEL,              0,                          0,                          0 },
		{ "rubble",         0,                          0,                          SURF_RUBBLE,              0,                          0,                          0 },
		{ "carpetsteps",    0,                          0,                          SURF_CARPET,              0,                          0,                          0 },
		{ "snowsteps",      0,                          0,                          SURF_SNOW,                0,                          0,                          0 },
		{ "roofsteps",      0,                          0,                          SURF_ROOF,                0,                          0,                          0 },


		/* ai */
		{ "ai_nosight",     CONTENTS_AI_NOSIGHT,          CONTENTS_SOLID,               0,                          0,                          C_TRANSLUCENT,              C_SOLID },

		/* ydnar: experimental until bits are confirmed! */
		{ "ai_nopass",      CONTENTS_DONOTENTER,          CONTENTS_SOLID,               0,                          0,                          C_TRANSLUCENT,              C_SOLID },
		{ "ai_nopasslarge", CONTENTS_DONOTENTER_LARGE,    CONTENTS_SOLID,               0,                          0,                          C_TRANSLUCENT,              C_SOLID },


		/* sliding bodies */
		{ "monsterslick",   0,                          0,                          SURF_MONSTERSLICK,        0,                          C_TRANSLUCENT,              0 },
		{ "monsterslicknorth",  0,                      0,                          SURF_MONSLICK_N,          0,                          C_TRANSLUCENT,              0 },
		{ "monsterslickeast",   0,                      0,                          SURF_MONSLICK_E,          0,                          C_TRANSLUCENT,              0 },
		{ "monsterslicksouth",  0,                      0,                          SURF_MONSLICK_S,          0,                          C_TRANSLUCENT,              0 },
		{ "monsterslickwest",   0,                      0,                          SURF_MONSLICK_W,          0,                          C_TRANSLUCENT,              0 },

};


/*
===============
LoadShaderImage
===============
*/

byte* LoadImageFile(char *filename, qboolean *bTGA)
{
  byte *buffer = NULL;
  int nLen = 0;
  *bTGA = qtrue;
  if (FileExists(filename))
  {
    LoadFileBlock(filename, &buffer);
  }
#ifdef _WIN32
  else
  {
    PakLoadAnyFile(filename, &buffer);
  }
#endif
  if ( buffer == NULL)
  {
    nLen = strlen(filename);
    filename[nLen-3] = 'j';
    filename[nLen-2] = 'p';
    filename[nLen-1] = 'g';
    if (FileExists(filename))
    {
      LoadFileBlock(filename, &buffer);
    }
#ifdef _WIN32
    else
    {
      PakLoadAnyFile(filename, &buffer);
    }
#endif
    if ( buffer )
    {
      *bTGA = qfalse;
    }
  }
  return buffer;
}

/*
===============
LoadShaderImage
===============
*/
static void LoadShaderImage( shaderInfo_t *si ) {
	char			filename[1024];
	int				i, count;
	float			color[4];
  byte      *buffer;
  qboolean  bTGA = qtrue;

	// look for the lightimage if it is specified
	if ( si->lightimage[0] ) {
		sprintf( filename, "%s%s", gamedir, si->lightimage );
		DefaultExtension( filename, ".tga" );
    buffer = LoadImageFile(filename, &bTGA);
    if ( buffer != NULL) {
      goto loadTga;
    }
  }

	// look for the editorimage if it is specified
	if ( si->editorimage[0] ) {
		sprintf( filename, "%s%s", gamedir, si->editorimage );
		DefaultExtension( filename, ".tga" );
    buffer = LoadImageFile(filename, &bTGA);
    if ( buffer != NULL) {
      goto loadTga;
    }
  }

  // just try the shader name with a .tga
	// on unix, we have case sensitivity problems...
  sprintf( filename, "%s%s.tga", gamedir, si->shader );
  buffer = LoadImageFile(filename, &bTGA);
  if ( buffer != NULL) {
		goto loadTga;
	}

  sprintf( filename, "%s%s.TGA", gamedir, si->shader );
  buffer = LoadImageFile(filename, &bTGA);
  if ( buffer != NULL) {
		goto loadTga;
	}

	// couldn't load anything
	_printf("WARNING: Couldn't find image for shader %s\n", si->shader );

	si->color[0] = 1;
	si->color[1] = 1;
	si->color[2] = 1;
	si->width = 64;
	si->height = 64;
	si->pixels = malloc( si->width * si->height * 4 );
	memset ( si->pixels, 255, si->width * si->height * 4 );
	return;

	// load the image to get dimensions and color
loadTga:
  if ( bTGA) {
	  LoadTGABuffer( buffer, &si->pixels, &si->width, &si->height );
  }
  else {
#ifdef _WIN32
    LoadJPGBuff(buffer, &si->pixels, &si->width, &si->height );
#endif
  }

  free(buffer);

	count = si->width * si->height;

	VectorClear( color );
	color[ 3 ] = 0;
	for ( i = 0 ; i < count ; i++ ) {
		color[0] += si->pixels[ i * 4 + 0 ];
		color[1] += si->pixels[ i * 4 + 1 ];
		color[2] += si->pixels[ i * 4 + 2 ];
		color[3] += si->pixels[ i * 4 + 3 ];
	}
	ColorNormalize( color, si->color );
	VectorScale( color, 1.0/count, si->averageColor );
}

/*
===============
AllocShaderInfo
===============
*/
static shaderInfo_t	*AllocShaderInfo( void ) {
	shaderInfo_t	*si;

	if ( numShaderInfo == MAX_SURF_INFO ) {
		Error( "MAX_SURF_INFO" );
	}
	si = &shaderInfo[ numShaderInfo ];
	numShaderInfo++;

	// set defaults

	si->contents = CONTENTS_SOLID;

	si->backsplashFraction = DEFAULT_BACKSPLASH_FRACTION;
	si->backsplashDistance = DEFAULT_BACKSPLASH_DISTANCE;

	si->lightmapSampleSize = 0;
	si->forceTraceLight = qfalse;
	si->forceVLight = qfalse;
	si->patchShadows = qfalse;
	si->vertexShadows = qfalse;
	si->noVertexShadows = qfalse;
	si->forceSunLight = qfalse;
	si->vertexScale = 1.0;
	si->notjunc = qfalse;

	return si;
}

/*
===============
ShaderInfoForShader
===============
*/
shaderInfo_t	*ShaderInfoForShader( const char *shaderName ) {
	int				i;
	shaderInfo_t	*si;
	char			shader[MAX_QPATH];

	// strip off extension
	strcpy( shader, shaderName );
	StripExtension( shader );

	// search for it
	for ( i = 0 ; i < numShaderInfo ; i++ ) {
		si = &shaderInfo[ i ];
		if ( !Q_stricmp( shader, si->shader ) ) {
			if ( !si->width ) {
				LoadShaderImage( si );
			}
			return si;
		}
	}

	si = AllocShaderInfo();
	strcpy( si->shader, shader );

	LoadShaderImage( si );

	return si;
}

/*
===============
ParseShaderFile
===============
*/
static void ParseShaderFile( const char *filename ) {
	int		i;
	int		numInfoParms = sizeof(infoParms) / sizeof(infoParms[0]);
	shaderInfo_t	*si;

//	qprintf( "shaderFile: %s\n", filename );
	LoadScriptFile( filename );
	while ( 1 ) {
		if ( !GetToken( qtrue ) ) {
			break;
		}

		si = AllocShaderInfo();
		strcpy( si->shader, token );
		MatchToken( "{" );
		while ( 1 ) {
			if ( !GetToken( qtrue ) ) {
				break;
			}
			if ( !strcmp( token, "}" ) ) {
				break;
			}

			// skip internal braced sections
			if ( !strcmp( token, "{" ) ) {
				si->hasPasses = qtrue;
				while ( 1 ) {
					if ( !GetToken( qtrue ) ) {
						break;
					}
					if ( !strcmp( token, "}" ) ) {
						break;
					}
				}
				continue;
			}

			if ( !Q_stricmp( token, "surfaceparm" ) ) {
				GetToken( qfalse );
				for ( i = 0 ; i < numInfoParms ; i++ ) {
					if ( !Q_stricmp( token, infoParms[i].name ) ) {
						si->surfaceFlags |= infoParms[i].surfaceFlags;
						si->contents |= infoParms[i].contentFlags;
						//if ( infoParms[i].clearSolid ) {
						//	si->contents &= ~CONTENTS_SOLID;
						//}
						break;
					}
				}
				if ( i == numInfoParms ) {
					// we will silently ignore all tokens beginning with qer,
					// which are QuakeEdRadient parameters
					if ( Q_strncasecmp( token, "qer", 3 ) ) {
						_printf( "Unknown surfaceparm: \"%s\"\n", token );
					}
				}
				continue;
			}


			// qer_editorimage <image>
			if ( !Q_stricmp( token, "qer_editorimage" ) ) {
				GetToken( qfalse );
				strcpy( si->editorimage, token );
				DefaultExtension( si->editorimage, ".tga" );
				continue;
			}

			// q3map_lightimage <image>
			if ( !Q_stricmp( token, "q3map_lightimage" ) ) {
				GetToken( qfalse );
				strcpy( si->lightimage, token );
				DefaultExtension( si->lightimage, ".tga" );
				continue;
			}

			// q3map_surfacelight <value>
			if ( !Q_stricmp( token, "q3map_surfacelight" )  ) {
				GetToken( qfalse );
				si->value = atoi( token );
				continue;
			}

			// q3map_lightsubdivide <value>
			if ( !Q_stricmp( token, "q3map_lightsubdivide" )  ) {
				GetToken( qfalse );
				si->lightSubdivide = atoi( token );
				continue;
			}

			// q3map_lightmapsamplesize <value>
			if ( !Q_stricmp( token, "q3map_lightmapsamplesize" ) ) {
				GetToken( qfalse );
				si->lightmapSampleSize = atoi( token );
				continue;
			}

			// q3map_tracelight
			if ( !Q_stricmp( token, "q3map_tracelight" ) ) {
				si->forceTraceLight = qtrue;
				continue;
			}

			// q3map_vlight
			if ( !Q_stricmp( token, "q3map_vlight" ) ) {
				si->forceVLight = qtrue;
				continue;
			}

			// q3map_patchshadows
			if ( !Q_stricmp( token, "q3map_patchshadows" ) ) {
				si->patchShadows = qtrue;
				continue;
			}

			// q3map_vertexshadows
			if ( !Q_stricmp( token, "q3map_vertexshadows" ) ) {
				si->vertexShadows = qtrue;
				continue;
			}

			// q3map_novertexshadows
			if ( !Q_stricmp( token, "q3map_novertexshadows" ) ) {
				si->noVertexShadows = qtrue;
				continue;
			}

			// q3map_forcesunlight
			if ( !Q_stricmp( token, "q3map_forcesunlight" ) ) {
				si->forceSunLight = qtrue;
				continue;
			}

			// q3map_vertexscale
			if ( !Q_stricmp( token, "q3map_vertexscale" ) ) {
				GetToken( qfalse );
				si->vertexScale = atof(token);
				continue;
			}

			// q3map_notjunc
			if ( !Q_stricmp( token, "q3map_notjunc" ) ) {
				si->notjunc = qtrue;
				continue;
			}

			// q3map_globaltexture
			if ( !Q_stricmp( token, "q3map_globaltexture" )  ) {
				si->globalTexture = qtrue;
				continue;
			}

			// q3map_backsplash <percent> <distance>
			if ( !Q_stricmp( token, "q3map_backsplash" ) ) {
				GetToken( qfalse );
				si->backsplashFraction = atof( token ) * 0.01;
				GetToken( qfalse );
				si->backsplashDistance = atof( token );
				continue;
			}

			// q3map_backshader <shader>
			if ( !Q_stricmp( token, "q3map_backshader" ) ) {
				GetToken( qfalse );
				strcpy( si->backShader, token );
				continue;
			}

			// q3map_flare <shader>
			if ( !Q_stricmp( token, "q3map_flare" ) ) {
				GetToken( qfalse );
				strcpy( si->flareShader, token );
				continue;
			}

			// light <value> 
			// old style flare specification
			if ( !Q_stricmp( token, "light" ) ) {
				GetToken( qfalse );
				strcpy( si->flareShader, "flareshader" );
				continue;
			}

			// q3map_sun <red> <green> <blue> <intensity> <degrees> <elivation>
			// color will be normalized, so it doesn't matter what range you use
			// intensity falls off with angle but not distance 100 is a fairly bright sun
			// degree of 0 = from the east, 90 = north, etc.  altitude of 0 = sunrise/set, 90 = noon
			if ( !Q_stricmp( token, "q3map_sun" ) ) {
				float	a, b;

				GetToken( qfalse );
				si->sunLight[0] = atof( token );
				GetToken( qfalse );
				si->sunLight[1] = atof( token );
				GetToken( qfalse );
				si->sunLight[2] = atof( token );
				
				VectorNormalize( si->sunLight, si->sunLight);

				GetToken( qfalse );
				a = atof( token );
				VectorScale( si->sunLight, a, si->sunLight);

				GetToken( qfalse );
				a = atof( token );
				a = a / 180 * Q_PI;

				GetToken( qfalse );
				b = atof( token );
				b = b / 180 * Q_PI;

				si->sunDirection[0] = cos( a ) * cos( b );
				si->sunDirection[1] = sin( a ) * cos( b );
				si->sunDirection[2] = sin( b );

				si->surfaceFlags |= SURF_SKY;
				continue;
			}

			// tesssize is used to force liquid surfaces to subdivide
			if ( !Q_stricmp( token, "tesssize" ) ) {
				GetToken( qfalse );
				si->subdivisions = atof( token );
				continue;
			}

			// cull none will set twoSided
			if ( !Q_stricmp( token, "cull" ) ) {
				GetToken( qfalse );
				if ( !Q_stricmp( token, "none" ) ) {
					si->twoSided = qtrue;
				}
				continue;
			}


			// deformVertexes autosprite[2]
			// we catch this so autosprited surfaces become point
			// lights instead of area lights
			if ( !Q_stricmp( token, "deformVertexes" ) ) {
				GetToken( qfalse );
				if ( !Q_strncasecmp( token, "autosprite", 10 ) ) {
					si->autosprite = qtrue;
          si->contents = CONTENTS_DETAIL;
				}
				continue;
			}


			// ignore all other tokens on the line

			while ( TokenAvailable() ) {
				GetToken( qfalse );
			}
		}			
	}
}

/*
===============
LoadShaderInfo
===============
*/
#define	MAX_SHADER_FILES	64
void LoadShaderInfo( void ) {
	char			filename[1024];
	int				i;
	char			*shaderFiles[MAX_SHADER_FILES];
	int				numShaderFiles;

	sprintf( filename, "%sscripts/shaderlist.txt", gamedir );
	LoadScriptFile( filename );

	numShaderFiles = 0;
	while ( 1 ) {
		if ( !GetToken( qtrue ) ) {
			break;
		}
    shaderFiles[numShaderFiles] = malloc(MAX_OS_PATH);
		strcpy( shaderFiles[ numShaderFiles ], token );
		numShaderFiles++;
	}

	for ( i = 0 ; i < numShaderFiles ; i++ ) {
		sprintf( filename, "%sscripts/%s.shader", gamedir, shaderFiles[i] );
		ParseShaderFile( filename );
    free(shaderFiles[i]);
	}

	qprintf( "%5i shaderInfo\n", numShaderInfo);
}


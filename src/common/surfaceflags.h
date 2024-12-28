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
// This file must be identical in the quake and utils directories

// contents flags are seperate bits
// a given brush can contribute multiple content bits

// these definitions also need to be in q_shared.h!

#define C_SOLID                 0x00000001
#define C_TRANSLUCENT           0x00000002
#define C_STRUCTURAL            0x00000004
#define C_HINT                  0x00000008
#define C_NODRAW                0x00000010
#define C_LIGHTGRID             0x00000020
#define C_ALPHASHADOW           0x00000040
#define C_LIGHTFILTER           0x00000080
#define C_VERTEXLIT             0x00000100
#define C_LIQUID                0x00000200
#define C_FOG                   0x00000400
#define C_SKY                   0x00000800
#define C_ORIGIN                0x00001000
#define C_AREAPORTAL            0x00002000
#define C_ANTIPORTAL            0x00004000  /* like hint, but doesn't generate portals */
#define C_SKIP                  0x00008000  /* like hint, but skips this face (doesn't split bsp) */
#define C_NOMARKS               0x00010000  /* no decals */
#define C_DETAIL                0x08000000  /* THIS MUST BE THE SAME AS IN RADIANT! */

/* game flags */
#define CONTENTS_SOLID                1           /* an eye is never valid in a solid */
#define CONTENTS_LAVA                 8
#define CONTENTS_SLIME                16
#define CONTENTS_WATER                32
#define CONTENTS_FOG                  64

#define CONTENTS_MISSILECLIP          0x80        /* wolf ranged missile blocking */
#define CONTENTS_ITEM                 0x100       /* wolf item contents */
#define CONTENTS_AI_NOSIGHT           0x1000      /* wolf ai sight blocking */
#define CONTENTS_CLIPSHOT             0x2000      /* wolf shot clip */
#define CONTENTS_AREAPORTAL           0x8000

#define CONTENTS_PLAYERCLIP           0x10000
#define CONTENTS_MONSTERCLIP          0x20000
#define CONTENTS_TELEPORTER           0x40000
#define CONTENTS_JUMPPAD              0x80000
#define CONTENTS_CLUSTERPORTAL        0x100000
#define CONTENTS_DONOTENTER           0x200000
#define CONTENTS_DONOTENTER_LARGE     0x400000    /* wolf dne */

#define CONTENTS_ORIGIN               0x1000000   /* removed before bsping an entity */

#define CONTENTS_BODY                 0x2000000   /* should never be on a brush, only in game */
#define CONTENTS_CORPSE               0x4000000
#define CONTENTS_DETAIL               0x8000000   /* brushes not used for the bsp */
#define CONTENTS_STRUCTURAL           0x10000000  /* brushes used for the bsp */
#define CONTENTS_TRANSLUCENT          0x20000000  /* don't consume surface fragments inside */
#define CONTENTS_TRIGGER              0x40000000
#define CONTENTS_NODROP               0x80000000  /* don't leave bodies or items (death fog, lava) */

#define SURF_NODAMAGE             0x1         /* never give falling damage */
#define SURF_SLICK                0x2         /* effects game physics */
#define SURF_SKY                  0x4         /* lighting from environment map */
#define SURF_LADDER               0x8
#define SURF_NOIMPACT             0x10        /* don't make missile explosions */
#define SURF_NOMARKS              0x20        /* don't leave missile marks */
#define SURF_CERAMIC              0x40        /* wolf ceramic material */
#define SURF_NODRAW               0x80        /* don't generate a drawsurface at all */
#define SURF_HINT                 0x100       /* make a primary bsp splitter */
#define SURF_SKIP                 0x200       /* completely ignore, allowing non-closed brushes */
#define SURF_NOLIGHTMAP           0x400       /* surface doesn't need a lightmap */
#define SURF_POINTLIGHT           0x800       /* generate lighting info at vertexes */
#define SURF_METAL                0x1000      /* wolf metal material */
#define SURF_NOSTEPS              0x2000      /* no footstep sounds */
#define SURF_NONSOLID             0x4000      /* don't collide against curves with this set */
#define SURF_LIGHTFILTER          0x8000      /* act as a light filter during q3map -light */
#define SURF_ALPHASHADOW          0x10000     /* do per-pixel light shadow casting in q3map */
#define SURF_NODLIGHT             0x20000     /* don't dlight even if solid (solid lava, skies) */
#define SURF_WOOD                 0x40000     /* wolf wood material */
#define SURF_GRASS                0x80000     /* wolf grass material */
#define SURF_GRAVEL               0x100000    /* wolf gravel material */
#define SURF_GLASS                0x200000    /* wolf glass material */
#define SURF_SNOW                 0x400000    /* wolf snow material */
#define SURF_ROOF                 0x800000    /* wolf roof material */
#define SURF_RUBBLE               0x1000000   /* wolf rubble material */
#define SURF_CARPET               0x2000000   /* wolf carpet material */
#define SURF_MONSTERSLICK         0x4000000   /* wolf npc slick surface */
#define SURF_MONSLICK_W           0x8000000   /* wolf slide bodies west */
#define SURF_MONSLICK_N           0x10000000  /* wolf slide bodies north */
#define SURF_MONSLICK_E           0x20000000  /* wolf slide bodies east */
#define SURF_MONSLICK_S           0x40000000  /* wolf slide bodies south */

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


#include "client.h"

#include "../game/botlib.h"

extern botlib_export_t *botlib_export;

static DllLoader* uidll = nullptr;
uiExport_t *uivm;

/*
====================
GetClientState
====================
*/
static void GetClientState( uiClientState_t *state ) {
	state->connectPacketCount = clc.connectPacketCount;
	state->connState = cls.state;
	Q_strncpyz( state->servername, cls.servername, sizeof( state->servername ) );
	Q_strncpyz( state->updateInfoString, cls.updateInfoString, sizeof( state->updateInfoString ) );
	Q_strncpyz( state->messageString, clc.serverMessage, sizeof( state->messageString ) );
	state->clientNum = cl.snap.ps.clientNum;
}

/*
====================
LAN_LoadCachedServers
====================
*/
void LAN_LoadCachedServers() {
	int size;
	fileHandle_t fileIn;
	cls.numglobalservers = cls.nummplayerservers = cls.numfavoriteservers = 0;
	cls.numGlobalServerAddresses = 0;
	if ( FS_SV_FOpenFileRead( "servercache.dat", &fileIn ) ) {
		FS_Read( &cls.numglobalservers, sizeof( int ), fileIn );
		FS_Read( &cls.nummplayerservers, sizeof( int ), fileIn );
		FS_Read( &cls.numfavoriteservers, sizeof( int ), fileIn );
		FS_Read( &size, sizeof( int ), fileIn );
		if ( size == sizeof( cls.globalServers ) + sizeof( cls.favoriteServers ) + sizeof( cls.mplayerServers ) ) {
			FS_Read( &cls.globalServers, sizeof( cls.globalServers ), fileIn );
			FS_Read( &cls.mplayerServers, sizeof( cls.mplayerServers ), fileIn );
			FS_Read( &cls.favoriteServers, sizeof( cls.favoriteServers ), fileIn );
		} else {
			cls.numglobalservers = cls.nummplayerservers = cls.numfavoriteservers = 0;
			cls.numGlobalServerAddresses = 0;
		}
		FS_FCloseFile( fileIn );
	}
}

/*
====================
LAN_SaveServersToCache
====================
*/
void LAN_SaveServersToCache() {
	int size;
	fileHandle_t fileOut;
#ifdef __MACOS__    //DAJ MacOS file typing
	{
		extern _MSL_IMP_EXP_C long _fcreator, _ftype;
		_ftype = 'WlfB';
		_fcreator = 'WlfM';
	}
#endif
	fileOut = FS_SV_FOpenFileWrite( "servercache.dat" );
	FS_Write( &cls.numglobalservers, sizeof( int ), fileOut );
	FS_Write( &cls.nummplayerservers, sizeof( int ), fileOut );
	FS_Write( &cls.numfavoriteservers, sizeof( int ), fileOut );
	size = sizeof( cls.globalServers ) + sizeof( cls.favoriteServers ) + sizeof( cls.mplayerServers );
	FS_Write( &size, sizeof( int ), fileOut );
	FS_Write( &cls.globalServers, sizeof( cls.globalServers ), fileOut );
	FS_Write( &cls.mplayerServers, sizeof( cls.mplayerServers ), fileOut );
	FS_Write( &cls.favoriteServers, sizeof( cls.favoriteServers ), fileOut );
	FS_FCloseFile( fileOut );
}


/*
====================
LAN_ResetPings
====================
*/
static void LAN_ResetPings( int source ) {
	int count,i;
	serverInfo_t *servers = NULL;
	count = 0;

	switch ( source ) {
	case AS_LOCAL:
		servers = &cls.localServers[0];
		count = MAX_OTHER_SERVERS;
		break;
	case AS_MPLAYER:
		servers = &cls.mplayerServers[0];
		count = MAX_OTHER_SERVERS;
		break;
	case AS_GLOBAL:
		servers = &cls.globalServers[0];
		count = MAX_GLOBAL_SERVERS;
		break;
	case AS_FAVORITES:
		servers = &cls.favoriteServers[0];
		count = MAX_OTHER_SERVERS;
		break;
	}
	if ( servers ) {
		for ( i = 0; i < count; i++ ) {
			servers[i].ping = -1;
		}
	}
}

/*
====================
LAN_AddServer
====================
*/
static int LAN_AddServer( int source, const char *name, const char *address ) {
	int max, *count, i;
	netadr_t adr;
	serverInfo_t *servers = NULL;
	max = MAX_OTHER_SERVERS;
	count = 0;

	switch ( source ) {
	case AS_LOCAL:
		count = &cls.numlocalservers;
		servers = &cls.localServers[0];
		break;
	case AS_MPLAYER:
		count = &cls.nummplayerservers;
		servers = &cls.mplayerServers[0];
		break;
	case AS_GLOBAL:
		max = MAX_GLOBAL_SERVERS;
		count = &cls.numglobalservers;
		servers = &cls.globalServers[0];
		break;
	case AS_FAVORITES:
		count = &cls.numfavoriteservers;
		servers = &cls.favoriteServers[0];
		break;
	}
	if ( servers && *count < max ) {
		NET_StringToAdr( address, &adr );
		for ( i = 0; i < *count; i++ ) {
			if ( NET_CompareAdr( servers[i].adr, adr ) ) {
				break;
			}
		}
		if ( i >= *count ) {
			servers[*count].adr = adr;
			Q_strncpyz( servers[*count].hostName, name, sizeof( servers[*count].hostName ) );
			servers[*count].visible = qtrue;
			( *count )++;
			return 1;
		}
		return 0;
	}
	return -1;
}

/*
====================
LAN_RemoveServer
====================
*/
static void LAN_RemoveServer( int source, const char *addr ) {
	int *count, i;
	serverInfo_t *servers = NULL;
	count = 0;
	switch ( source ) {
	case AS_LOCAL:
		count = &cls.numlocalservers;
		servers = &cls.localServers[0];
		break;
	case AS_MPLAYER:
		count = &cls.nummplayerservers;
		servers = &cls.mplayerServers[0];
		break;
	case AS_GLOBAL:
		count = &cls.numglobalservers;
		servers = &cls.globalServers[0];
		break;
	case AS_FAVORITES:
		count = &cls.numfavoriteservers;
		servers = &cls.favoriteServers[0];
		break;
	}
	if ( servers ) {
		netadr_t comp;
		NET_StringToAdr( addr, &comp );
		for ( i = 0; i < *count; i++ ) {
			if ( NET_CompareAdr( comp, servers[i].adr ) ) {
				int j = i;
				while ( j < *count - 1 ) {
					Com_Memcpy( &servers[j], &servers[j + 1], sizeof( servers[j] ) );
					j++;
				}
				( *count )--;
				break;
			}
		}
	}
}


/*
====================
LAN_GetServerCount
====================
*/
static int LAN_GetServerCount( int source ) {
	switch ( source ) {
	case AS_LOCAL:
		return cls.numlocalservers;
		break;
	case AS_MPLAYER:
		return cls.nummplayerservers;
		break;
	case AS_GLOBAL:
		return cls.numglobalservers;
		break;
	case AS_FAVORITES:
		return cls.numfavoriteservers;
		break;
	}
	return 0;
}

/*
====================
LAN_GetLocalServerAddressString
====================
*/
static void LAN_GetServerAddressString( int source, int n, char *buf, int buflen ) {
	switch ( source ) {
	case AS_LOCAL:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			Q_strncpyz( buf, NET_AdrToString( cls.localServers[n].adr ), buflen );
			return;
		}
		break;
	case AS_MPLAYER:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			Q_strncpyz( buf, NET_AdrToString( cls.mplayerServers[n].adr ), buflen );
			return;
		}
		break;
	case AS_GLOBAL:
		if ( n >= 0 && n < MAX_GLOBAL_SERVERS ) {
			Q_strncpyz( buf, NET_AdrToString( cls.globalServers[n].adr ), buflen );
			return;
		}
		break;
	case AS_FAVORITES:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			Q_strncpyz( buf, NET_AdrToString( cls.favoriteServers[n].adr ), buflen );
			return;
		}
		break;
	}
	buf[0] = '\0';
}

/*
====================
LAN_GetServerInfo
====================
*/
static void LAN_GetServerInfo( int source, int n, char *buf, int buflen ) {
	char info[MAX_STRING_CHARS];
	serverInfo_t *server = NULL;
	info[0] = '\0';
	switch ( source ) {
	case AS_LOCAL:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			server = &cls.localServers[n];
		}
		break;
	case AS_MPLAYER:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			server = &cls.mplayerServers[n];
		}
		break;
	case AS_GLOBAL:
		if ( n >= 0 && n < MAX_GLOBAL_SERVERS ) {
			server = &cls.globalServers[n];
		}
		break;
	case AS_FAVORITES:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			server = &cls.favoriteServers[n];
		}
		break;
	}
	if ( server && buf ) {
		buf[0] = '\0';
		Info_SetValueForKey( info, "hostname", server->hostName );
		Info_SetValueForKey( info, "mapname", server->mapName );
		Info_SetValueForKey( info, "clients", va( "%i",server->clients ) );
		Info_SetValueForKey( info, "sv_maxclients", va( "%i",server->maxClients ) );
		Info_SetValueForKey( info, "ping", va( "%i",server->ping ) );
		Info_SetValueForKey( info, "minping", va( "%i",server->minPing ) );
		Info_SetValueForKey( info, "maxping", va( "%i",server->maxPing ) );
		Info_SetValueForKey( info, "game", server->game );
		Info_SetValueForKey( info, "gametype", va( "%i",server->gameType ) );
		Info_SetValueForKey( info, "nettype", va( "%i",server->netType ) );
		Info_SetValueForKey( info, "addr", NET_AdrToString( server->adr ) );
		Info_SetValueForKey( info, "sv_allowAnonymous", va( "%i", server->allowAnonymous ) );
		Info_SetValueForKey( info, "friendlyFire", va( "%i", server->friendlyFire ) );               // NERVE - SMF
		Info_SetValueForKey( info, "maxlives", va( "%i", server->maxlives ) );                       // NERVE - SMF
		Info_SetValueForKey( info, "tourney", va( "%i", server->tourney ) );                     // NERVE - SMF
		Info_SetValueForKey( info, "punkbuster", va( "%i", server->punkbuster ) );                   // DHM - Nerve
		Info_SetValueForKey( info, "gamename", server->gameName );                                // Arnout
		Info_SetValueForKey( info, "g_antilag", va( "%i", server->antilag ) ); // TTimo
		Q_strncpyz( buf, info, buflen );
	} else {
		if ( buf ) {
			buf[0] = '\0';
		}
	}
}

/*
====================
LAN_GetServerPing
====================
*/
static int LAN_GetServerPing( int source, int n ) {
	serverInfo_t *server = NULL;
	switch ( source ) {
	case AS_LOCAL:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			server = &cls.localServers[n];
		}
		break;
	case AS_MPLAYER:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			server = &cls.mplayerServers[n];
		}
		break;
	case AS_GLOBAL:
		if ( n >= 0 && n < MAX_GLOBAL_SERVERS ) {
			server = &cls.globalServers[n];
		}
		break;
	case AS_FAVORITES:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			server = &cls.favoriteServers[n];
		}
		break;
	}
	if ( server ) {
		return server->ping;
	}
	return -1;
}

/*
====================
LAN_GetServerPtr
====================
*/
static serverInfo_t *LAN_GetServerPtr( int source, int n ) {
	switch ( source ) {
	case AS_LOCAL:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			return &cls.localServers[n];
		}
		break;
	case AS_MPLAYER:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			return &cls.mplayerServers[n];
		}
		break;
	case AS_GLOBAL:
		if ( n >= 0 && n < MAX_GLOBAL_SERVERS ) {
			return &cls.globalServers[n];
		}
		break;
	case AS_FAVORITES:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			return &cls.favoriteServers[n];
		}
		break;
	}
	return NULL;
}

/*
====================
LAN_CompareServers
====================
*/
static int LAN_CompareServers( int source, int sortKey, int sortDir, int s1, int s2 ) {
	int res;
	serverInfo_t *server1, *server2;

	server1 = LAN_GetServerPtr( source, s1 );
	server2 = LAN_GetServerPtr( source, s2 );
	if ( !server1 || !server2 ) {
		return 0;
	}

	res = 0;
	switch ( sortKey ) {
	case SORT_HOST:
		res = Q_stricmp( server1->hostName, server2->hostName );
		break;

	case SORT_MAP:
		res = Q_stricmp( server1->mapName, server2->mapName );
		break;
	case SORT_CLIENTS:
		if ( server1->clients < server2->clients ) {
			res = -1;
		} else if ( server1->clients > server2->clients )     {
			res = 1;
		} else {
			res = 0;
		}
		break;
	case SORT_GAME:
		if ( server1->gameType < server2->gameType ) {
			res = -1;
		} else if ( server1->gameType > server2->gameType )     {
			res = 1;
		} else {
			res = 0;
		}
		break;
	case SORT_PING:
		if ( server1->ping < server2->ping ) {
			res = -1;
		} else if ( server1->ping > server2->ping )     {
			res = 1;
		} else {
			res = 0;
		}
		break;
	case SORT_PUNKBUSTER:
		if ( server1->punkbuster < server2->punkbuster ) {
			res = -1;
		} else if ( server1->punkbuster > server2->punkbuster )     {
			res = 1;
		} else {
			res = 0;
		}
	}

	if ( sortDir ) {
		if ( res < 0 ) {
			return 1;
		}
		if ( res > 0 ) {
			return -1;
		}
		return 0;
	}
	return res;
}

/*
====================
LAN_GetPingQueueCount
====================
*/
static int LAN_GetPingQueueCount( void ) {
	return ( CL_GetPingQueueCount() );
}

/*
====================
LAN_ClearPing
====================
*/
static void LAN_ClearPing( int n ) {
	CL_ClearPing( n );
}

/*
====================
LAN_GetPing
====================
*/
static void LAN_GetPing( int n, char *buf, int buflen, int *pingtime ) {
	CL_GetPing( n, buf, buflen, pingtime );
}

/*
====================
LAN_GetPingInfo
====================
*/
static void LAN_GetPingInfo( int n, char *buf, int buflen ) {
	CL_GetPingInfo( n, buf, buflen );
}

/*
====================
LAN_MarkServerVisible
====================
*/
static void LAN_MarkServerVisible( int source, int n, qboolean visible ) {
	if ( n == -1 ) {
		int count = MAX_OTHER_SERVERS;
		serverInfo_t *server = NULL;
		switch ( source ) {
		case AS_LOCAL:
			server = &cls.localServers[0];
			break;
		case AS_MPLAYER:
			server = &cls.mplayerServers[0];
			break;
		case AS_GLOBAL:
			server = &cls.globalServers[0];
			count = MAX_GLOBAL_SERVERS;
			break;
		case AS_FAVORITES:
			server = &cls.favoriteServers[0];
			break;
		}
		if ( server ) {
			for ( n = 0; n < count; n++ ) {
				server[n].visible = visible;
			}
		}

	} else {
		switch ( source ) {
		case AS_LOCAL:
			if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
				cls.localServers[n].visible = visible;
			}
			break;
		case AS_MPLAYER:
			if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
				cls.mplayerServers[n].visible = visible;
			}
			break;
		case AS_GLOBAL:
			if ( n >= 0 && n < MAX_GLOBAL_SERVERS ) {
				cls.globalServers[n].visible = visible;
			}
			break;
		case AS_FAVORITES:
			if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
				cls.favoriteServers[n].visible = visible;
			}
			break;
		}
	}
}


/*
=======================
LAN_ServerIsVisible
=======================
*/
static int LAN_ServerIsVisible( int source, int n ) {
	switch ( source ) {
	case AS_LOCAL:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			return cls.localServers[n].visible;
		}
		break;
	case AS_MPLAYER:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			return cls.mplayerServers[n].visible;
		}
		break;
	case AS_GLOBAL:
		if ( n >= 0 && n < MAX_GLOBAL_SERVERS ) {
			return cls.globalServers[n].visible;
		}
		break;
	case AS_FAVORITES:
		if ( n >= 0 && n < MAX_OTHER_SERVERS ) {
			return cls.favoriteServers[n].visible;
		}
		break;
	}
	return qfalse;
}

/*
=======================
LAN_UpdateVisiblePings
=======================
*/
qboolean LAN_UpdateVisiblePings( int source ) {
	return CL_UpdateVisiblePings_f( source );
}

/*
====================
LAN_GetServerStatus
====================
*/
int LAN_GetServerStatus( char *serverAddress, char *serverStatus, int maxLen ) {
	return CL_ServerStatus( serverAddress, serverStatus, maxLen );
}

/*
====================
CL_GetGlConfig
====================
*/
static void CL_GetGlconfig( glconfig_t *config ) {
	*config = cls.glconfig;
}

/*
====================
GetClipboardData
====================
*/
static void GetClipboardData( char *buf, int buflen ) {
	char    *cbd;

	cbd = Sys_GetClipboardData();

	if ( !cbd ) {
		*buf = 0;
		return;
	}

	Q_strncpyz( buf, cbd, buflen );

	Z_Free( cbd );
}

/*
====================
Key_KeynumToStringBuf
====================
*/
void Key_KeynumToStringBuf( int keynum, char *buf, int buflen ) {
	Q_strncpyz( buf, Key_KeynumToString( keynum, qtrue ), buflen );
}

/*
====================
Key_GetBindingBuf
====================
*/
void Key_GetBindingBuf( int keynum, char *buf, int buflen ) {
	char    *value;

	value = Key_GetBinding( keynum );
	if ( value ) {
		Q_strncpyz( buf, value, buflen );
	} else {
		*buf = 0;
	}
}

/*
====================
Key_GetCatcher
====================
*/
int Key_GetCatcher( void ) {
	return cls.keyCatchers;
}

/*
====================
Ket_SetCatcher
====================
*/
void Key_SetCatcher( int catcher ) {
	// NERVE - SMF - console overrides everything
	if ( cls.keyCatchers & KEYCATCH_CONSOLE ) {
		cls.keyCatchers = catcher | KEYCATCH_CONSOLE;
	} else {
		cls.keyCatchers = catcher;
	}

}


/*
====================
CLUI_GetCDKey
====================
*/
static void CLUI_GetCDKey( char *buf, int buflen ) {
	cvar_t  *fs;
	fs = Cvar_Get( "fs_game", "", CVAR_INIT | CVAR_SYSTEMINFO );
	if ( UI_usesUniqueCDKey() && fs && fs->string[0] != 0 ) {
		memcpy( buf, &cl_cdkey[16], 16 );
		buf[16] = 0;
	} else {
		memcpy( buf, cl_cdkey, 16 );
		buf[16] = 0;
	}
}


/*
====================
CLUI_SetCDKey
====================
*/
static void CLUI_SetCDKey( char *buf ) {
	cvar_t  *fs;
	fs = Cvar_Get( "fs_game", "", CVAR_INIT | CVAR_SYSTEMINFO );
	if ( UI_usesUniqueCDKey() && fs && fs->string[0] != 0 ) {
		memcpy( &cl_cdkey[16], buf, 16 );
		cl_cdkey[32] = 0;
		// set the flag so the fle will be written at the next opportunity
		cvar_modifiedFlags |= CVAR_ARCHIVE;
	} else {
		memcpy( cl_cdkey, buf, 16 );
		// set the flag so the fle will be written at the next opportunity
		cvar_modifiedFlags |= CVAR_ARCHIVE;
	}
}


/*
====================
GetConfigString
====================
*/
static int GetConfigString( int index, char *buf, int size ) {
	int offset;

	if ( index < 0 || index >= MAX_CONFIGSTRINGS ) {
		return qfalse;
	}

	offset = cl.gameState.stringOffsets[index];
	if ( !offset ) {
		if ( size ) {
			buf[0] = 0;
		}
		return qfalse;
	}

	Q_strncpyz( buf, cl.gameState.stringData + offset, size );

	return qtrue;
}

void UI_InitImports(uiImport_t* import)
{
	// --- UI_ERROR/UI_PRINT ---
	import->Com_Error                       = Com_Error;
	import->Com_Printf                      = Com_Printf;

	// --- UI_MILLISECONDS ---
	import->Sys_Milliseconds                = Sys_Milliseconds;

	// --- Cvar-related ---
	import->Cvar_Register                   = Cvar_Register;
	import->Cvar_Update                     = Cvar_Update;
	import->Cvar_Set                        = Cvar_Set;
	import->Cvar_VariableValue              = Cvar_VariableValue;
	import->Cvar_VariableStringBuffer       = Cvar_VariableStringBuffer;
	import->Cvar_SetValue                   = Cvar_SetValue;
	import->Cvar_Reset                      = Cvar_Reset;
	import->Cvar_Get                        = Cvar_Get;
	import->Cvar_InfoStringBuffer           = Cvar_InfoStringBuffer;

	// --- Cmd / console-related ---
	import->Cmd_Argc                        = Cmd_Argc;
	import->Cmd_ArgvBuffer                  = Cmd_ArgvBuffer;
	import->Cbuf_ExecuteText                = Cbuf_ExecuteText;

	// --- File system ---
	import->FS_FOpenFileByMode              = FS_FOpenFileByMode;
	import->FS_Read                         = FS_Read;
	import->FS_Write                        = FS_Write;
	import->FS_FCloseFile                   = FS_FCloseFile;
	import->FS_Delete                       = FS_Delete;
	import->FS_GetFileList                  = FS_GetFileList;

	// --- Renderer hooks ---
	import->R_RegisterModel                 = re.RegisterModel;
	import->R_RegisterSkin                  = re.RegisterSkin;
	import->R_RegisterShaderNoMip           = re.RegisterShaderNoMip;
	import->R_ClearScene                    = re.ClearScene;
	import->R_AddRefEntityToScene           = re.AddRefEntityToScene;
	import->R_AddPolyToScene                = re.AddPolyToScene;
	import->R_AddPolysToScene               = re.AddPolysToScene;
	import->R_AddLightToScene               = re.AddLightToScene;
	import->R_AddCoronaToScene              = re.AddCoronaToScene;
	import->R_RenderScene                   = re.RenderScene;
	import->R_SetColor                      = re.SetColor;
	import->R_DrawStretchPic                = re.DrawStretchPic;
	import->R_ModelBounds                   = re.ModelBounds;

	// --- UI-specific calls ---
	import->SCR_UpdateScreen                = SCR_UpdateScreen;
	import->R_LerpTag                       = re.LerpTag;   // or your actual function signature

	// --- Sound ---
	#ifdef DOOMSOUND
	import->S_RegisterSound                 = S_RegisterSound;
	#else
		// S_RegisterSound(..., qfalse)
	import->S_RegisterSound                 = S_RegisterSound;
	#endif
	import->S_StartLocalSound               = S_StartLocalSound;
	import->S_StopBackgroundTrack           = S_StopBackgroundTrack;
	import->S_StartBackgroundTrack          = S_StartBackgroundTrack;

	// --- Keyboard ---
	import->Key_KeynumToStringBuf           = Key_KeynumToStringBuf;
	import->Key_GetBindingBuf               = Key_GetBindingBuf;
	import->Key_SetBinding                  = Key_SetBinding;
	import->Key_IsDown                      = Key_IsDown;
	import->Key_GetOverstrikeMode           = Key_GetOverstrikeMode;
	import->Key_SetOverstrikeMode           = Key_SetOverstrikeMode;
	import->Key_ClearStates                 = Key_ClearStates;
	import->Key_GetCatcher                  = Key_GetCatcher;
	import->Key_SetCatcher                  = Key_SetCatcher;

	// --- Clipboard, client, configstring ---
	import->GetClipboardData               = GetClipboardData;
	import->GetClientState                 = GetClientState;
	import->CL_GetGlconfig                 = CL_GetGlconfig;
	import->GetConfigString                = GetConfigString;

	// --- Server browser / LAN ---
	import->LAN_LoadCachedServers           = LAN_LoadCachedServers;
	import->LAN_SaveServersToCache          = LAN_SaveServersToCache;
	import->LAN_AddServer                   = LAN_AddServer;
	import->LAN_RemoveServer                = LAN_RemoveServer;
	import->LAN_GetPingQueueCount           = LAN_GetPingQueueCount;
	import->LAN_ClearPing                   = LAN_ClearPing;
	import->LAN_GetPing                     = LAN_GetPing;
	import->LAN_GetPingInfo                 = LAN_GetPingInfo;
	import->LAN_GetServerCount              = LAN_GetServerCount;
	import->LAN_GetServerAddressString      = LAN_GetServerAddressString;
	import->LAN_GetServerInfo               = LAN_GetServerInfo;
	import->LAN_GetServerPing               = LAN_GetServerPing;
	import->LAN_MarkServerVisible           = LAN_MarkServerVisible;
	import->LAN_ServerIsVisible             = LAN_ServerIsVisible;
	import->LAN_UpdateVisiblePings          = LAN_UpdateVisiblePings;
	import->LAN_ResetPings                  = LAN_ResetPings;
	import->LAN_GetServerStatus             = LAN_GetServerStatus;
	import->LAN_CompareServers              = LAN_CompareServers;

	// --- Hunk / memory usage ---
	import->Hunk_MemoryRemaining            = Hunk_MemoryRemaining;

	// --- CD Key ---
	import->CLUI_GetCDKey                   = CLUI_GetCDKey;
	import->CLUI_SetCDKey                   = CLUI_SetCDKey;
	import->CL_CDKeyValidate                = CL_CDKeyValidate;

	// --- Font ---
	import->R_RegisterFont                  = re.RegisterFont;

	// --- Basic C-library helpers (memcpy, memset, etc.) ---
	import->_Memset                         = memset;
	import->_Memcpy                         = memcpy;
	import->_Strncpy                        = strncpy;

	// --- Math functions ---
	import->_Sin                            = sinf;
	import->_Cos                            = cosf;
	import->_Atan2                          = atan2f;
	import->_Sqrt                           = sqrtf;
	import->_Floor                          = floorf;
	import->_Ceil                           = ceilf;

	// --- Botlib parser calls ---
	import->PC_AddGlobalDefine              = botlib_export->PC_AddGlobalDefine;
	import->PC_LoadSourceHandle             = botlib_export->PC_LoadSourceHandle;
	import->PC_FreeSourceHandle             = botlib_export->PC_FreeSourceHandle;
	import->PC_ReadTokenHandle              = botlib_export->PC_ReadTokenHandle;
	import->PC_SourceFileAndLine            = botlib_export->PC_SourceFileAndLine;

	// --- Real time ---
	import->Com_RealTime                    = Com_RealTime;

	// --- Cinematics ---
	import->CIN_PlayCinematic               = CIN_PlayCinematic;
	import->CIN_StopCinematic               = CIN_StopCinematic;
	import->CIN_RunCinematic                = CIN_RunCinematic;
	import->CIN_DrawCinematic               = CIN_DrawCinematic;
	import->CIN_SetExtents                  = CIN_SetExtents;

	// --- Remap shader ---
	import->R_RemapShader                   = re.RemapShader;

	// --- Misc UI calls (limbo strings, translation, autoupdate, open URL) ---
	import->CL_GetLimboString               = CL_GetLimboString;
	import->CL_TranslateString              = CL_TranslateString;
	import->CL_CheckAutoUpdate              = CL_CheckAutoUpdate;
	import->CL_GetAutoUpdate                = CL_GetAutoUpdate;
	import->CL_OpenURL                      = CL_OpenURL;

}

/*
====================
CL_ShutdownUI
====================
*/
void CL_ShutdownUI( void ) {
	cls.keyCatchers &= ~KEYCATCH_UI;
	cls.uiStarted = qfalse;
	if (!uidll) {
		return;
	}
	if (uivm) {
		uivm->Shutdown();
	}
	delete uidll;
	uivm = NULL;
	uidll = nullptr;
}

/*
====================
CL_InitUI
====================
*/

void CL_InitUI( void ) {
	int v;
	static uiImport_t imports;

	uidll = new DllLoader("ui");
	if (!uidll->IsLoaded())
	{
		delete uidll;
		uidll = nullptr;
		uivm = nullptr;
		Com_Error(ERR_DROP, "VM_Create on UI failed");
		return;
	}

	// Init the engine exports.
	UI_InitImports(&imports);
	uivm = uidll->GetDllEntry<uiExport_t, uiImport_t>(&imports);
	if (!uivm)
	{
		delete uidll;
		uidll = nullptr;
		uivm = nullptr;
		Com_Error(ERR_DROP, "VM_Create on UI failed, uivm is invalid");
		return;
	}

	// init for this gamestate
	uivm->Init((qboolean)(cls.state >= CA_AUTHORIZING && cls.state < CA_ACTIVE ) );
}


qboolean UI_usesUniqueCDKey() {
	return qtrue; 
}

qboolean UI_checkKeyExec( int key ) {
	if ( uivm ) {
		return uivm->CheckExecKey(key);
	} else {
		return qfalse;
	}
}

/*
====================
UI_GameCommand

See if the current console command is claimed by the ui
====================
*/
qboolean UI_GameCommand( void ) {
	if ( !uivm ) {
		return qfalse;
	}

	return uivm->ConsoleCommand(cls.realtime );
}

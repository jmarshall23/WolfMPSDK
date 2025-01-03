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

// cl_cgame.c  -- client system interaction with client game

#include "client.h"

#include "../game/botlib.h"

cGameExport_t* cgvm;
static DllLoader* cgdll = nullptr;
extern botlib_export_t *botlib_export;

/*
====================
CL_GetGameState
====================
*/
void CL_GetGameState( gameState_t *gs ) {
	*gs = cl.gameState;
}

/*
====================
CL_GetGlconfig
====================
*/
void CL_GetGlconfig( glconfig_t *glconfig ) {
	*glconfig = cls.glconfig;
}


/*
====================
CL_GetUserCmd
====================
*/
qboolean CL_GetUserCmd( int cmdNumber, usercmd_t *ucmd ) {
	// cmds[cmdNumber] is the last properly generated command

	// can't return anything that we haven't created yet
	if ( cmdNumber > cl.cmdNumber ) {
		Com_Error( ERR_DROP, "CL_GetUserCmd: %i >= %i", cmdNumber, cl.cmdNumber );
	}

	// the usercmd has been overwritten in the wrapping
	// buffer because it is too far out of date
	if ( cmdNumber <= cl.cmdNumber - CMD_BACKUP ) {
		return qfalse;
	}

	*ucmd = cl.cmds[ cmdNumber & CMD_MASK ];

	return qtrue;
}

int CL_GetCurrentCmdNumber( void ) {
	return cl.cmdNumber;
}


/*
====================
CL_GetParseEntityState
====================
*/
qboolean    CL_GetParseEntityState( int parseEntityNumber, entityState_t *state ) {
	// can't return anything that hasn't been parsed yet
	if ( parseEntityNumber >= cl.parseEntitiesNum ) {
		Com_Error( ERR_DROP, "CL_GetParseEntityState: %i >= %i",
				   parseEntityNumber, cl.parseEntitiesNum );
	}

	// can't return anything that has been overwritten in the circular buffer
	if ( parseEntityNumber <= cl.parseEntitiesNum - MAX_PARSE_ENTITIES ) {
		return qfalse;
	}

	*state = cl.parseEntities[ parseEntityNumber & ( MAX_PARSE_ENTITIES - 1 ) ];
	return qtrue;
}

/*
====================
CL_GetCurrentSnapshotNumber
====================
*/
void    CL_GetCurrentSnapshotNumber( int *snapshotNumber, int *serverTime ) {
	*snapshotNumber = cl.snap.messageNum;
	*serverTime = cl.snap.serverTime;
}

/*
====================
CL_GetSnapshot
====================
*/
qboolean    CL_GetSnapshot( int snapshotNumber, snapshot_t *snapshot ) {
	clSnapshot_t    *clSnap;
	int i, count;

	if ( snapshotNumber > cl.snap.messageNum ) {
		Com_Error( ERR_DROP, "CL_GetSnapshot: snapshotNumber > cl.snapshot.messageNum" );
	}

	// if the frame has fallen out of the circular buffer, we can't return it
	if ( cl.snap.messageNum - snapshotNumber >= PACKET_BACKUP ) {
		return qfalse;
	}

	// if the frame is not valid, we can't return it
	clSnap = &cl.snapshots[snapshotNumber & PACKET_MASK];
	if ( !clSnap->valid ) {
		return qfalse;
	}

	// if the entities in the frame have fallen out of their
	// circular buffer, we can't return it
	if ( cl.parseEntitiesNum - clSnap->parseEntitiesNum >= MAX_PARSE_ENTITIES ) {
		return qfalse;
	}

	// write the snapshot
	snapshot->snapFlags = clSnap->snapFlags;
	snapshot->serverCommandSequence = clSnap->serverCommandNum;
	snapshot->ping = clSnap->ping;
	snapshot->serverTime = clSnap->serverTime;
	memcpy( snapshot->areamask, clSnap->areamask, sizeof( snapshot->areamask ) );
	snapshot->ps = clSnap->ps;
	count = clSnap->numEntities;
	if ( count > MAX_ENTITIES_IN_SNAPSHOT ) {
		Com_DPrintf( "CL_GetSnapshot: truncated %i entities to %i\n", count, MAX_ENTITIES_IN_SNAPSHOT );
		count = MAX_ENTITIES_IN_SNAPSHOT;
	}
	snapshot->numEntities = count;
	for ( i = 0 ; i < count ; i++ ) {
		snapshot->entities[i] =
			cl.parseEntities[ ( clSnap->parseEntitiesNum + i ) & ( MAX_PARSE_ENTITIES - 1 ) ];
	}

	// FIXME: configstring changes and server commands!!!

	return qtrue;
}

/*
==============
CL_SetUserCmdValue
==============
*/
void CL_SetUserCmdValue( int userCmdValue, int holdableValue, float sensitivityScale, int mpSetup, int mpIdentClient ) {
	cl.cgameUserCmdValue        = userCmdValue;
	cl.cgameUserHoldableValue   = holdableValue;
	cl.cgameSensitivity         = sensitivityScale;
	cl.cgameMpSetup             = mpSetup;              // NERVE - SMF
	cl.cgameMpIdentClient       = mpIdentClient;        // NERVE - SMF
}

/*
==================
CL_SetClientLerpOrigin
==================
*/
void CL_SetClientLerpOrigin( float x, float y, float z ) {
	cl.cgameClientLerpOrigin[0] = x;
	cl.cgameClientLerpOrigin[1] = y;
	cl.cgameClientLerpOrigin[2] = z;
}

/*
==============
CL_AddCgameCommand
==============
*/
void CL_AddCgameCommand( const char *cmdName ) {
	Cmd_AddCommand( cmdName, NULL );
}

/*
==============
CL_CgameError
==============
*/
void CL_CgameError( const char *string ) {
	Com_Error( ERR_DROP, "%s", string );
}


/*
=====================
CL_ConfigstringModified
=====================
*/
void CL_ConfigstringModified( void ) {
	char        *old, *s;
	int i, index;
	char        *dup;
	gameState_t oldGs;
	int len;

	index = atoi( Cmd_Argv( 1 ) );
	if ( index < 0 || index >= MAX_CONFIGSTRINGS ) {
		Com_Error( ERR_DROP, "configstring > MAX_CONFIGSTRINGS" );
	}
//	s = Cmd_Argv(2);
	// get everything after "cs <num>"
	s = Cmd_ArgsFrom( 2 );

	old = cl.gameState.stringData + cl.gameState.stringOffsets[ index ];
	if ( !strcmp( old, s ) ) {
		return;     // unchanged
	}

	// build the new gameState_t
	oldGs = cl.gameState;

	memset( &cl.gameState, 0, sizeof( cl.gameState ) );

	// leave the first 0 for uninitialized strings
	cl.gameState.dataCount = 1;

	for ( i = 0 ; i < MAX_CONFIGSTRINGS ; i++ ) {
		if ( i == index ) {
			dup = s;
		} else {
			dup = oldGs.stringData + oldGs.stringOffsets[ i ];
		}
		if ( !dup[0] ) {
			continue;       // leave with the default empty string
		}

		len = strlen( dup );

		if ( len + 1 + cl.gameState.dataCount > MAX_GAMESTATE_CHARS ) {
			Com_Error( ERR_DROP, "MAX_GAMESTATE_CHARS exceeded" );
		}

		// append it to the gameState string buffer
		cl.gameState.stringOffsets[ i ] = cl.gameState.dataCount;
		memcpy( cl.gameState.stringData + cl.gameState.dataCount, dup, len + 1 );
		cl.gameState.dataCount += len + 1;
	}

	if ( index == CS_SYSTEMINFO ) {
		// parse serverId and other cvars
		CL_SystemInfoChanged();
	}

}


/*
===================
CL_GetServerCommand

Set up argc/argv for the given command
===================
*/
qboolean CL_GetServerCommand( int serverCommandNumber ) {
	char    *s;
	char    *cmd;
	static char bigConfigString[BIG_INFO_STRING];
	int argc;

	// if we have irretrievably lost a reliable command, drop the connection
	if ( serverCommandNumber <= clc.serverCommandSequence - MAX_RELIABLE_COMMANDS ) {
		// when a demo record was started after the client got a whole bunch of
		// reliable commands then the client never got those first reliable commands
		if ( clc.demoplaying ) {
			return qfalse;
		}
		Com_Error( ERR_DROP, "CL_GetServerCommand: a reliable command was cycled out" );
		return qfalse;
	}

	if ( serverCommandNumber > clc.serverCommandSequence ) {
		Com_Error( ERR_DROP, "CL_GetServerCommand: requested a command not received" );
		return qfalse;
	}

	s = clc.serverCommands[ serverCommandNumber & ( MAX_RELIABLE_COMMANDS - 1 ) ];
	clc.lastExecutedServerCommand = serverCommandNumber;

	if ( cl_showServerCommands->integer ) {         // NERVE - SMF
		Com_DPrintf( "serverCommand: %i : %s\n", serverCommandNumber, s );
	}

rescan:
	Cmd_TokenizeString( s );
	cmd = Cmd_Argv( 0 );
	argc = Cmd_Argc();

	if ( !strcmp( cmd, "disconnect" ) ) {
		// NERVE - SMF - allow server to indicate why they were disconnected
		if ( argc >= 2 ) {
			Com_Error( ERR_SERVERDISCONNECT, va( "Server Disconnected - %s", Cmd_Argv( 1 ) ) );
		} else {
			Com_Error( ERR_SERVERDISCONNECT,"Server disconnected\n" );
		}
	}

	if ( !strcmp( cmd, "bcs0" ) ) {
		Com_sprintf( bigConfigString, BIG_INFO_STRING, "cs %s \"%s", Cmd_Argv( 1 ), Cmd_Argv( 2 ) );
		return qfalse;
	}

	if ( !strcmp( cmd, "bcs1" ) ) {
		s = Cmd_Argv( 2 );
		if ( strlen( bigConfigString ) + strlen( s ) >= BIG_INFO_STRING ) {
			Com_Error( ERR_DROP, "bcs exceeded BIG_INFO_STRING" );
		}
		strcat( bigConfigString, s );
		return qfalse;
	}

	if ( !strcmp( cmd, "bcs2" ) ) {
		s = Cmd_Argv( 2 );
		if ( strlen( bigConfigString ) + strlen( s ) + 1 >= BIG_INFO_STRING ) {
			Com_Error( ERR_DROP, "bcs exceeded BIG_INFO_STRING" );
		}
		strcat( bigConfigString, s );
		strcat( bigConfigString, "\"" );
		s = bigConfigString;
		goto rescan;
	}

	if ( !strcmp( cmd, "cs" ) ) {
		CL_ConfigstringModified();
		// reparse the string, because CL_ConfigstringModified may have done another Cmd_TokenizeString()
		Cmd_TokenizeString( s );
		return qtrue;
	}

	if ( !strcmp( cmd, "map_restart" ) ) {
		// clear notify lines and outgoing commands before passing
		// the restart to the cgame
		Con_ClearNotify();
		memset( cl.cmds, 0, sizeof( cl.cmds ) );
		return qtrue;
	}

	if ( !strcmp( cmd, "popup" ) ) { // direct server to client popup request, bypassing cgame
//		trap_UI_Popup(Cmd_Argv(1));
//		if ( cls.state == CA_ACTIVE && !clc.demoplaying ) {
//			uivm->SetActiveMenu( UIMENU_CLIPBOARD);
//			Menus_OpenByName(Cmd_Argv(1));
//		}
		return qfalse;
	}


	// the clientLevelShot command is used during development
	// to generate 128*128 screenshots from the intermission
	// point of levels for the menu system to use
	// we pass it along to the cgame to make apropriate adjustments,
	// but we also clear the console and notify lines here
	if ( !strcmp( cmd, "clientLevelShot" ) ) {
		// don't do it if we aren't running the server locally,
		// otherwise malicious remote servers could overwrite
		// the existing thumbnails
		if ( !com_sv_running->integer ) {
			return qfalse;
		}
		// close the console
		Con_Close();
		// take a special screenshot next frame
		Cbuf_AddText( "wait ; wait ; wait ; wait ; screenshot levelshot\n" );
		return qtrue;
	}

	// we may want to put a "connect to other server" command here

	// cgame can now act on the command
	return qtrue;
}

// DHM - Nerve :: Copied from server to here
/*
====================
CL_SetExpectedHunkUsage

  Sets com_expectedhunkusage, so the client knows how to draw the percentage bar
====================
*/
void CL_SetExpectedHunkUsage( const char *mapname ) {
	int handle;
	char *memlistfile = "hunkusage.dat";
	char *buf;
	char *buftrav;
	char *token;
	int len;

	len = FS_FOpenFileByMode( memlistfile, &handle, FS_READ );
	if ( len >= 0 ) { // the file exists, so read it in, strip out the current entry for this map, and save it out, so we can append the new value

		buf = (char *)Z_Malloc( len + 1 );
		memset( buf, 0, len + 1 );

		FS_Read( (void *)buf, len, handle );
		FS_FCloseFile( handle );

		// now parse the file, filtering out the current map
		buftrav = buf;
		while ( ( token = COM_Parse( &buftrav ) ) && token[0] ) {
			if ( !Q_strcasecmp( token, (char *)mapname ) ) {
				// found a match
				token = COM_Parse( &buftrav );  // read the size
				if ( token && token[0] ) {
					// this is the usage
					Cvar_Set( "com_expectedhunkusage", token );
					Z_Free( buf );
					return;
				}
			}
		}

		Z_Free( buf );
	}
	// just set it to a negative number,so the cgame knows not to draw the percent bar
	Cvar_Set( "com_expectedhunkusage", "-1" );
}

// dhm - nerve

/*
====================
CL_CM_LoadMap

Just adds default parameters that cgame doesn't need to know about
====================
*/
void CL_CM_LoadMap( const char *mapname ) {
	int checksum;

	// DHM - Nerve :: If we are not running the server, then set expected usage here
	if ( !com_sv_running->integer ) {
		CL_SetExpectedHunkUsage( mapname );
	} else
	{
		// TTimo
		// catch here when a local server is started to avoid outdated com_errorDiagnoseIP
		Cvar_Set( "com_errorDiagnoseIP", "" );
	}

	CM_LoadMap( mapname, qtrue, &checksum );
}

/*
====================
CL_ShutdonwCGame

====================
*/
void CL_ShutdownCGame( void ) {
	cls.keyCatchers &= ~KEYCATCH_CGAME;
	cls.cgameStarted = qfalse;
	if (!cgdll) {
		return;
	}
	if ( cgvm ) {
		cgvm->Shutdown();
	}
	delete cgdll;
	cgvm = NULL;
	cgdll = nullptr;
}

/**
 * Shows a specific UI menu if the client state is active and not demo-playing.
 *
 * @param menuName  The string name of the menu to show.
 */
void CG_ShowUIMenu(const char* menuName) {
	// Avoid null strings
	if (!menuName) {
		return;
	}

	// Check which menuName we received and call into the UI VM accordingly
	if (!Q_stricmp(menuName, "UIMENU_WM_PICKTEAM")) {
		uivm->SetActiveMenu(UIMENU_WM_PICKTEAM);
	}
	else if (!Q_stricmp(menuName, "UIMENU_WM_PICKPLAYER")) {
		uivm->SetActiveMenu(UIMENU_WM_PICKPLAYER);
	}
	else if (!Q_stricmp(menuName, "UIMENU_WM_QUICKMESSAGE")) {
		uivm->SetActiveMenu(UIMENU_WM_QUICKMESSAGE);
	}
	else if (!Q_stricmp(menuName, "UIMENU_WM_QUICKMESSAGEALT")) {
		uivm->SetActiveMenu(UIMENU_WM_QUICKMESSAGEALT);
	}
	else if (!Q_stricmp(menuName, "UIMENU_WM_LIMBO")) {
		uivm->SetActiveMenu(UIMENU_WM_LIMBO);
	}
	else if (!Q_stricmp(menuName, "UIMENU_WM_AUTOUPDATE")) {
		uivm->SetActiveMenu(UIMENU_WM_AUTOUPDATE);
	}
	// hbook menus
	else if (!Q_stricmp(menuName, "hbook1")) {
		uivm->SetActiveMenu(UIMENU_BOOK1);
	}
	else if (!Q_stricmp(menuName, "hbook2")) {
		uivm->SetActiveMenu(UIMENU_BOOK2);
	}
	else if (!Q_stricmp(menuName, "hbook3")) {
		uivm->SetActiveMenu(UIMENU_BOOK3);
	}
	// Default to clipboard if none of the above matched
	else {
		uivm->SetActiveMenu(UIMENU_CLIPBOARD);
	}
}

/**
 * Closes the limbo menu if the user requested UIMENU_WM_LIMBO
 * and the current active menu is already limbo.
 *
 * @param uivm     Pointer to the UI VM.
 * @param menuName Name of the menu the user wants to open.
 */
void CG_CloseLimboIfActive(const char* menuName) {
	// Validate the menuName
	if (!menuName) {
		return;
	}

	// Check if the requested menu is limbo
	if (!Q_stricmp(menuName, "UIMENU_WM_LIMBO")) {
		// If we're already on limbo, simulate pressing ESC twice to close it
		if (uivm->GetActiveMenu() == UIMENU_WM_LIMBO) {			
			uivm->KeyEvent(K_ESCAPE, qtrue);
			uivm->KeyEvent(K_ESCAPE, qtrue);
		}
	}
}

/*
=================
CG_InitExport

Assigns function pointers into the passed-in cgameImport_t struct.
Call this before loading or initializing your cgame module so the cgame
has access to all engine-level functionality.
=================
*/
void CG_InitExport(cgameImport_t* import) {
	//
	// Print / Error / Timing
	//
	import->Com_Printf        = Com_Printf;
	import->Com_Error         = Com_Error;
	import->Sys_Milliseconds  = Sys_Milliseconds;

	//
	// Cvars
	//
	import->Cvar_Register               = Cvar_Register;
	import->Cvar_Update                 = Cvar_Update;
	import->Cvar_Set                    = Cvar_Set;
	import->Cvar_VariableStringBuffer   = Cvar_VariableStringBuffer;

	//
	// Command system
	//
	import->Cmd_Argc        = Cmd_Argc;
	import->Cmd_ArgvBuffer  = Cmd_ArgvBuffer;
	import->Cmd_ArgsBuffer  = Cmd_ArgsBuffer;

	//
	// File system
	//
	import->FS_FOpenFileByMode  = FS_FOpenFileByMode;
	import->FS_Read            = FS_Read;
	import->FS_Write           = FS_Write;
	import->FS_FCloseFile      = FS_FCloseFile;

	//
	// Console command buffers
	//
	import->Cbuf_AddText = Cbuf_AddText;

	//
	// Add / remove console commands
	//
	import->CL_AddCgameCommand = CL_AddCgameCommand;
	import->Cmd_RemoveCommand  = Cmd_RemoveCommand;

	//
	// Client commands
	//
	import->CL_AddReliableCommand = CL_AddReliableCommand;

	//
	// Screen
	//
	import->SCR_UpdateScreen = SCR_UpdateScreen;

	//
	// Collision Map
	//
	import->CL_CM_LoadMap                  = CL_CM_LoadMap;
	import->CM_NumInlineModels             = CM_NumInlineModels;
	import->CM_InlineModel                 = CM_InlineModel;
	import->CM_TempBoxModel                = CM_TempBoxModel;
	import->CM_PointContents               = CM_PointContents;
	import->CM_TransformedPointContents    = CM_TransformedPointContents;
	import->CM_BoxTrace                    = CM_BoxTrace;
	import->CM_TransformedBoxTrace         = CM_TransformedBoxTrace;

	//
	// Renderer: mark fragments
	//
	import->re_MarkFragments = re.MarkFragments;

	//
	// Sound
	//
	import->S_StartSound           = S_StartSound;
	import->S_StartSoundEx         = S_StartSoundEx;
	import->S_StartLocalSound      = S_StartLocalSound;
	import->S_ClearLoopingSounds   = S_ClearLoopingSounds;
	import->S_AddLoopingSound      = S_AddLoopingSound;
	import->S_UpdateEntityPosition = S_UpdateEntityPosition;
	import->S_GetVoiceAmplitude    = S_GetVoiceAmplitude;
	import->S_Respatialize         = S_Respatialize;
	import->S_RegisterSound        = S_RegisterSound;
	import->S_StartBackgroundTrack = S_StartBackgroundTrack;
	import->S_StartStreamingSound  = S_StartStreamingSound;
	import->S_StopBackgroundTrack  = S_StopBackgroundTrack;

	//
	// Renderer: model / shader registration
	//
	import->re_LoadWorld            = re.LoadWorld;
	import->re_RegisterModel        = re.RegisterModel;
	import->re_RegisterSkin         = re.RegisterSkin;
	import->re_GetSkinModel         = re.GetSkinModel;
	import->re_GetShaderFromModel   = re.GetShaderFromModel;
	import->re_RegisterShader       = re.RegisterShader;
	import->re_RegisterFont         = re.RegisterFont;
	import->re_RegisterShaderNoMip  = re.RegisterShaderNoMip;

	//
	// Renderer: scene building
	//
	import->re_ClearScene            = re.ClearScene;
	import->re_AddRefEntityToScene   = re.AddRefEntityToScene;
	import->re_AddPolyToScene        = re.AddPolyToScene;
	import->re_AddPolysToScene       = re.AddPolysToScene;
	import->re_AddLightToScene       = re.AddLightToScene;
	import->re_AddCoronaToScene      = re.AddCoronaToScene;
	import->re_SetFog                = re.SetFog;
	import->re_RenderScene           = re.RenderScene;

	//
	// Renderer: 2D drawing
	//
	import->re_SetColor                   = re.SetColor;
	import->re_DrawStretchPic             = re.DrawStretchPic;
	import->re_DrawRotatedPic            = re.DrawRotatedPic;
	import->re_DrawStretchPicGradient     = re.DrawStretchPicGradient;

	//
	// Renderer: utility
	//
	import->re_ModelBounds        = re.ModelBounds;
	import->re_LerpTag            = re.LerpTag;
	import->re_GetEntityToken     = re.GetEntityToken;
	import->re_RemapShader        = re.RemapShader;

	//
	// CL_*
	//
	import->CL_GetGlconfig              = CL_GetGlconfig;
	import->CL_GetGameState             = CL_GetGameState;
	import->CL_GetCurrentSnapshotNumber = CL_GetCurrentSnapshotNumber;
	import->CL_GetSnapshot              = CL_GetSnapshot;
	import->CL_GetServerCommand         = CL_GetServerCommand;
	import->CL_GetCurrentCmdNumber      = CL_GetCurrentCmdNumber;
	import->CL_GetUserCmd               = CL_GetUserCmd;
	import->CL_SetUserCmdValue          = CL_SetUserCmdValue;
	import->CL_SetClientLerpOrigin      = CL_SetClientLerpOrigin;

	//
	// Memory / key / etc.
	//
	import->Hunk_MemoryRemaining = Hunk_MemoryRemaining;
	import->Key_IsDown           = Key_IsDown;
	import->Key_GetCatcher       = Key_GetCatcher;
	import->Key_SetCatcher       = Key_SetCatcher;
	import->Key_GetKey           = Key_GetKey;

	//
	// libc-ish
	//
	import->_Memset  = memset;
	import->_Memcpy  = memcpy;
	import->_Strncpy = strncpy;

	//
	// Math
	//
	import->_Sin   = sinf;    // or just sin
	import->_Cos   = cosf;
	import->_Atan2 = atan2f;
	import->_Sqrt  = sqrtf;
	import->_Floor = floorf;
	import->_Ceil  = ceilf;
	import->_Acos  = Q_acos;  // some engines define Q_acos, or you can just use acosf

	//
	// Botlib PC_*
	//
	import->PC_AddGlobalDefine    = botlib_export->PC_AddGlobalDefine;
	import->PC_LoadSourceHandle   = botlib_export->PC_LoadSourceHandle;
	import->PC_FreeSourceHandle   = botlib_export->PC_FreeSourceHandle;
	import->PC_ReadTokenHandle    = botlib_export->PC_ReadTokenHandle;
	import->PC_SourceFileAndLine  = botlib_export->PC_SourceFileAndLine;

	//
	// Time, Vector snapping, etc.
	//
	import->Com_RealTime   = Com_RealTime;
	import->Sys_SnapVector = Sys_SnapVector;

	//
	// Server <-> Game bridging
	//
	import->SV_SendMoveSpeedsToGame = SV_SendMoveSpeedsToGame;

	//
	// Cinematics
	//
	import->CIN_PlayCinematic  = CIN_PlayCinematic;
	import->CIN_StopCinematic  = CIN_StopCinematic;
	import->CIN_RunCinematic   = CIN_RunCinematic;
	import->CIN_DrawCinematic  = CIN_DrawCinematic;
	import->CIN_SetExtents     = CIN_SetExtents;

	//
	// Camera system
	//
	import->loadCamera       = loadCamera;
	import->startCamera      = startCamera;
	import->getCameraInfo    = getCameraInfo;

	//
	// In-game popups / UI hooking
	//
	import->InGame_Popup        = CG_ShowUIMenu;
	import->InGame_ClosePopup   = CG_CloseLimboIfActive;
	import->CL_AddToLimboChat   = CL_AddToLimboChat;

	//
	// Key and binding strings
	//
	import->Key_GetBindingBuf      = Key_GetBindingBuf;
	import->Key_SetBinding         = Key_SetBinding;
	import->Key_KeynumToStringBuf  = Key_KeynumToStringBuf;

	//
	// Translation
	//
	import->CL_TranslateString = CL_TranslateString;
}

/*
====================
CL_UpdateLevelHunkUsage

  This updates the "hunkusage.dat" file with the current map and it's hunk usage count

  This is used for level loading, so we can show a percentage bar dependant on the amount
  of hunk memory allocated so far

  This will be slightly inaccurate if some settings like sound quality are changed, but these
  things should only account for a small variation (hopefully)
====================
*/
void CL_UpdateLevelHunkUsage( void ) {
	int handle;
	char *memlistfile = "hunkusage.dat";
	char *buf, *outbuf;
	char *buftrav, *outbuftrav;
	char *token;
	char outstr[256];
	int len, memusage;

	memusage = Cvar_VariableIntegerValue( "com_hunkused" ) + Cvar_VariableIntegerValue( "hunk_soundadjust" );

	len = FS_FOpenFileByMode( memlistfile, &handle, FS_READ );
	if ( len >= 0 ) { // the file exists, so read it in, strip out the current entry for this map, and save it out, so we can append the new value

		buf = (char *)Z_Malloc( len + 1 );
		memset( buf, 0, len + 1 );
		outbuf = (char *)Z_Malloc( len + 1 );
		memset( outbuf, 0, len + 1 );

		FS_Read( (void *)buf, len, handle );
		FS_FCloseFile( handle );

		// now parse the file, filtering out the current map
		buftrav = buf;
		outbuftrav = outbuf;
		outbuftrav[0] = '\0';
		while ( ( token = COM_Parse( &buftrav ) ) && token[0] ) {
			if ( !Q_strcasecmp( token, cl.mapname ) ) {
				// found a match
				token = COM_Parse( &buftrav );  // read the size
				if ( token && token[0] ) {
					if ( atoi( token ) == memusage ) {  // if it is the same, abort this process
						Z_Free( buf );
						Z_Free( outbuf );
						return;
					}
				}
			} else {    // send it to the outbuf
				Q_strcat( outbuftrav, len + 1, token );
				Q_strcat( outbuftrav, len + 1, " " );
				token = COM_Parse( &buftrav );  // read the size
				if ( token && token[0] ) {
					Q_strcat( outbuftrav, len + 1, token );
					Q_strcat( outbuftrav, len + 1, "\n" );
				} else {
					Com_Error( ERR_DROP, "hunkusage.dat file is corrupt\n" );
				}
			}
		}

#ifdef __MACOS__    //DAJ MacOS file typing
		{
			extern _MSL_IMP_EXP_C long _fcreator, _ftype;
			_ftype = 'TEXT';
			_fcreator = 'WlfS';
		}
#endif
		handle = FS_FOpenFileWrite( memlistfile );
		if ( handle < 0 ) {
			Com_Error( ERR_DROP, "cannot create %s\n", memlistfile );
		}
		// input file is parsed, now output to the new file
		len = strlen( outbuf );
		if ( FS_Write( (void *)outbuf, len, handle ) != len ) {
			Com_Error( ERR_DROP, "cannot write to %s\n", memlistfile );
		}
		FS_FCloseFile( handle );

		Z_Free( buf );
		Z_Free( outbuf );
	}
	// now append the current map to the current file
	FS_FOpenFileByMode( memlistfile, &handle, FS_APPEND );
	if ( handle < 0 ) {
		Com_Error( ERR_DROP, "cannot write to hunkusage.dat, check disk full\n" );
	}
	Com_sprintf( outstr, sizeof( outstr ), "%s %i\n", cl.mapname, memusage );
	FS_Write( outstr, strlen( outstr ), handle );
	FS_FCloseFile( handle );

	// now just open it and close it, so it gets copied to the pak dir
	len = FS_FOpenFileByMode( memlistfile, &handle, FS_READ );
	if ( len >= 0 ) {
		FS_FCloseFile( handle );
	}
}

/*
====================
CL_InitCGame

Should only by called by CL_StartHunkUsers
====================
*/
void CL_InitCGame( void ) {
	const char          *info;
	const char          *mapname;
	int t1, t2;
	static cgameImport_t imports;

	t1 = Sys_Milliseconds();

	// put away the console
	Con_Close();

	// find the current mapname
	info = cl.gameState.stringData + cl.gameState.stringOffsets[ CS_SERVERINFO ];
	mapname = Info_ValueForKey( info, "mapname" );
	Com_sprintf( cl.mapname, sizeof( cl.mapname ), "maps/%s.bsp", mapname );

	// load the dll
	cgdll = new DllLoader("cgame");
	if (!cgdll->IsLoaded())
	{
		delete cgdll;
		cgdll = nullptr;
		cgvm = nullptr;
		Com_Error(ERR_DROP, "VM_Create on cgame failed");
		return;
	}

	// Init the engine exports.
	CG_InitExport(&imports);
	cgvm = cgdll->GetDllEntry<cGameExport_t, cgameImport_t>(&imports);
	if (!cgvm)
	{
		delete cgdll;
		cgvm = nullptr;
		cgdll = nullptr;
		Com_Error(ERR_DROP, "VM_Create on cgame failed, cgvm is invalid");
		return;
	}

	cls.state = CA_LOADING;

	// init for this gamestate
	// use the lastExecutedServerCommand instead of the serverCommandSequence
	// otherwise server commands sent just before a gamestate are dropped
	cgvm->Init(clc.serverMessageSequence, clc.lastExecutedServerCommand, clc.clientNum );

	// we will send a usercmd this frame, which
	// will cause the server to send us the first snapshot
	cls.state = CA_PRIMED;

	t2 = Sys_Milliseconds();

	Com_Printf( "CL_InitCGame: %5.2f seconds\n", ( t2 - t1 ) / 1000.0 );

	// have the renderer touch all its images, so they are present
	// on the card even if the driver does deferred loading
	re.EndRegistration();

	// make sure everything is paged in
	if ( !Sys_LowPhysicalMemory() ) {
		Com_TouchMemory();
	}

	// clear anything that got printed
	Con_ClearNotify();

	// Ridah, update the memory usage file
	CL_UpdateLevelHunkUsage();
}


/*
====================
CL_GameCommand

See if the current console command is claimed by the cgame
====================
*/
qboolean CL_GameCommand( void ) {
	if ( !cgvm ) {
		return qfalse;
	}

	return cgvm->ConsoleCommand();
}



/*
=====================
CL_CGameRendering
=====================
*/
void CL_CGameRendering( stereoFrame_t stereo ) {	
	cgvm->DrawActiveFrame(cl.serverTime, stereo, clc.demoplaying );
}


/*
=================
CL_AdjustTimeDelta

Adjust the clients view of server time.

We attempt to have cl.serverTime exactly equal the server's view
of time plus the timeNudge, but with variable latencies over
the internet it will often need to drift a bit to match conditions.

Our ideal time would be to have the adjusted time approach, but not pass,
the very latest snapshot.

Adjustments are only made when a new snapshot arrives with a rational
latency, which keeps the adjustment process framerate independent and
prevents massive overadjustment during times of significant packet loss
or bursted delayed packets.
=================
*/

#define RESET_TIME  500

void CL_AdjustTimeDelta( void ) {
	int resetTime;
	int newDelta;
	int deltaDelta;

	cl.newSnapshots = qfalse;

	// the delta never drifts when replaying a demo
	if ( clc.demoplaying ) {
		return;
	}

	// if the current time is WAY off, just correct to the current value
	if ( com_sv_running->integer ) {
		resetTime = 100;
	} else {
		resetTime = RESET_TIME;
	}

	newDelta = cl.snap.serverTime - cls.realtime;
	deltaDelta = abs( newDelta - cl.serverTimeDelta );

	if ( deltaDelta > RESET_TIME ) {
		cl.serverTimeDelta = newDelta;
		cl.oldServerTime = cl.snap.serverTime;  // FIXME: is this a problem for cgame?
		cl.serverTime = cl.snap.serverTime;
		if ( cl_showTimeDelta->integer ) {
			Com_Printf( "<RESET> " );
		}
	} else if ( deltaDelta > 100 ) {
		// fast adjust, cut the difference in half
		if ( cl_showTimeDelta->integer ) {
			Com_Printf( "<FAST> " );
		}
		cl.serverTimeDelta = ( cl.serverTimeDelta + newDelta ) >> 1;
	} else {
		// slow drift adjust, only move 1 or 2 msec

		// if any of the frames between this and the previous snapshot
		// had to be extrapolated, nudge our sense of time back a little
		// the granularity of +1 / -2 is too high for timescale modified frametimes
		if ( com_timescale->value == 0 || com_timescale->value == 1 ) {
			if ( cl.extrapolatedSnapshot ) {
				cl.extrapolatedSnapshot = qfalse;
				cl.serverTimeDelta -= 2;
			} else {
				// otherwise, move our sense of time forward to minimize total latency
				cl.serverTimeDelta++;
			}
		}
	}

	if ( cl_showTimeDelta->integer ) {
		Com_Printf( "%i ", cl.serverTimeDelta );
	}
}


/*
==================
CL_FirstSnapshot
==================
*/
void CL_FirstSnapshot( void ) {
	// ignore snapshots that don't have entities
	if ( cl.snap.snapFlags & SNAPFLAG_NOT_ACTIVE ) {
		return;
	}
	cls.state = CA_ACTIVE;

	// set the timedelta so we are exactly on this first frame
	cl.serverTimeDelta = cl.snap.serverTime - cls.realtime;
	cl.oldServerTime = cl.snap.serverTime;

	clc.timeDemoBaseTime = cl.snap.serverTime;

	// if this is the first frame of active play,
	// execute the contents of activeAction now
	// this is to allow scripting a timedemo to start right
	// after loading
	if ( cl_activeAction->string[0] ) {
		Cbuf_AddText( cl_activeAction->string );
		Cvar_Set( "activeAction", "" );
	}

	Sys_BeginProfiling();
}

/*
==================
CL_SetCGameTime
==================
*/
void CL_SetCGameTime( void ) {
	// getting a valid frame message ends the connection process
	if ( cls.state != CA_ACTIVE ) {
		if ( cls.state != CA_PRIMED ) {
			return;
		}
		if ( clc.demoplaying ) {
			// we shouldn't get the first snapshot on the same frame
			// as the gamestate, because it causes a bad time skip
			if ( !clc.firstDemoFrameSkipped ) {
				clc.firstDemoFrameSkipped = qtrue;
				return;
			}
			CL_ReadDemoMessage();
		}
		if ( cl.newSnapshots ) {
			cl.newSnapshots = qfalse;
			CL_FirstSnapshot();
		}
		if ( cls.state != CA_ACTIVE ) {
			return;
		}
	}

	// if we have gotten to this point, cl.snap is guaranteed to be valid
	if ( !cl.snap.valid ) {
		Com_Error( ERR_DROP, "CL_SetCGameTime: !cl.snap.valid" );
	}

	// allow pause in single player
	if ( sv_paused->integer && cl_paused->integer && com_sv_running->integer ) {
		// paused
		return;
	}

	if ( cl.snap.serverTime < cl.oldFrameServerTime ) {
		// Ridah, if this is a localhost, then we are probably loading a savegame
		if ( !Q_stricmp( cls.servername, "localhost" ) ) {
			// do nothing?
			CL_FirstSnapshot();
		} else {
			Com_Error( ERR_DROP, "cl.snap.serverTime < cl.oldFrameServerTime" );
		}
	}
	cl.oldFrameServerTime = cl.snap.serverTime;


	// get our current view of time

	if ( clc.demoplaying && cl_freezeDemo->integer ) {
		// cl_freezeDemo is used to lock a demo in place for single frame advances

	} else {
		// cl_timeNudge is a user adjustable cvar that allows more
		// or less latency to be added in the interest of better
		// smoothness or better responsiveness.
		int tn;

		tn = cl_timeNudge->integer;
		if ( tn < -30 ) {
			tn = -30;
		} else if ( tn > 30 ) {
			tn = 30;
		}

		cl.serverTime = cls.realtime + cl.serverTimeDelta - tn;

		// guarantee that time will never flow backwards, even if
		// serverTimeDelta made an adjustment or cl_timeNudge was changed
		if ( cl.serverTime < cl.oldServerTime ) {
			cl.serverTime = cl.oldServerTime;
		}
		cl.oldServerTime = cl.serverTime;

		// note if we are almost past the latest frame (without timeNudge),
		// so we will try and adjust back a bit when the next snapshot arrives
		if ( cls.realtime + cl.serverTimeDelta >= cl.snap.serverTime - 5 ) {
			cl.extrapolatedSnapshot = qtrue;
		}
	}

	// if we have gotten new snapshots, drift serverTimeDelta
	// don't do this every frame, or a period of packet loss would
	// make a huge adjustment
	if ( cl.newSnapshots ) {
		CL_AdjustTimeDelta();
	}

	if ( !clc.demoplaying ) {
		return;
	}

	// if we are playing a demo back, we can just keep reading
	// messages from the demo file until the cgame definately
	// has valid snapshots to interpolate between

	// a timedemo will always use a deterministic set of time samples
	// no matter what speed machine it is run on,
	// while a normal demo may have different time samples
	// each time it is played back
	if ( cl_timedemo->integer ) {
		if ( !clc.timeDemoStart ) {
			clc.timeDemoStart = Sys_Milliseconds();
		}
		clc.timeDemoFrames++;
		cl.serverTime = clc.timeDemoBaseTime + clc.timeDemoFrames * 50;
	}

	while ( cl.serverTime >= cl.snap.serverTime ) {
		// feed another messag, which should change
		// the contents of cl.snap
		CL_ReadDemoMessage();
		if ( cls.state != CA_ACTIVE ) {
			return;     // end of demo
		}
	}

}

/*
====================
CL_GetTag
====================
*/
qboolean CL_GetTag( int clientNum, char *tagname, orientation_t *or ) {
	if ( !cgvm ) {
		return qfalse;
	}

	return cgvm->GetTag(clientNum, tagname, or );
}

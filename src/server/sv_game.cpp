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

// sv_game.c -- interface to the game dll

#include "server.h"

static DllLoader* gamedll = nullptr;
gameExport_t* gvm = NULL;                // game virtual machine // bk001212 init
botlib_export_t *botlib_export;

void SV_GameError( const char *string ) {
	Com_Error( ERR_DROP, "%s", string );
}

void SV_GamePrint( const char *string ) {
	Com_Printf( "%s", string );
}

// these functions must be used instead of pointer arithmetic, because
// the game allocates gentities with private information after the server shared part
int SV_NumForGentity( sharedEntity_t *ent ) {
	int num;

	num = ( (byte *)ent - (byte *)sv.gentities ) / sv.gentitySize;

	return num;
}

sharedEntity_t *SV_GentityNum( int num ) {
	sharedEntity_t *ent;

	ent = ( sharedEntity_t * )( (byte *)sv.gentities + sv.gentitySize * ( num ) );

	return ent;
}

playerState_t *SV_GameClientNum( int num ) {
	playerState_t   *ps;

	ps = ( playerState_t * )( (byte *)sv.gameClients + sv.gameClientSize * ( num ) );

	return ps;
}

svEntity_t  *SV_SvEntityForGentity( sharedEntity_t *gEnt ) {
	if ( !gEnt || gEnt->s.number < 0 || gEnt->s.number >= MAX_GENTITIES ) {
		Com_Error( ERR_DROP, "SV_SvEntityForGentity: bad gEnt" );
	}
	return &sv.svEntities[ gEnt->s.number ];
}

sharedEntity_t *SV_GEntityForSvEntity( svEntity_t *svEnt ) {
	int num;

	num = svEnt - sv.svEntities;
	return SV_GentityNum( num );
}

/*
===============
SV_GameSendServerCommand

Sends a command string to a client
===============
*/
void SV_GameSendServerCommand( int clientNum, const char *text ) {
	if ( clientNum == -1 ) {
		SV_SendServerCommand( NULL, "%s", text );
	} else {
		if ( clientNum < 0 || clientNum >= sv_maxclients->integer ) {
			return;
		}
		SV_SendServerCommand( svs.clients + clientNum, "%s", text );
	}
}


/*
===============
SV_GameDropClient

Disconnects the client with a message
===============
*/
void SV_GameDropClient( int clientNum, const char *reason ) {
	if ( clientNum < 0 || clientNum >= sv_maxclients->integer ) {
		return;
	}
	SV_DropClient( svs.clients + clientNum, reason );
}


/*
=================
SV_SetBrushModel

sets mins and maxs for inline bmodels
=================
*/
void SV_SetBrushModel( sharedEntity_t *ent, const char *name ) {
	clipHandle_t h;
	vec3_t mins, maxs;

	if ( !name ) {
		Com_Error( ERR_DROP, "SV_SetBrushModel: NULL" );
	}

	if ( name[0] != '*' ) {
		Com_Error( ERR_DROP, "SV_SetBrushModel: %s isn't a brush model", name );
	}


	ent->s.modelindex = atoi( name + 1 );

	h = CM_InlineModel( ent->s.modelindex );
	CM_ModelBounds( h, mins, maxs );
	VectorCopy( mins, ent->r.mins );
	VectorCopy( maxs, ent->r.maxs );
	ent->r.bmodel = qtrue;

	ent->r.contents = -1;       // we don't know exactly what is in the brushes

	SV_LinkEntity( ent );       // FIXME: remove
}



/*
=================
SV_inPVS

Also checks portalareas so that doors block sight
=================
*/
qboolean SV_inPVS( const vec3_t p1, const vec3_t p2 ) {
	int leafnum;
	int cluster;
	int area1, area2;
	byte    *mask;

	leafnum = CM_PointLeafnum( p1 );
	cluster = CM_LeafCluster( leafnum );
	area1 = CM_LeafArea( leafnum );
	mask = CM_ClusterPVS( cluster );

	leafnum = CM_PointLeafnum( p2 );
	cluster = CM_LeafCluster( leafnum );
	area2 = CM_LeafArea( leafnum );
	if ( mask && ( !( mask[cluster >> 3] & ( 1 << ( cluster & 7 ) ) ) ) ) {
		return qfalse;
	}
	if ( !CM_AreasConnected( area1, area2 ) ) {
		return qfalse;      // a door blocks sight
	}
	return qtrue;
}


/*
=================
SV_inPVSIgnorePortals

Does NOT check portalareas
=================
*/
qboolean SV_inPVSIgnorePortals( const vec3_t p1, const vec3_t p2 ) {
	int leafnum;
	int cluster;
	int area1, area2;
	byte    *mask;

	leafnum = CM_PointLeafnum( p1 );
	cluster = CM_LeafCluster( leafnum );
	area1 = CM_LeafArea( leafnum );
	mask = CM_ClusterPVS( cluster );

	leafnum = CM_PointLeafnum( p2 );
	cluster = CM_LeafCluster( leafnum );
	area2 = CM_LeafArea( leafnum );

	if ( mask && ( !( mask[cluster >> 3] & ( 1 << ( cluster & 7 ) ) ) ) ) {
		return qfalse;
	}

	return qtrue;
}


/*
========================
SV_AdjustAreaPortalState
========================
*/
void SV_AdjustAreaPortalState( sharedEntity_t *ent, qboolean open ) {
	svEntity_t  *svEnt;

	svEnt = SV_SvEntityForGentity( ent );
	if ( svEnt->areanum2 == -1 ) {
		return;
	}
	CM_AdjustAreaPortalState( svEnt->areanum, svEnt->areanum2, open );
}


/*
==================
SV_GameAreaEntities
==================
*/
qboolean    SV_EntityContact( const vec3_t mins, const vec3_t maxs, const sharedEntity_t *gEnt, const int capsule ) {
	const float *origin, *angles;
	clipHandle_t ch;
	trace_t trace;

	// check for exact collision
	origin = gEnt->r.currentOrigin;
	angles = gEnt->r.currentAngles;

	ch = SV_ClipHandleForEntity( gEnt );
	CM_TransformedBoxTrace( &trace, vec3_origin, vec3_origin, mins, maxs,
							ch, -1, origin, angles, capsule );

	return trace.startsolid;
}


/*
===============
SV_GetServerinfo

===============
*/
void SV_GetServerinfo( char *buffer, int bufferSize ) {
	if ( bufferSize < 1 ) {
		Com_Error( ERR_DROP, "SV_GetServerinfo: bufferSize == %i", bufferSize );
	}
	Q_strncpyz( buffer, Cvar_InfoString( CVAR_SERVERINFO ), bufferSize );
}

/*
===============
SV_LocateGameData

===============
*/
void SV_LocateGameData( sharedEntity_t *gEnts, int numGEntities, int sizeofGEntity_t, playerState_t *clients, int sizeofGameClient ) {
	sv.gentities = gEnts;
	sv.gentitySize = sizeofGEntity_t;
	sv.num_entities = numGEntities;

	sv.gameClients = clients;
	sv.gameClientSize = sizeofGameClient;
}


/*
===============
SV_GetUsercmd

===============
*/
void SV_GetUsercmd( int clientNum, usercmd_t *cmd ) {
	if ( clientNum < 0 || clientNum >= sv_maxclients->integer ) {
		Com_Error( ERR_DROP, "SV_GetUsercmd: bad clientNum:%i", clientNum );
	}
	*cmd = svs.clients[clientNum].lastUsercmd;
}

/*
===============
SV_ClientThinkWithId
===============
*/
void SV_ClientThinkWithId(int clientId, usercmd_t* cmd) {
	SV_ClientThink(&svs.clients[clientId], cmd);
}

/*
============
G_GetEntityToken
============
*/
qboolean G_GetEntityToken(char *buffer, int bufferSize) {
	const char* s;

	// Parse the next token from sv.entityParsePoint
	s = COM_Parse(&sv.entityParsePoint);

	// Copy the parsed token into the destination buffer
	Q_strncpyz(buffer, s, bufferSize);

	// If sv.entityParsePoint is NULL and the parsed token is empty,
	// we’ve reached the end (return qfalse), otherwise qtrue
	if (!sv.entityParsePoint && !s[0]) {
		return qfalse;
	}

	return qtrue;
}


//==============================================

void SV_InitGameImport(gameImport_t* import) {

	//-------------------------------------------------------------------------
	// PRINT, ERROR
	//-------------------------------------------------------------------------
	import->Com_Printf              = Com_Printf;
	import->Com_Error               = Com_Error;

	//-------------------------------------------------------------------------
	// TIMING
	//-------------------------------------------------------------------------
	import->Sys_Milliseconds        = Sys_Milliseconds;

	//-------------------------------------------------------------------------
	// CVAR
	//-------------------------------------------------------------------------
	import->Cvar_Register           = Cvar_Register;
	import->Cvar_Update             = Cvar_Update;
	import->Cvar_Set                = Cvar_Set;
	import->Cvar_VariableIntegerValue = Cvar_VariableIntegerValue;
	import->Cvar_VariableStringBuffer = Cvar_VariableStringBuffer;

	//-------------------------------------------------------------------------
	// COMMANDS & CONSOLE
	//-------------------------------------------------------------------------
	import->Cmd_Argc                = Cmd_Argc;
	import->Cmd_ArgvBuffer          = Cmd_ArgvBuffer;
	import->Cbuf_ExecuteText        = Cbuf_ExecuteText;

	//-------------------------------------------------------------------------
	// FILESYSTEM
	//-------------------------------------------------------------------------
	import->FS_FOpenFileByMode      = FS_FOpenFileByMode;
	import->FS_Read                 = FS_Read;
	import->FS_Write                = FS_Write;
	import->FS_Rename               = FS_Rename;
	import->FS_FCloseFile           = FS_FCloseFile;
	import->FS_GetFileList          = FS_GetFileList;

	//-------------------------------------------------------------------------
	// ENTITY LINKING / SPATIAL
	//-------------------------------------------------------------------------
	import->SV_LocateGameData       = SV_LocateGameData;
	import->SV_GameDropClient       = SV_GameDropClient;
	import->SV_GameSendServerCommand = SV_GameSendServerCommand;
	import->SV_LinkEntity           = SV_LinkEntity;
	import->SV_UnlinkEntity         = SV_UnlinkEntity;
	import->SV_AreaEntities         = SV_AreaEntities;
	import->SV_EntityContact        = SV_EntityContact;

	//-------------------------------------------------------------------------
	// TRACING
	//-------------------------------------------------------------------------
	import->SV_Trace                = SV_Trace;
	import->SV_PointContents        = SV_PointContents;

	//-------------------------------------------------------------------------
	// PVS
	//-------------------------------------------------------------------------
	import->SV_SetBrushModel        = SV_SetBrushModel;
	import->SV_inPVS                = SV_inPVS;
	import->SV_inPVSIgnorePortals   = SV_inPVSIgnorePortals;

	//-------------------------------------------------------------------------
	// CONFIGSTRINGS, USERINFO, SERVERINFO
	//-------------------------------------------------------------------------
	import->SV_SetConfigstring      = SV_SetConfigstring;
	import->SV_GetConfigstring      = SV_GetConfigstring;
	import->SV_SetUserinfo          = SV_SetUserinfo;
	import->SV_GetUserinfo          = SV_GetUserinfo;
	import->SV_GetServerinfo        = SV_GetServerinfo;
	import->SV_AdjustAreaPortalState = SV_AdjustAreaPortalState;
	import->CM_AreasConnected       = CM_AreasConnected;

	//-------------------------------------------------------------------------
	// BOT CLIENT MANAGEMENT
	//-------------------------------------------------------------------------
	import->SV_BotAllocateClient    = SV_BotAllocateClient;
	import->SV_BotFreeClient        = SV_BotFreeClient;

	//-------------------------------------------------------------------------
	// USER COMMANDS
	//-------------------------------------------------------------------------
	import->SV_GetUsercmd           = SV_GetUsercmd;

	//-------------------------------------------------------------------------
	// ENTITY PARSING
	//-------------------------------------------------------------------------
	import->GetEntityToken          = G_GetEntityToken; // or COM_Parse if that’s what you use

	//-------------------------------------------------------------------------
	// DEBUG POLYGONS (BOT DEBUG)
	//-------------------------------------------------------------------------
	import->BotImport_DebugPolygonCreate = BotImport_DebugPolygonCreate;
	import->BotImport_DebugPolygonDelete = BotImport_DebugPolygonDelete;

	//-------------------------------------------------------------------------
	// MISC
	//-------------------------------------------------------------------------
	import->Com_RealTime            = Com_RealTime;
	import->Sys_SnapVector          = Sys_SnapVector;
	import->SV_GetTag               = SV_GetTag;

	//=========================================================================
	// BOTLIB CALLS (Adapt to your botlib_export usage)
	//=========================================================================
	import->SV_BotLibSetup          = SV_BotLibSetup;
	import->SV_BotLibShutdown       = SV_BotLibShutdown;
	import->BotLibVarSet            = botlib_export->BotLibVarSet;
	import->BotLibVarGet            = botlib_export->BotLibVarGet;

	// PC_ calls
	import->PC_AddGlobalDefine      = botlib_export->PC_AddGlobalDefine;
	import->PC_LoadSourceHandle     = botlib_export->PC_LoadSourceHandle;
	import->PC_FreeSourceHandle     = botlib_export->PC_FreeSourceHandle;
	import->PC_ReadTokenHandle      = botlib_export->PC_ReadTokenHandle;
	import->PC_SourceFileAndLine    = botlib_export->PC_SourceFileAndLine;

	// BotLib frames, map loads, etc.
	import->BotLibStartFrame        = botlib_export->BotLibStartFrame;
	import->BotLibLoadMap           = botlib_export->BotLibLoadMap;
	import->BotLibUpdateEntity      = botlib_export->BotLibUpdateEntity;
	import->BotLibTest              = botlib_export->Test;

	// Snapshots / console
	import->SV_BotGetSnapshotEntity = SV_BotGetSnapshotEntity;
	import->SV_BotGetConsoleMessage = SV_BotGetConsoleMessage;
	import->SV_ClientThink          = SV_ClientThinkWithId;

	// AAS_
	import->AAS_EntityInfo          = botlib_export->aas.AAS_EntityInfo;
	import->AAS_Initialized         = botlib_export->aas.AAS_Initialized;
	import->AAS_PresenceTypeBoundingBox = botlib_export->aas.AAS_PresenceTypeBoundingBox;
	import->AAS_Time                = botlib_export->aas.AAS_Time;
	import->AAS_SetCurrentWorld     = botlib_export->aas.AAS_SetCurrentWorld;
	import->AAS_PointAreaNum        = botlib_export->aas.AAS_PointAreaNum;
	import->AAS_TraceAreas          = botlib_export->aas.AAS_TraceAreas;
	import->AAS_PointContents       = botlib_export->aas.AAS_PointContents;
	import->AAS_NextBSPEntity       = botlib_export->aas.AAS_NextBSPEntity;
	import->AAS_ValueForBSPEpairKey = botlib_export->aas.AAS_ValueForBSPEpairKey;
	import->AAS_VectorForBSPEpairKey = botlib_export->aas.AAS_VectorForBSPEpairKey;
	import->AAS_FloatForBSPEpairKey = botlib_export->aas.AAS_FloatForBSPEpairKey;
	import->AAS_IntForBSPEpairKey   = botlib_export->aas.AAS_IntForBSPEpairKey;
	import->AAS_AreaReachability    = botlib_export->aas.AAS_AreaReachability;
	import->AAS_AreaTravelTimeToGoalArea = botlib_export->aas.AAS_AreaTravelTimeToGoalArea;
	import->AAS_Swimming           = botlib_export->aas.AAS_Swimming;
	import->AAS_PredictClientMovement = botlib_export->aas.AAS_PredictClientMovement;

	// Route table / extra AAS
	import->AAS_RT_ShowRoute        = botlib_export->aas.AAS_RT_ShowRoute;
	import->AAS_RT_GetHidePos       = botlib_export->aas.AAS_RT_GetHidePos;
	import->AAS_FindAttackSpotWithinRange = botlib_export->aas.AAS_FindAttackSpotWithinRange;
	import->AAS_SetAASBlockingEntity = botlib_export->aas.AAS_SetAASBlockingEntity;

	// EA_
	import->EA_Say                  = botlib_export->ea.EA_Say;
	import->EA_SayTeam              = botlib_export->ea.EA_SayTeam;
	import->EA_UseItem              = botlib_export->ea.EA_UseItem;
	import->EA_DropItem             = botlib_export->ea.EA_DropItem;
	import->EA_UseInv               = botlib_export->ea.EA_UseInv;
	import->EA_DropInv              = botlib_export->ea.EA_DropInv;
	import->EA_Gesture              = botlib_export->ea.EA_Gesture;
	import->EA_Command              = botlib_export->ea.EA_Command;
	import->EA_SelectWeapon         = botlib_export->ea.EA_SelectWeapon;
	import->EA_Talk                 = botlib_export->ea.EA_Talk;
	import->EA_Attack               = botlib_export->ea.EA_Attack;
	import->EA_Reload               = botlib_export->ea.EA_Reload;
	import->EA_Use                  = botlib_export->ea.EA_Use;
	import->EA_Respawn              = botlib_export->ea.EA_Respawn;
	import->EA_Jump                 = botlib_export->ea.EA_Jump;
	import->EA_DelayedJump          = botlib_export->ea.EA_DelayedJump;
	import->EA_Crouch               = botlib_export->ea.EA_Crouch;
	import->EA_MoveUp               = botlib_export->ea.EA_MoveUp;
	import->EA_MoveDown             = botlib_export->ea.EA_MoveDown;
	import->EA_MoveForward          = botlib_export->ea.EA_MoveForward;
	import->EA_MoveBack             = botlib_export->ea.EA_MoveBack;
	import->EA_MoveLeft             = botlib_export->ea.EA_MoveLeft;
	import->EA_MoveRight            = botlib_export->ea.EA_MoveRight;
	import->EA_Move                 = botlib_export->ea.EA_Move;
	import->EA_View                 = botlib_export->ea.EA_View;
	import->EA_EndRegular           = botlib_export->ea.EA_EndRegular;
	import->EA_GetInput             = botlib_export->ea.EA_GetInput;
	import->EA_ResetInput           = botlib_export->ea.EA_ResetInput;

	// AI_ (Bot)
	import->BotLoadCharacter        = botlib_export->ai.BotLoadCharacter;
	import->BotFreeCharacter        = botlib_export->ai.BotFreeCharacter;
	import->Characteristic_Float    = botlib_export->ai.Characteristic_Float;
	import->Characteristic_BFloat   = botlib_export->ai.Characteristic_BFloat;
	import->Characteristic_Integer  = botlib_export->ai.Characteristic_Integer;
	import->Characteristic_BInteger = botlib_export->ai.Characteristic_BInteger;
	import->Characteristic_String   = botlib_export->ai.Characteristic_String;

	// AI_ Chat
	import->BotAllocChatState       = botlib_export->ai.BotAllocChatState;
	import->BotFreeChatState        = botlib_export->ai.BotFreeChatState;
	import->BotQueueConsoleMessage  = botlib_export->ai.BotQueueConsoleMessage;
	import->BotRemoveConsoleMessage = botlib_export->ai.BotRemoveConsoleMessage;
	import->BotNextConsoleMessage   = botlib_export->ai.BotNextConsoleMessage;
	import->BotNumConsoleMessages   = botlib_export->ai.BotNumConsoleMessages;
	import->BotInitialChat          = botlib_export->ai.BotInitialChat;
	import->BotNumInitialChats      = botlib_export->ai.BotNumInitialChats;
	import->BotReplyChat            = botlib_export->ai.BotReplyChat;
	import->BotChatLength           = botlib_export->ai.BotChatLength;
	import->BotEnterChat            = botlib_export->ai.BotEnterChat;
	import->BotGetChatMessage       = botlib_export->ai.BotGetChatMessage;
	import->StringContains          = botlib_export->ai.StringContains;
	import->BotFindMatch            = botlib_export->ai.BotFindMatch;
	import->BotMatchVariable        = botlib_export->ai.BotMatchVariable;
	import->UnifyWhiteSpaces        = botlib_export->ai.UnifyWhiteSpaces;
	import->BotReplaceSynonyms      = botlib_export->ai.BotReplaceSynonyms;
	import->BotLoadChatFile         = botlib_export->ai.BotLoadChatFile;
	import->BotSetChatGender        = botlib_export->ai.BotSetChatGender;
	import->BotSetChatName          = botlib_export->ai.BotSetChatName;

	// AI_ Goals
	import->BotResetGoalState       = botlib_export->ai.BotResetGoalState;
	import->BotResetAvoidGoals      = botlib_export->ai.BotResetAvoidGoals;
	import->BotRemoveFromAvoidGoals = botlib_export->ai.BotRemoveFromAvoidGoals;
	import->BotPushGoal             = botlib_export->ai.BotPushGoal;
	import->BotPopGoal              = botlib_export->ai.BotPopGoal;
	import->BotEmptyGoalStack       = botlib_export->ai.BotEmptyGoalStack;
	import->BotDumpAvoidGoals       = botlib_export->ai.BotDumpAvoidGoals;
	import->BotDumpGoalStack        = botlib_export->ai.BotDumpGoalStack;
	import->BotGoalName             = botlib_export->ai.BotGoalName;
	import->BotGetTopGoal           = botlib_export->ai.BotGetTopGoal;
	import->BotGetSecondGoal        = botlib_export->ai.BotGetSecondGoal;
	import->BotChooseLTGItem        = botlib_export->ai.BotChooseLTGItem;
	import->BotChooseNBGItem        = botlib_export->ai.BotChooseNBGItem;
	import->BotTouchingGoal         = botlib_export->ai.BotTouchingGoal;
	import->BotItemGoalInVisButNotVisible = botlib_export->ai.BotItemGoalInVisButNotVisible;
	import->BotGetLevelItemGoal     = botlib_export->ai.BotGetLevelItemGoal;
	import->BotGetNextCampSpotGoal  = botlib_export->ai.BotGetNextCampSpotGoal;
	import->BotGetMapLocationGoal   = botlib_export->ai.BotGetMapLocationGoal;
	import->BotAvoidGoalTime        = botlib_export->ai.BotAvoidGoalTime;
	import->BotInitLevelItems       = botlib_export->ai.BotInitLevelItems;
	import->BotUpdateEntityItems    = botlib_export->ai.BotUpdateEntityItems;
	import->BotLoadItemWeights      = botlib_export->ai.BotLoadItemWeights;
	import->BotFreeItemWeights      = botlib_export->ai.BotFreeItemWeights;
	import->BotInterbreedGoalFuzzyLogic = botlib_export->ai.BotInterbreedGoalFuzzyLogic;
	import->BotSaveGoalFuzzyLogic   = botlib_export->ai.BotSaveGoalFuzzyLogic;
	import->BotMutateGoalFuzzyLogic = botlib_export->ai.BotMutateGoalFuzzyLogic;
	import->BotAllocGoalState       = botlib_export->ai.BotAllocGoalState;
	import->BotFreeGoalState        = botlib_export->ai.BotFreeGoalState;

	// AI_ Movement
	import->BotResetMoveState       = botlib_export->ai.BotResetMoveState;
	import->BotMoveToGoal           = botlib_export->ai.BotMoveToGoal;
	import->BotMoveInDirection      = botlib_export->ai.BotMoveInDirection;
	import->BotResetAvoidReach      = botlib_export->ai.BotResetAvoidReach;
	import->BotResetLastAvoidReach  = botlib_export->ai.BotResetLastAvoidReach;
	import->BotReachabilityArea     = botlib_export->ai.BotReachabilityArea;
	import->BotMovementViewTarget   = botlib_export->ai.BotMovementViewTarget;
	import->BotPredictVisiblePosition = botlib_export->ai.BotPredictVisiblePosition;
	import->BotAllocMoveState       = botlib_export->ai.BotAllocMoveState;
	import->BotFreeMoveState        = botlib_export->ai.BotFreeMoveState;
	import->BotInitMoveState        = botlib_export->ai.BotInitMoveState;
	import->BotInitAvoidReach       = botlib_export->ai.BotInitAvoidReach;

	// AI_ Weapons
	import->BotChooseBestFightWeapon = botlib_export->ai.BotChooseBestFightWeapon;
	import->BotGetWeaponInfo        = botlib_export->ai.BotGetWeaponInfo;
	import->BotLoadWeaponWeights    = botlib_export->ai.BotLoadWeaponWeights;
	import->BotAllocWeaponState     = botlib_export->ai.BotAllocWeaponState;
	import->BotFreeWeaponState      = botlib_export->ai.BotFreeWeaponState;
	import->BotResetWeaponState     = botlib_export->ai.BotResetWeaponState;

	// Genetic selection
	import->GeneticParentsAndChildSelection = botlib_export->ai.GeneticParentsAndChildSelection;
}

/*
===============
SV_ShutdownGameProgs

Called every time a map changes
===============
*/
void SV_ShutdownGameProgs( void ) {
	if (!gamedll) {
		return;
	}
	if (gvm) {
		gvm->ShutdownGame(qfalse);
	}
	delete gamedll;
	gvm = NULL;
	gamedll = nullptr;
}

/*
==================
SV_InitGameVM

Called for both a full init and a restart
==================
*/
static void SV_InitGameVM( qboolean restart ) {
	int i;
	static gameImport_t imports;

	// load the dll
	gamedll = new DllLoader("game");
	if (!gamedll->IsLoaded())
	{
		delete gamedll;
		gamedll = nullptr;
		gvm = nullptr;
		Com_Error(ERR_DROP, "VM_Create on game failed");
		return;
	}

	// Init the engine exports.
	SV_InitGameImport(&imports);
	gvm = gamedll->GetDllEntry<gameExport_t, gameImport_t>(&imports);
	if (!gvm)
	{
		delete gamedll;
		gvm = nullptr;
		gamedll = nullptr;
		Com_Error(ERR_DROP, "VM_Create on game failed, gvm is invalid");
		return;
	}

	// start the entity parsing at the beginning
	sv.entityParsePoint = CM_EntityString();

	// clear all gentity pointers that might still be set from
	// a previous level
	for ( i = 0 ; i < sv_maxclients->integer ; i++ ) {
		svs.clients[i].gentity = NULL;
	}

	// use the current msec count for a random seed
	// init for this gamestate	
	gvm->InitGame(svs.time, Com_Milliseconds(), restart );
}



/*
===================
SV_RestartGameProgs

Called on a map_restart, but not on a normal map change
===================
*/
void SV_RestartGameProgs( void ) {
	if ( !gvm ) {
		return;
	}
	
	SV_ShutdownGameProgs();

	SV_InitGameVM( qfalse );
}


/*
===============
SV_InitGameProgs

Called on a normal map change, not on a map_restart
===============
*/
void SV_InitGameProgs( void ) {	
	SV_InitGameVM( qfalse );
}


/*
====================
SV_GameCommand

See if the current console command is claimed by the game
====================
*/
qboolean SV_GameCommand( void ) {
	if ( sv.state != SS_GAME ) {
		return qfalse;
	}

	return gvm->ConsoleCommand();
}


/*
====================
SV_SendMoveSpeedsToGame
====================
*/
void SV_SendMoveSpeedsToGame( int entnum, char *text ) {
	if ( !gvm ) {
		return;
	}
	gvm->RetrieveMoveSpeedsFromClient(entnum, text);
}

/*
====================
SV_GetTag

  return qfalse if unable to retrieve tag information for this client
====================
*/
extern qboolean CL_GetTag( int clientNum, char *tagname, orientation_t * or );

qboolean SV_GetTag( int clientNum, char *tagname, orientation_t *or ) {
#ifndef DEDICATED // TTimo: dedicated only binary defines DEDICATED
	if ( com_dedicated->integer ) {
		return qfalse;
	}

	return CL_GetTag( clientNum, tagname, or );
#else
	return qfalse;
#endif
}

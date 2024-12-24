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


// g_public.h -- game module information visible to server

#include "../game/botlib.h"

#define GAME_API_VERSION    8

// entity->svFlags
// the server does not know how to interpret most of the values
// in entityStates (level eType), so the game must explicitly flag
// special server behaviors
#define SVF_NOCLIENT            0x00000001  // don't send entity to clients, even if it has effects
#define SVF_VISDUMMY            0x00000004  // this ent is a "visibility dummy" and needs it's master to be sent to clients that can see it even if they can't see the master ent
#define SVF_BOT                 0x00000008
// Wolfenstein
#define SVF_CASTAI              0x00000010
// done.
#define SVF_BROADCAST           0x00000020  // send to all connected clients
#define SVF_PORTAL              0x00000040  // merge a second pvs at origin2 into snapshots
#define SVF_USE_CURRENT_ORIGIN  0x00000080  // entity->r.currentOrigin instead of entity->s.origin
											// for link position (missiles and movers)
// Ridah
#define SVF_NOFOOTSTEPS         0x00000100
// done.
// MrE:
#define SVF_CAPSULE             0x00000200  // use capsule for collision detection

#define SVF_VISDUMMY_MULTIPLE   0x00000400  // so that one vis dummy can add to snapshot multiple speakers

// recent id changes
#define SVF_SINGLECLIENT        0x00000800  // only send to a single client (entityShared_t->singleClient)
#define SVF_NOSERVERINFO        0x00001000  // don't send CS_SERVERINFO updates to this client
											// so that it can be updated for ping tools without
											// lagging clients
#define SVF_NOTSINGLECLIENT     0x00002000  // send entity to everyone but one client
											// (entityShared_t->singleClient)

//===============================================================


typedef struct {
	entityState_t s;                // communicated by server to clients

	qboolean linked;                // qfalse if not in any good cluster
	int linkcount;

	int svFlags;                    // SVF_NOCLIENT, SVF_BROADCAST, etc
	int singleClient;               // only send to this client when SVF_SINGLECLIENT is set

	qboolean bmodel;                // if false, assume an explicit mins / maxs bounding box
									// only set by trap_SetBrushModel
	vec3_t mins, maxs;
	int contents;                   // CONTENTS_TRIGGER, CONTENTS_SOLID, CONTENTS_BODY, etc
									// a non-solid entity should set to 0

	vec3_t absmin, absmax;          // derived from mins/maxs and origin + rotation

	// currentOrigin will be used for all collision detection and world linking.
	// it will not necessarily be the same as the trajectory evaluation for the current
	// time, because each entity must be moved one at a time after time is advanced
	// to avoid simultanious collision issues
	vec3_t currentOrigin;
	vec3_t currentAngles;

	// when a trace call is made and passEntityNum != ENTITYNUM_NONE,
	// an ent will be excluded from testing if:
	// ent->s.number == passEntityNum	(don't interact with self)
	// ent->s.ownerNum = passEntityNum	(don't interact with your own missiles)
	// entity[ent->s.ownerNum].ownerNum = passEntityNum	(don't interact with other missiles from owner)
	int ownerNum;
	int eventTime;

	int worldflags;             // DHM - Nerve
} entityShared_t;



// the server looks at a sharedEntity, which is the start of the game's gentity_t structure
typedef struct {
	entityState_t s;                // communicated by server to clients
	entityShared_t r;               // shared by both the server system and game
} sharedEntity_t;



//===============================================================

//
// system traps provided by the main engine
//
/*
 * Structure grouping all "trap" or "import" functions from the engine
 * so the game (mod) code can call back into the engine/botlib, etc.
 */
typedef struct gameImport_s
{
    //
    // Print & error
    //
    void  (*Com_Printf)(const char* fmt, ...);
    void  (*Com_Error)(int code, const char* fmt, ...);

    //
    // Timing
    //
    int   (*Sys_Milliseconds)(void);

    //
    // CVARs
    //
    void  (*Cvar_Register)(vmCvar_t* vmCvar, const char* varName, const char* defaultValue, int flags);
    void  (*Cvar_Update)(vmCvar_t* vmCvar);
    void  (*Cvar_Set)(const char* var_name, const char* value);
    int   (*Cvar_VariableIntegerValue)(const char* var_name);
    void  (*Cvar_VariableStringBuffer)(const char* var_name,
        char* buffer, int bufsize);

    //
    // Commands & console
    //
    int   (*Cmd_Argc)(void);
    void  (*Cmd_ArgvBuffer)(int arg, char* buffer, int bufferLength);
    void  (*Cbuf_ExecuteText)(int exec_when, const char* text);

    //
    // Filesystem
    //
    int   (*FS_FOpenFileByMode)(const char* qpath, fileHandle_t* f, fsMode_t mode);
    int   (*FS_Read)(void* buffer, int len, fileHandle_t f);
    int   (*FS_Write)(const void* buffer, int len, fileHandle_t f);
    void  (*FS_Rename)(const char* from, const char* to);
    void  (*FS_FCloseFile)(fileHandle_t f);
    int   (*FS_GetFileList)(const char* path, const char* extension,
        char* listbuf, int bufsize);

    //
    // Linking / entity interactions
    //
    void  (*SV_LocateGameData)(sharedEntity_t* gEnts, int numGEntities, int sizeofGEntity_t, playerState_t* clients, int sizeofGameClient);
    void  (*SV_GameDropClient)(int clientNum, const char* reason);
    void  (*SV_GameSendServerCommand)(int clientNum, const char* text);
    void  (*SV_LinkEntity)(sharedEntity_t* ent);
    void  (*SV_UnlinkEntity)(sharedEntity_t* ent);
    int   (*SV_AreaEntities)(const vec3_t mins, const vec3_t maxs,
        int* entityList, int maxcount);
    qboolean   (*SV_EntityContact)(const vec3_t mins, const vec3_t maxs, const sharedEntity_t* gEnt, const int capsule);

    //
    // Tracing
    //
    void  (*SV_Trace)(trace_t* results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask, int capsule);
    int   (*SV_PointContents)(const vec3_t p, int passEntityNum);

    //
    // PVS
    //
    void  (*SV_SetBrushModel)(sharedEntity_t* ent, const char* name);
    qboolean(*SV_inPVS)(const vec3_t p1, const vec3_t p2);
    qboolean(*SV_inPVSIgnorePortals)(const vec3_t p1, const vec3_t p2);

    //
    // Configstrings / userinfo
    //
    void  (*SV_SetConfigstring)(int index, const char* val);
    void  (*SV_GetConfigstring)(int index, char* buffer, int bufferSize);
    void  (*SV_SetUserinfo)(int index, const char* val);
    void  (*SV_GetUserinfo)(int index, char* buffer, int bufferSize);
    void  (*SV_GetServerinfo)(char* buffer, int bufferSize);
    void  (*SV_AdjustAreaPortalState)(sharedEntity_t* ent, qboolean open);
    qboolean(*CM_AreasConnected)(int area1, int area2);

    //
    // Bot clients
    //
    int   (*SV_BotAllocateClient)(void);
    void  (*SV_BotFreeClient)(int clientNum);

    //
    // User commands
    //
    void  (*SV_GetUsercmd)(int clientNum, usercmd_t* cmd);

    //
    // Entity parsing
    //
    // G_GET_ENTITY_TOKEN is basically COM_Parse (some engine global)
    // Return qboolean but effectively int in C code:
    qboolean(*GetEntityToken)(char* buffer, int bufferSize);

    //
    // Debug polygons (for bot debugging)
    //
    int   (*BotImport_DebugPolygonCreate)(int color, int numPoints, vec3_t* points);
    void  (*BotImport_DebugPolygonDelete)(int id);

    //
    // Misc
    //
    int   (*Com_RealTime)(qtime_t* qtime);
    void  (*Sys_SnapVector)(float* v);
    qboolean(*SV_GetTag)(int clientNum, char* tagname, orientation_t* or );

	//
//=========================================================================
// BOTLIB CALLS
//=========================================================================
//
// General BotLib management
//
	int         (*SV_BotLibSetup)(void);
	int         (*SV_BotLibShutdown)(void);
	int         (*BotLibVarSet)(char* var_name, char* value);
	int         (*BotLibVarGet)(char* var_name, char* buffer, int size);

	//
	// BotLib PC_ calls (parser)
	//
	int         (*PC_AddGlobalDefine)(char* define);
	int         (*PC_LoadSourceHandle)(const char* filename);
	int         (*PC_FreeSourceHandle)(int handle);
	int         (*PC_ReadTokenHandle)(int handle, pc_token_t* pc_token);
	int         (*PC_SourceFileAndLine)(int handle, char* filename, int* line);

	//
	// BotLib frames, map loads, etc.
	//
	int         (*BotLibStartFrame)(float time);
	int         (*BotLibLoadMap)(const char* mapname);
	int         (*BotLibUpdateEntity)(int ent, struct bot_entitystate_s* state);
	int         (*BotLibTest)(int parm0, char* parm1, vec3_t parm2, vec3_t parm3);

	//
	// Snapshots / console messages / user commands
	//
	int         (*SV_BotGetSnapshotEntity)(int clientNum, int sequence);
	int         (*SV_BotGetConsoleMessage)(int clientNum, char* buf, int size);
	// BOTLIB_USER_COMMAND is effectively SV_ClientThink in the switch statement
	void        (*SV_ClientThink)(int clientId, usercmd_t* cmd);

	//
	// AAS_ calls
	//
	void        (*AAS_EntityInfo)(int entnum, struct aas_entityinfo_s* info);  // typically aas_entityinfo_t*
	int         (*AAS_Initialized)(void);
	void        (*AAS_PresenceTypeBoundingBox)(int presencetype, vec3_t mins, vec3_t maxs);
	float       (*AAS_Time)(void);
	void        (*AAS_SetCurrentWorld)(int index);
	int         (*AAS_PointAreaNum)(vec3_t point);
	int         (*AAS_TraceAreas)(vec3_t start, vec3_t end, int* areas, vec3_t* points, int maxareas);
	int         (*AAS_PointContents)(vec3_t point);
	int         (*AAS_NextBSPEntity)(int ent);
	int         (*AAS_ValueForBSPEpairKey)(int ent, char* key, char* value, int size);
	int         (*AAS_VectorForBSPEpairKey)(int ent, char* key, vec3_t v);
	int         (*AAS_FloatForBSPEpairKey)(int ent, char* key, float* value);
	int         (*AAS_IntForBSPEpairKey)(int ent, char* key, int* value);
	int         (*AAS_AreaReachability)(int areanum);
	int         (*AAS_AreaTravelTimeToGoalArea)(int areanum, vec3_t origin,
		int goalareanum, int travelflags);
	int         (*AAS_Swimming)(vec3_t origin);
	int         (*AAS_PredictClientMovement)(struct aas_clientmove_s* move,
		int entnum, vec3_t origin,
		int presencetype, int onground,
		vec3_t velocity, vec3_t cmdmove,
		int cmdframes,
		int maxframes, float frametime,
		int stopevent, int stopareanum, int visualize);

	// Route table / extra AAS stuff
	void        (*AAS_RT_ShowRoute)(vec3_t srcpos, int srcarea, int destarea);
	qboolean    (*AAS_RT_GetHidePos)(vec3_t srcpos, int srcnum, int srcarea, vec3_t destpos, int destnum, int destarea, vec3_t returnPos);
	int         (*AAS_FindAttackSpotWithinRange)(int srcnum, int enemyareanum,
		int travelflags, float maxrange,
		int prefer, vec3_t returnpos);
	void        (*AAS_SetAASBlockingEntity)(vec3_t mins, vec3_t maxs, qboolean blocking);

	//
	// EA_ (external AI) calls
	//
	void        (*EA_Say)(int client, char* str);
	void        (*EA_SayTeam)(int client, char* str);
	void        (*EA_UseItem)(int client, char* it);
	void        (*EA_DropItem)(int client, char* it);
	void        (*EA_UseInv)(int client, char* inv);
	void        (*EA_DropInv)(int client, char* inv);
	void        (*EA_Gesture)(int client);
	void        (*EA_Command)(int client, char* command);
	void        (*EA_SelectWeapon)(int client, int weapon);
	void        (*EA_Talk)(int client);
	void        (*EA_Attack)(int client);
	void        (*EA_Reload)(int client);
	void        (*EA_Use)(int client);
	void        (*EA_Respawn)(int client);
	void        (*EA_Jump)(int client);
	void        (*EA_DelayedJump)(int client);
	void        (*EA_Crouch)(int client);
	void        (*EA_MoveUp)(int client);
	void        (*EA_MoveDown)(int client);
	void        (*EA_MoveForward)(int client);
	void        (*EA_MoveBack)(int client);
	void        (*EA_MoveLeft)(int client);
	void        (*EA_MoveRight)(int client);
	void        (*EA_Move)(int client, vec3_t dir, float speed);
	void        (*EA_View)(int client, vec3_t viewangles);
	void        (*EA_EndRegular)(int client, float thinktime);
	void        (*EA_GetInput)(int client, float thinktime, bot_input_t* action); // typically bot_input_t*
	void        (*EA_ResetInput)(int client, bot_input_t* action);

	//
	// AI_ Character / Chat
	//
	int         (*BotLoadCharacter)(char* charfile, int skill);
	void        (*BotFreeCharacter)(int character);
	float       (*Characteristic_Float)(int character, int index);
	float       (*Characteristic_BFloat)(int character, int index, float min, float max);
	int         (*Characteristic_Integer)(int character, int index);
	int         (*Characteristic_BInteger)(int character, int index, int min, int max);
	void        (*Characteristic_String)(int character, int index, char* buf, int size);

	//
	// AI_ Chat State
	//
	int         (*BotAllocChatState)(void);
	void        (*BotFreeChatState)(int handle);
	void        (*BotQueueConsoleMessage)(int chatstate, int type, char* message);
	void        (*BotRemoveConsoleMessage)(int chatstate, int handle);
	int         (*BotNextConsoleMessage)(int chatstate, struct bot_consolemessage_s* cm); // bot_consolemessage_t*
	int         (*BotNumConsoleMessages)(int chatstate);
	void        (*BotInitialChat)(int chatstate, char* type, int mfunc,
		char* p1, char* p2, char* p3,
		char* p4, char* p5, char* p6,
		char* p7, char* p8);
	int         (*BotNumInitialChats)(int chatstate, char* type);
	int         (*BotReplyChat)(int chatstate, char* message, int mfunc,
		int mfunc2, char* p1, char* p2,
		char* p3, char* p4, char* p5,
		char* p6, char* p7, char* p8);
	int         (*BotChatLength)(int chatstate);
	void        (*BotEnterChat)(int chatstate, int client, int sendto);
	void        (*BotGetChatMessage)(int chatstate, char* buf, int size);
	int         (*StringContains)(char* str1, char* str2, int casesensitive);
	int         (*BotFindMatch)(char* str, struct bot_match_s* match, unsigned long int context);
	void        (*BotMatchVariable)(struct bot_match_s* match, int variable, char* buf, int size);
	void        (*UnifyWhiteSpaces)(char* string);
	void        (*BotReplaceSynonyms)(char* string, unsigned long context);
	int         (*BotLoadChatFile)(int chatstate, char* chatfile, char* filename);
	void        (*BotSetChatGender)(int chatstate, int gender);
	void        (*BotSetChatName)(int chatstate, char* name);

	//
	// AI_ Goals
	//
	void        (*BotResetGoalState)(int goalstate);
	void        (*BotResetAvoidGoals)(int goalstate);
	void        (*BotRemoveFromAvoidGoals)(int goalstate, int number);
	void        (*BotPushGoal)(int goalstate, struct bot_goal_s* goal);
	void        (*BotPopGoal)(int goalstate);
	void        (*BotEmptyGoalStack)(int goalstate);
	void        (*BotDumpAvoidGoals)(int goalstate);
	void        (*BotDumpGoalStack)(int goalstate);
	void        (*BotGoalName)(int number, char* namebuf, int size);
	int         (*BotGetTopGoal)(int goalstate, struct bot_goal_s* goal);
	int         (*BotGetSecondGoal)(int goalstate, struct bot_goal_s* goal);
	int         (*BotChooseLTGItem)(int goalstate, vec_t* origin, int* inventory, int travelflags);
	int         (*BotChooseNBGItem)(int goalstate, vec3_t origin, int* inventory, int travelflags,struct bot_goal_s* ltg, float maxtime);
	int         (*BotTouchingGoal)(vec3_t origin, struct bot_goal_s* goal);
	int         (*BotItemGoalInVisButNotVisible)(int viewer, vec3_t eye,
		vec3_t viewangles, struct bot_goal_s* goal);
	int         (*BotGetLevelItemGoal)(int index, char* classname, struct bot_goal_s* goal);
	int         (*BotGetNextCampSpotGoal)(int num, struct bot_goal_s* goal);
	int         (*BotGetMapLocationGoal)(char* name, struct bot_goal_s* goal);
	float       (*BotAvoidGoalTime)(int goalstate, int number);
	void        (*BotInitLevelItems)(void);
	void        (*BotUpdateEntityItems)(void);
	int         (*BotLoadItemWeights)(int goalstate, char* filename);
	void        (*BotFreeItemWeights)(int goalstate);
	void        (*BotInterbreedGoalFuzzyLogic)(int parent1, int parent2, int child);
	void        (*BotSaveGoalFuzzyLogic)(int goalstate, char* filename);
	void        (*BotMutateGoalFuzzyLogic)(int goalstate, float range);
	int         (*BotAllocGoalState)(int client);
	void        (*BotFreeGoalState)(int handle);

	//
	// AI_ Movement
	//
	void        (*BotResetMoveState)(int movestate);
	void        (*BotMoveToGoal)(struct bot_moveresult_s* result, int movestate, struct bot_goal_s* goal, int travelflags);
	int         (*BotMoveInDirection)(int movestate, vec3_t dir, float speed, int type);
	void        (*BotResetAvoidReach)(int movestate);
	void        (*BotResetLastAvoidReach)(int movestate);
	int         (*BotReachabilityArea)(vec3_t origin, int testground);
	int         (*BotMovementViewTarget)(int movestate, struct bot_goal_s* goal, int travelflags, float lookahead, vec3_t target);
	int         (*BotPredictVisiblePosition)(vec3_t origin, int areanum, struct bot_goal_s* goal, int travelflags, vec3_t target);
	int         (*BotAllocMoveState)(void);
	void        (*BotFreeMoveState)(int handle);
	void        (*BotInitMoveState)(int handle, struct bot_initmove_s* initmove);
	void        (*BotInitAvoidReach)(int handle);

	//
	// AI_ Weapons
	//
	int         (*BotChooseBestFightWeapon)(int weaponstate, int* inventory);
	void        (*BotGetWeaponInfo)(int weaponstate, int weapon, struct weaponinfo_s* weaponinfo);
	int         (*BotLoadWeaponWeights)(int weaponstate, char* filename);
	int         (*BotAllocWeaponState)(void);
	void        (*BotFreeWeaponState)(int weaponstate);
	void        (*BotResetWeaponState)(int weaponstate);

	//
	// Genetic selection
	//
	int         (*GeneticParentsAndChildSelection)(int numranks, float* ranks,
		int* parent1, int* parent2, int* child);

} gameImport_t;


//
// functions exported by the game subsystem
typedef struct gameExport_s {
	void (*InitGame)(int levelTime, int randomSeed, int restart);
	void (*ShutdownGame)(int restart);
	char *(*ClientConnect)(int clientNum, qboolean firstTime, qboolean isBot);
	void (*ClientThink)(int clientNum);
	void (*ClientUserinfoChanged)(int clientNum);
	void (*ClientDisconnect)(int clientNum);
	void (*ClientBegin)(int clientNum);
	void (*ClientCommand)(int clientNum);
	void (*RunFrame)(int levelTime);
	qboolean(*ConsoleCommand)(void);
	int (*BotAIStartFrame)(int time);
	// Ridah, Cast AI
	qboolean(*AICast_VisibleFromPos)(float* srcOrigin, int srcNum, float* destOrigin, int destNum, qboolean isShrouded);
	qboolean(*AICast_CheckAttackAtPos)(int entityNum, int enemyNum, float* origin, qboolean direct, qboolean wander);
	// done.

	void (*RetrieveMoveSpeedsFromClient)(int entnum, char* text);
} gameExport_t;



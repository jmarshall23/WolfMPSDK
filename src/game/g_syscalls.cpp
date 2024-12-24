/*s
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

#include "g_local.h"

//
// Global pointer to the import table
//
static gameImport_t gi = {};

void G_InitGame(int levelTime, int randomSeed, int restart);
void G_RunFrame(int levelTime);
void G_ShutdownGame(int restart);
void CheckExitRules(void);
qboolean AICast_CheckAttackAtPos(int entnum, int enemy, vec3_t pos, qboolean ducking, qboolean allowHitWorld);
void G_RetrieveMoveSpeedsFromClient(int entnum, char* text);

gameExport_t *dllEntry(gameImport_t* import) {
	static gameExport_t gameExport;

	gameExport.InitGame = G_InitGame;
	gameExport.ShutdownGame = G_ShutdownGame;
	gameExport.ClientConnect = ClientConnect;
	gameExport.ClientThink = ClientThink;
	gameExport.ClientUserinfoChanged = ClientUserinfoChanged;
	gameExport.ClientDisconnect = ClientDisconnect;
	gameExport.ClientBegin = ClientBegin;
	gameExport.ClientCommand = ClientCommand;
	gameExport.RunFrame = G_RunFrame;
	gameExport.ConsoleCommand = ConsoleCommand;
	gameExport.BotAIStartFrame = BotAIStartFrame;
	// Ridah, Cast AI
	gameExport.AICast_VisibleFromPos = AICast_VisibleFromPos;
	gameExport.AICast_CheckAttackAtPos = AICast_CheckAttackAtPos;
	// done
	gameExport.RetrieveMoveSpeedsFromClient = G_RetrieveMoveSpeedsFromClient;

	gi = *import;

	return &gameExport;
}

//
// Now replace each trap_* function with direct calls to gi....
//

void trap_Printf(const char* fmt) {
	// Original was: syscall( G_PRINT, fmt );
	gi.Com_Printf("%s", fmt);
}

void trap_Error(const char* fmt) {
	// Original was: syscall( G_ERROR, fmt );
	// Com_Error takes (int code, const char* fmt, ...)
	// Choose an error code that best suits your usage (ERR_DROP or ERR_FATAL, etc.)
	gi.Com_Error(ERR_DROP, "%s", fmt);
}

int trap_Milliseconds(void) {
	// Original was: return syscall( G_MILLISECONDS );
	return gi.Sys_Milliseconds();
}

int trap_Argc(void) {
	return gi.Cmd_Argc();
}

void trap_Argv(int n, char* buffer, int bufferLength) {
	gi.Cmd_ArgvBuffer(n, buffer, bufferLength);
}

int trap_FS_FOpenFile(const char* qpath, fileHandle_t* f, fsMode_t mode) {
	return gi.FS_FOpenFileByMode(qpath, f, mode);
}

void trap_FS_Read(void* buffer, int len, fileHandle_t f) {
	// Original was void, so we assume we don't need the return code
	gi.FS_Read(buffer, len, f);
}

int trap_FS_Write(const void* buffer, int len, fileHandle_t f) {
	return gi.FS_Write(buffer, len, f);
}

int trap_FS_Rename(const char* from, const char* to) {
	// Original returned int, but the struct’s FS_Rename is void.
	// If your engine’s FS_Rename really returns void, you can ignore the return here:
	gi.FS_Rename(from, to);
	return 0;  // Or fix as needed if your engine differs.
}

void trap_FS_FCloseFile(fileHandle_t f) {
	gi.FS_FCloseFile(f);
}

int trap_FS_GetFileList(const char* path, const char* extension, char* listbuf, int bufsize) {
	return gi.FS_GetFileList(path, extension, listbuf, bufsize);
}

void trap_SendConsoleCommand(int exec_when, const char* text) {
	// The struct calls it “Cbuf_ExecuteText”
	gi.Cbuf_ExecuteText(exec_when, text);
}

void trap_Cvar_Register(vmCvar_t* cvar, const char* var_name, const char* value, int flags) {
	gi.Cvar_Register(cvar, var_name, value, flags);
}

void trap_Cvar_Update(vmCvar_t* cvar) {
	gi.Cvar_Update(cvar);
}

void trap_Cvar_Set(const char* var_name, const char* value) {
	gi.Cvar_Set(var_name, value);
}

int trap_Cvar_VariableIntegerValue(const char* var_name) {
	return gi.Cvar_VariableIntegerValue(var_name);
}

void trap_Cvar_VariableStringBuffer(const char* var_name, char* buffer, int bufsize) {
	gi.Cvar_VariableStringBuffer(var_name, buffer, bufsize);
}

void trap_LocateGameData(gentity_t* gEnts, int numGEntities, int sizeofGEntity_t,
	playerState_t* clients, int sizeofGClient) {
	gi.SV_LocateGameData((sharedEntity_t*)gEnts, numGEntities, sizeofGEntity_t,
		clients, sizeofGClient);
}

void trap_DropClient(int clientNum, const char* reason) {
	gi.SV_GameDropClient(clientNum, reason);
}

void trap_SendServerCommand(int clientNum, const char* text) {
	gi.SV_GameSendServerCommand(clientNum, text);
}

void trap_SetConfigstring(int num, const char* string) {
	gi.SV_SetConfigstring(num, string);
}

void trap_GetConfigstring(int num, char* buffer, int bufferSize) {
	gi.SV_GetConfigstring(num, buffer, bufferSize);
}

void trap_GetUserinfo(int num, char* buffer, int bufferSize) {
	gi.SV_GetUserinfo(num, buffer, bufferSize);
}

void trap_SetUserinfo(int num, const char* buffer) {
	gi.SV_SetUserinfo(num, buffer);
}

void trap_GetServerinfo(char* buffer, int bufferSize) {
	gi.SV_GetServerinfo(buffer, bufferSize);
}

void trap_SetBrushModel(gentity_t* ent, const char* name) {
	gi.SV_SetBrushModel((sharedEntity_t*)ent, name);
}

void trap_Trace(trace_t* results, const vec3_t start, const vec3_t mins, const vec3_t maxs,
	const vec3_t end, int passEntityNum, int contentmask) {
	// The struct’s SV_Trace has an extra “int capsule” param
	gi.SV_Trace(results, start, mins, maxs, end, passEntityNum, contentmask, /*capsule=*/0);
}

void trap_TraceCapsule(trace_t* results, const vec3_t start, const vec3_t mins, const vec3_t maxs,
	const vec3_t end, int passEntityNum, int contentmask) {
	// Same call, but with capsule=1
	gi.SV_Trace(results, start, mins, maxs, end, passEntityNum, contentmask, /*capsule=*/1);
}

int trap_PointContents(const vec3_t point, int passEntityNum) {
	return gi.SV_PointContents(point, passEntityNum);
}

qboolean trap_InPVS(const vec3_t p1, const vec3_t p2) {
	return gi.SV_inPVS(p1, p2);
}

qboolean trap_InPVSIgnorePortals(const vec3_t p1, const vec3_t p2) {
	return gi.SV_inPVSIgnorePortals(p1, p2);
}

void trap_AdjustAreaPortalState(gentity_t* ent, qboolean open) {
	gi.SV_AdjustAreaPortalState((sharedEntity_t*)ent, open);
}

qboolean trap_AreasConnected(int area1, int area2) {
	return gi.CM_AreasConnected(area1, area2);
}

void trap_LinkEntity(gentity_t* ent) {
	gi.SV_LinkEntity((sharedEntity_t*)ent);
}

void trap_UnlinkEntity(gentity_t* ent) {
	gi.SV_UnlinkEntity((sharedEntity_t*)ent);
}

int trap_EntitiesInBox(const vec3_t mins, const vec3_t maxs, int* list, int maxcount) {
	return gi.SV_AreaEntities(mins, maxs, list, maxcount);
}

qboolean trap_EntityContact(const vec3_t mins, const vec3_t maxs, const gentity_t* ent) {
	// SV_EntityContact has an extra param “capsule”
	return gi.SV_EntityContact(mins, maxs, (sharedEntity_t*)ent, /*capsule=*/0);
}

qboolean trap_EntityContactCapsule(const vec3_t mins, const vec3_t maxs, const gentity_t* ent) {
	return gi.SV_EntityContact(mins, maxs, (sharedEntity_t*)ent, /*capsule=*/1);
}

int trap_BotAllocateClient(void) {
	return gi.SV_BotAllocateClient();
}

void trap_BotFreeClient(int clientNum) {
	gi.SV_BotFreeClient(clientNum);
}

void trap_GetUsercmd(int clientNum, usercmd_t* cmd) {
	gi.SV_GetUsercmd(clientNum, cmd);
}

qboolean trap_GetEntityToken(char* buffer, int bufferSize) {
	return gi.GetEntityToken(buffer, bufferSize);
}

int trap_DebugPolygonCreate(int color, int numPoints, vec3_t* points) {
	return gi.BotImport_DebugPolygonCreate(color, numPoints, points);
}

void trap_DebugPolygonDelete(int id) {
	gi.BotImport_DebugPolygonDelete(id);
}

int trap_RealTime(qtime_t* qtime) {
	return gi.Com_RealTime(qtime);
}

void trap_SnapVector(float* v) {
	gi.Sys_SnapVector(v);
}

qboolean trap_GetTag(int clientNum, char* tagName, orientation_t* or ) {
	return gi.SV_GetTag(clientNum, tagName, or );
}

//
//  BOTLIB TRAPS
//

int trap_BotLibSetup(void) {
	return gi.SV_BotLibSetup();
}

int trap_BotLibShutdown(void) {
	return gi.SV_BotLibShutdown();
}

int trap_BotLibVarSet(char* var_name, char* value) {
	return gi.BotLibVarSet(var_name, value);
}

int trap_BotLibVarGet(char* var_name, char* value, int size) {
	return gi.BotLibVarGet(var_name, value, size);
}

int trap_BotLibDefine(char* string) {
	return gi.PC_AddGlobalDefine(string);
}

int trap_BotLibStartFrame(float time) {
	return gi.BotLibStartFrame(time);
}

int trap_BotLibLoadMap(const char* mapname) {
	return gi.BotLibLoadMap(mapname);
}

int trap_BotLibUpdateEntity(int ent, void* bue /*bot_entitystate_s*/) {
	return gi.BotLibUpdateEntity(ent, (bot_entitystate_s*) bue);
}

int trap_BotLibTest(int parm0, char* parm1, vec3_t parm2, vec3_t parm3) {
	return gi.BotLibTest(parm0, parm1, parm2, parm3);
}

int trap_BotGetSnapshotEntity(int clientNum, int sequence) {
	return gi.SV_BotGetSnapshotEntity(clientNum, sequence);
}

int trap_BotGetServerCommand(int clientNum, char* message, int size) {
	return gi.SV_BotGetConsoleMessage(clientNum, message, size);
}

void trap_BotUserCommand(int clientNum, usercmd_t* ucmd) {
	gi.SV_ClientThink(clientNum, ucmd);
}

//
// AAS
//
void trap_AAS_EntityInfo(int entnum, void* info /*aas_entityinfo_s*/) {
	gi.AAS_EntityInfo(entnum, (aas_entityinfo_s*)info);
}

int trap_AAS_Initialized(void) {
	return gi.AAS_Initialized();
}

void trap_AAS_PresenceTypeBoundingBox(int presencetype, vec3_t mins, vec3_t maxs) {
	gi.AAS_PresenceTypeBoundingBox(presencetype, mins, maxs);
}

float trap_AAS_Time(void) {
	return gi.AAS_Time();
}

void trap_AAS_SetCurrentWorld(int index) {
	gi.AAS_SetCurrentWorld(index);
}

int trap_AAS_PointAreaNum(vec3_t point) {
	return gi.AAS_PointAreaNum(point);
}

int trap_AAS_TraceAreas(vec3_t start, vec3_t end, int* areas, vec3_t* points, int maxareas) {
	return gi.AAS_TraceAreas(start, end, areas, points, maxareas);
}

int trap_AAS_PointContents(vec3_t point) {
	return gi.AAS_PointContents(point);
}

int trap_AAS_NextBSPEntity(int ent) {
	return gi.AAS_NextBSPEntity(ent);
}

int trap_AAS_ValueForBSPEpairKey(int ent, char* key, char* value, int size) {
	return gi.AAS_ValueForBSPEpairKey(ent, key, value, size);
}

int trap_AAS_VectorForBSPEpairKey(int ent, char* key, vec3_t v) {
	return gi.AAS_VectorForBSPEpairKey(ent, key, v);
}

int trap_AAS_FloatForBSPEpairKey(int ent, char* key, float* value) {
	return gi.AAS_FloatForBSPEpairKey(ent, key, value);
}

int trap_AAS_IntForBSPEpairKey(int ent, char* key, int* value) {
	return gi.AAS_IntForBSPEpairKey(ent, key, value);
}

int trap_AAS_AreaReachability(int areanum) {
	return gi.AAS_AreaReachability(areanum);
}

int trap_AAS_AreaTravelTimeToGoalArea(int areanum, vec3_t origin, int goalareanum, int travelflags) {
	return gi.AAS_AreaTravelTimeToGoalArea(areanum, origin, goalareanum, travelflags);
}

int trap_AAS_Swimming(vec3_t origin) {
	return gi.AAS_Swimming(origin);
}

int trap_AAS_PredictClientMovement(void* move /**/, int entnum, vec3_t origin,
	int presencetype, int onground, vec3_t velocity, vec3_t cmdmove,
	int cmdframes, int maxframes, float frametime,
	int stopevent, int stopareanum, int visualize) {
	return gi.AAS_PredictClientMovement((aas_clientmove_s *)move, entnum, origin, presencetype, onground,
		velocity, cmdmove, cmdframes, maxframes,
		frametime, stopevent, stopareanum, visualize);
}

//
// Route-tables
//
void trap_AAS_RT_ShowRoute(vec3_t srcpos, int srcnum, int destnum) {
	gi.AAS_RT_ShowRoute(srcpos, srcnum, destnum);
}

qboolean trap_AAS_RT_GetHidePos(vec3_t srcpos, int srcnum, int srcarea,
	vec3_t destpos, int destnum, int destarea,
	vec3_t returnPos) {
	return gi.AAS_RT_GetHidePos(srcpos, srcnum, srcarea, destpos, destnum, destarea, returnPos);
}

int trap_AAS_FindAttackSpotWithinRange(int srcnum, int rangenum, int enemynum, float rangedist, int travelflags, float* outpos) {
	return gi.AAS_FindAttackSpotWithinRange(srcnum, rangenum, enemynum, rangedist, travelflags, outpos);
}

void trap_AAS_SetAASBlockingEntity(vec3_t absmin, vec3_t absmax, qboolean blocking) {
	gi.AAS_SetAASBlockingEntity(absmin, absmax, blocking);
}

//
// EA_ (external AI) calls
//
void trap_EA_Say(int client, char* str) {
	gi.EA_Say(client, str);
}

void trap_EA_SayTeam(int client, char* str) {
	gi.EA_SayTeam(client, str);
}

void trap_EA_UseItem(int client, char* it) {
	gi.EA_UseItem(client, it);
}

void trap_EA_DropItem(int client, char* it) {
	gi.EA_DropItem(client, it);
}

void trap_EA_UseInv(int client, char* inv) {
	gi.EA_UseInv(client, inv);
}

void trap_EA_DropInv(int client, char* inv) {
	gi.EA_DropInv(client, inv);
}

void trap_EA_Gesture(int client) {
	gi.EA_Gesture(client);
}

void trap_EA_Command(int client, char* command) {
	gi.EA_Command(client, command);
}

void trap_EA_SelectWeapon(int client, int weapon) {
	gi.EA_SelectWeapon(client, weapon);
}

void trap_EA_Talk(int client) {
	gi.EA_Talk(client);
}

void trap_EA_Attack(int client) {
	gi.EA_Attack(client);
}

void trap_EA_Reload(int client) {
	gi.EA_Reload(client);
}

void trap_EA_Use(int client) {
	gi.EA_Use(client);
}

void trap_EA_Respawn(int client) {
	gi.EA_Respawn(client);
}

void trap_EA_Jump(int client) {
	gi.EA_Jump(client);
}

void trap_EA_DelayedJump(int client) {
	gi.EA_DelayedJump(client);
}

void trap_EA_Crouch(int client) {
	gi.EA_Crouch(client);
}

void trap_EA_MoveUp(int client) {
	gi.EA_MoveUp(client);
}

void trap_EA_MoveDown(int client) {
	gi.EA_MoveDown(client);
}

void trap_EA_MoveForward(int client) {
	gi.EA_MoveForward(client);
}

void trap_EA_MoveBack(int client) {
	gi.EA_MoveBack(client);
}

void trap_EA_MoveLeft(int client) {
	gi.EA_MoveLeft(client);
}

void trap_EA_MoveRight(int client) {
	gi.EA_MoveRight(client);
}

void trap_EA_Move(int client, vec3_t dir, float speed) {
	gi.EA_Move(client, dir, speed);
}

void trap_EA_View(int client, vec3_t viewangles) {
	gi.EA_View(client, viewangles);
}

void trap_EA_EndRegular(int client, float thinktime) {
	gi.EA_EndRegular(client, thinktime);
}

void trap_EA_GetInput(int client, float thinktime, void* input) {
	gi.EA_GetInput(client, thinktime, (bot_input_t*)input);
}

void trap_EA_ResetInput(int client, void* init) {
	gi.EA_ResetInput(client, (bot_input_t *)init);
}

//
// Bot Character / Chat
//
int trap_BotLoadCharacter(char* charfile, int skill) {
	return gi.BotLoadCharacter(charfile, skill);
}

void trap_BotFreeCharacter(int character) {
	gi.BotFreeCharacter(character);
}

float trap_Characteristic_Float(int character, int index) {
	return gi.Characteristic_Float(character, index);
}

float trap_Characteristic_BFloat(int character, int index, float min, float max) {
	return gi.Characteristic_BFloat(character, index, min, max);
}

int trap_Characteristic_Integer(int character, int index) {
	return gi.Characteristic_Integer(character, index);
}

int trap_Characteristic_BInteger(int character, int index, int min, int max) {
	return gi.Characteristic_BInteger(character, index, min, max);
}

void trap_Characteristic_String(int character, int index, char* buf, int size) {
	gi.Characteristic_String(character, index, buf, size);
}

int trap_BotAllocChatState(void) {
	return gi.BotAllocChatState();
}

void trap_BotFreeChatState(int handle) {
	gi.BotFreeChatState(handle);
}

void trap_BotQueueConsoleMessage(int chatstate, int type, char* message) {
	gi.BotQueueConsoleMessage(chatstate, type, message);
}

void trap_BotRemoveConsoleMessage(int chatstate, int handle) {
	gi.BotRemoveConsoleMessage(chatstate, handle);
}

int trap_BotNextConsoleMessage(int chatstate, void* cm) {
	return gi.BotNextConsoleMessage(chatstate, (bot_consolemessage_s *)cm);
}

int trap_BotNumConsoleMessages(int chatstate) {
	return gi.BotNumConsoleMessages(chatstate);
}

void trap_BotInitialChat(int chatstate, char* type, int mcontext,
	char* var0, char* var1, char* var2, char* var3,
	char* var4, char* var5, char* var6, char* var7) {
	gi.BotInitialChat(chatstate, type, mcontext,
		var0, var1, var2, var3,
		var4, var5, var6, var7);
}

int trap_BotNumInitialChats(int chatstate, char* type) {
	return gi.BotNumInitialChats(chatstate, type);
}

int trap_BotReplyChat(int chatstate, char* message, int mcontext, int vcontext,
	char* var0, char* var1, char* var2, char* var3,
	char* var4, char* var5, char* var6, char* var7) {
	return gi.BotReplyChat(chatstate, message, mcontext, vcontext,
		var0, var1, var2, var3,
		var4, var5, var6, var7);
}

int trap_BotChatLength(int chatstate) {
	return gi.BotChatLength(chatstate);
}

void trap_BotEnterChat(int chatstate, int client, int sendto) {
	gi.BotEnterChat(chatstate, client, sendto);
}

void trap_BotGetChatMessage(int chatstate, char* buf, int size) {
	gi.BotGetChatMessage(chatstate, buf, size);
}

int trap_StringContains(char* str1, char* str2, int casesensitive) {
	return gi.StringContains(str1, str2, casesensitive);
}

int trap_BotFindMatch(char* str, void* match, unsigned long int context) {
	return gi.BotFindMatch(str, (bot_match_s*)match, context);
}

void trap_BotMatchVariable(void* match, int variable, char* buf, int size) {
	gi.BotMatchVariable((bot_match_s *)match, variable, buf, size);
}

void trap_UnifyWhiteSpaces(char* string) {
	gi.UnifyWhiteSpaces(string);
}

void trap_BotReplaceSynonyms(char* string, unsigned long int context) {
	gi.BotReplaceSynonyms(string, context);
}

int trap_BotLoadChatFile(int chatstate, char* chatfile, char* chatname) {
	return gi.BotLoadChatFile(chatstate, chatfile, chatname);
}

void trap_BotSetChatGender(int chatstate, int gender) {
	gi.BotSetChatGender(chatstate, gender);
}

void trap_BotSetChatName(int chatstate, char* name) {
	gi.BotSetChatName(chatstate, name);
}

//
// AI Goals
//
void trap_BotResetGoalState(int goalstate) {
	gi.BotResetGoalState(goalstate);
}

void trap_BotResetAvoidGoals(int goalstate) {
	gi.BotResetAvoidGoals(goalstate);
}

void trap_BotRemoveFromAvoidGoals(int goalstate, int number) {
	gi.BotRemoveFromAvoidGoals(goalstate, number);
}

void trap_BotPushGoal(int goalstate, void* goal) {
	gi.BotPushGoal(goalstate, (bot_goal_s*)goal);
}

void trap_BotPopGoal(int goalstate) {
	gi.BotPopGoal(goalstate);
}

void trap_BotEmptyGoalStack(int goalstate) {
	gi.BotEmptyGoalStack(goalstate);
}

void trap_BotDumpAvoidGoals(int goalstate) {
	gi.BotDumpAvoidGoals(goalstate);
}

void trap_BotDumpGoalStack(int goalstate) {
	gi.BotDumpGoalStack(goalstate);
}

void trap_BotGoalName(int number, char* name, int size) {
	gi.BotGoalName(number, name, size);
}

int trap_BotGetTopGoal(int goalstate, void* goal) {
	return gi.BotGetTopGoal(goalstate, (bot_goal_s*)goal);
}

int trap_BotGetSecondGoal(int goalstate, void* goal) {
	return gi.BotGetSecondGoal(goalstate, (bot_goal_s*)goal);
}

int trap_BotChooseLTGItem(int goalstate, vec3_t origin, int* inventory, int travelflags) {
	return gi.BotChooseLTGItem(goalstate, origin, inventory, travelflags);
}

int trap_BotChooseNBGItem(int goalstate, vec3_t origin, int* inventory, int travelflags, void* ltg, float maxtime) {
	return gi.BotChooseNBGItem(goalstate, origin, inventory, travelflags, (bot_goal_s*)ltg, maxtime);
}

int trap_BotTouchingGoal(vec3_t origin, void* goal) {
	return gi.BotTouchingGoal(origin, (bot_goal_s*)goal);
}

int trap_BotItemGoalInVisButNotVisible(int viewer, vec3_t eye, vec3_t viewangles, void* goal) {
	return gi.BotItemGoalInVisButNotVisible(viewer, eye, viewangles, (bot_goal_s*)goal);
}

int trap_BotGetLevelItemGoal(int index, char* classname, void* goal) {
	return gi.BotGetLevelItemGoal(index, classname, (bot_goal_s*)goal);
}

int trap_BotGetNextCampSpotGoal(int num, void* goal) {
	return gi.BotGetNextCampSpotGoal(num, (bot_goal_s*)goal);
}

int trap_BotGetMapLocationGoal(char* name, void* goal) {
	return gi.BotGetMapLocationGoal(name, (bot_goal_s*)goal);
}

float trap_BotAvoidGoalTime(int goalstate, int number) {
	return gi.BotAvoidGoalTime(goalstate, number);
}

void trap_BotInitLevelItems(void) {
	gi.BotInitLevelItems();
}

void trap_BotUpdateEntityItems(void) {
	gi.BotUpdateEntityItems();
}

int trap_BotLoadItemWeights(int goalstate, char* filename) {
	return gi.BotLoadItemWeights(goalstate, filename);
}

void trap_BotFreeItemWeights(int goalstate) {
	gi.BotFreeItemWeights(goalstate);
}

void trap_BotInterbreedGoalFuzzyLogic(int parent1, int parent2, int child) {
	gi.BotInterbreedGoalFuzzyLogic(parent1, parent2, child);
}

void trap_BotSaveGoalFuzzyLogic(int goalstate, char* filename) {
	gi.BotSaveGoalFuzzyLogic(goalstate, filename);
}

void trap_BotMutateGoalFuzzyLogic(int goalstate, float range) {
	gi.BotMutateGoalFuzzyLogic(goalstate, range);
}

int trap_BotAllocGoalState(int state) {
	return gi.BotAllocGoalState(state);
}

void trap_BotFreeGoalState(int handle) {
	gi.BotFreeGoalState(handle);
}

//
// AI Movement
//
void trap_BotResetMoveState(int movestate) {
	gi.BotResetMoveState(movestate);
}

void trap_BotMoveToGoal(void* result, int movestate, void* goal, int travelflags) {
	gi.BotMoveToGoal((bot_moveresult_s *)result, movestate, (bot_goal_s*)goal, travelflags);
}

int trap_BotMoveInDirection(int movestate, vec3_t dir, float speed, int type) {
	return gi.BotMoveInDirection(movestate, dir, speed, type);
}

void trap_BotResetAvoidReach(int movestate) {
	gi.BotResetAvoidReach(movestate);
}

void trap_BotResetLastAvoidReach(int movestate) {
	gi.BotResetLastAvoidReach(movestate);
}

int trap_BotReachabilityArea(vec3_t origin, int testground) {
	return gi.BotReachabilityArea(origin, testground);
}

int trap_BotMovementViewTarget(int movestate, void* goal, int travelflags, float lookahead, vec3_t target) {
	return gi.BotMovementViewTarget(movestate, (bot_goal_s*)goal, travelflags, lookahead, target);
}

int trap_BotPredictVisiblePosition(vec3_t origin, int areanum, void* goal, int travelflags, vec3_t target) {
	return gi.BotPredictVisiblePosition(origin, areanum, (bot_goal_s *)goal, travelflags, target);
}

int trap_BotAllocMoveState(void) {
	return gi.BotAllocMoveState();
}

void trap_BotFreeMoveState(int handle) {
	gi.BotFreeMoveState(handle);
}

void trap_BotInitMoveState(int handle, void* initmove) {
	gi.BotInitMoveState(handle, (bot_initmove_s *)initmove);
}

void trap_BotInitAvoidReach(int handle) {
	gi.BotInitAvoidReach(handle);
}

//
// AI Weapons
//
int trap_BotChooseBestFightWeapon(int weaponstate, int* inventory) {
	return gi.BotChooseBestFightWeapon(weaponstate, inventory);
}

void trap_BotGetWeaponInfo(int weaponstate, int weapon, void* weaponinfo) {
	gi.BotGetWeaponInfo(weaponstate, weapon, (weaponinfo_s *)weaponinfo);
}

int trap_BotLoadWeaponWeights(int weaponstate, char* filename) {
	return gi.BotLoadWeaponWeights(weaponstate, filename);
}

int trap_BotAllocWeaponState(void) {
	return gi.BotAllocWeaponState();
}

void trap_BotFreeWeaponState(int weaponstate) {
	gi.BotFreeWeaponState(weaponstate);
}

void trap_BotResetWeaponState(int weaponstate) {
	gi.BotResetWeaponState(weaponstate);
}

int trap_GeneticParentsAndChildSelection(int numranks, float* ranks,
	int* parent1, int* parent2, int* child) {
	return gi.GeneticParentsAndChildSelection(numranks, ranks, parent1, parent2, child);
}
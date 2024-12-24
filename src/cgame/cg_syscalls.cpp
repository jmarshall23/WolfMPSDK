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

// cg_syscalls.c -- this file is only included when building a dll
// cg_syscalls.asm is included instead when building a qvm
#include "cg_local.h"

cgameImport_t cgi;

void CG_Init(int serverMessageNum, int serverCommandSequence, int clientNum);
void CG_Shutdown(void);

cGameExport_t *dllEntry(cgameImport_t *imports) {
	static cGameExport_t cge;

	cge.Init = CG_Init;
	cge.Shutdown = CG_Shutdown;
	cge.ConsoleCommand = CG_ConsoleCommand;
	cge.DrawActiveFrame = CG_DrawActiveFrame;
	cge.CrosshairPlayer = CG_CrosshairPlayer;
	cge.LastAttacker = CG_LastAttacker;
	cge.KeyEvent = CG_KeyEvent;
	cge.MouseEvent = CG_MouseEvent;
	cge.EventHandling = CG_EventHandling;
	cge.GetTag = CG_GetTag;
	cge.CheckCenterView = CG_CheckCenterView;

	cgi = *imports;

	return &cge;
}

//-------------------------------------------------
// Print / Error / Timing
//-------------------------------------------------
void trap_Print(const char* fmt) {
	cgi.Com_Printf("%s", fmt);
}

void trap_Error(const char* fmt) {
	// Many mods call Com_Error with ERR_DROP, but adapt if needed
	cgi.Com_Error(ERR_DROP, "%s", fmt);
}

int trap_Milliseconds(void) {
	return cgi.Sys_Milliseconds();
}

//-------------------------------------------------
// Cvars
//-------------------------------------------------
void trap_Cvar_Register(vmCvar_t* vmCvar, const char* varName, const char* defaultValue, int flags) {
	cgi.Cvar_Register(vmCvar, varName, defaultValue, flags);
}

void trap_Cvar_Update(vmCvar_t* vmCvar) {
	cgi.Cvar_Update(vmCvar);
}

void trap_Cvar_Set(const char* varName, const char* value) {
	cgi.Cvar_Set(varName, value);
}

void trap_Cvar_VariableStringBuffer(const char* varName, char* buffer, int bufsize) {
	cgi.Cvar_VariableStringBuffer(varName, buffer, bufsize);
}

//-------------------------------------------------
// Commands
//-------------------------------------------------
int trap_Argc(void) {
	return cgi.Cmd_Argc();
}

void trap_Argv(int n, char* buffer, int bufferLength) {
	cgi.Cmd_ArgvBuffer(n, buffer, bufferLength);
}

void trap_Args(char* buffer, int bufferLength) {
	cgi.Cmd_ArgsBuffer(buffer, bufferLength);
}

//-------------------------------------------------
// File system
//-------------------------------------------------
int trap_FS_FOpenFile(const char* qpath, fileHandle_t* f, fsMode_t mode) {
	return cgi.FS_FOpenFileByMode(qpath, f, mode);
}

void trap_FS_Read(void* buffer, int len, fileHandle_t f) {
	cgi.FS_Read(buffer, len, f);
}

void trap_FS_Write(const void* buffer, int len, fileHandle_t f) {
	cgi.FS_Write(buffer, len, f);
}

void trap_FS_FCloseFile(fileHandle_t f) {
	cgi.FS_FCloseFile(f);
}

//-------------------------------------------------
// Console command buffers
//-------------------------------------------------
void trap_SendConsoleCommand(const char* text) {
	cgi.Cbuf_AddText(text);
}

//-------------------------------------------------
// Add/remove console commands
//-------------------------------------------------
void trap_AddCommand(const char* cmdName) {
	cgi.CL_AddCgameCommand(cmdName);
}

void trap_SendClientCommand(const char* s) {
	cgi.CL_AddReliableCommand(s);
}

//-------------------------------------------------
// Screen
//-------------------------------------------------
void trap_UpdateScreen(void) {
	cgi.SCR_UpdateScreen();
}

//-------------------------------------------------
// Collision Map
//-------------------------------------------------
void trap_CM_LoadMap(const char* mapname) {
	cgi.CL_CM_LoadMap(mapname);
}

int trap_CM_NumInlineModels(void) {
	return cgi.CM_NumInlineModels();
}

clipHandle_t trap_CM_InlineModel(int index) {
	return cgi.CM_InlineModel(index);
}

clipHandle_t trap_CM_TempBoxModel(const vec3_t mins, const vec3_t maxs) {
	return cgi.CM_TempBoxModel(mins, maxs, qfalse);
}

clipHandle_t trap_CM_TempCapsuleModel(const vec3_t mins, const vec3_t maxs) {
	return cgi.CM_TempBoxModel(mins, maxs, qtrue);
}

int trap_CM_PointContents(const vec3_t p, clipHandle_t model) {
	return cgi.CM_PointContents(p, model);
}

int trap_CM_TransformedPointContents(const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles) {
	return cgi.CM_TransformedPointContents(p, model, origin, angles);
}

void trap_CM_BoxTrace(trace_t* results, const vec3_t start, const vec3_t end,
	const vec3_t mins, const vec3_t maxs,
	clipHandle_t model, int brushmask) {
	cgi.CM_BoxTrace(results, start, end, mins, maxs, model, brushmask, qfalse);
}

void trap_CM_TransformedBoxTrace(trace_t* results, const vec3_t start, const vec3_t end,
	const vec3_t mins, const vec3_t maxs,
	clipHandle_t model, int brushmask,
	const vec3_t origin, const vec3_t angles) {
	cgi.CM_TransformedBoxTrace(results, start, end, mins, maxs, model, brushmask, origin, angles, qfalse);
}

void trap_CM_CapsuleTrace(trace_t* results, const vec3_t start, const vec3_t end,
	const vec3_t mins, const vec3_t maxs,
	clipHandle_t model, int brushmask) {
	cgi.CM_BoxTrace(results, start, end, mins, maxs, model, brushmask, qtrue);
}

void trap_CM_TransformedCapsuleTrace(trace_t* results, const vec3_t start, const vec3_t end,
	const vec3_t mins, const vec3_t maxs,
	clipHandle_t model, int brushmask,
	const vec3_t origin, const vec3_t angles) {
	cgi.CM_TransformedBoxTrace(results, start, end, mins, maxs, model, brushmask, origin, angles, qtrue);
}

int trap_CM_MarkFragments(int numPoints, const vec3_t* points,
	const vec3_t projection,
	int maxPoints, vec3_t pointBuffer,
	int maxFragments, markFragment_t* fragmentBuffer) {
	return cgi.re_MarkFragments(
		numPoints, points, projection,
		maxPoints, pointBuffer, 
		maxFragments, fragmentBuffer
	);
}

//-------------------------------------------------
// Sound
//-------------------------------------------------
void trap_S_StartSound(vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx) {
	cgi.S_StartSound(origin, entityNum, entchannel, sfx);
}

// (SA) added
void trap_S_StartSoundEx(vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx, int flags) {
	cgi.S_StartSoundEx(origin, entityNum, entchannel, sfx, flags);
}

void trap_S_StartLocalSound(sfxHandle_t sfx, int channelNum) {
	cgi.S_StartLocalSound(sfx, channelNum);
}

void trap_S_ClearLoopingSounds(qboolean killall) {
	// The original cgame code might ignore killall or handle differently
	// For example:
	if (killall) {
		// Possibly do something extra
	}
	cgi.S_ClearLoopingSounds();
}

void trap_S_AddLoopingSound(int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx, int volume) {
	// (SA) had a “range” param in RTCW. Adjust as your code needs:
	cgi.S_AddLoopingSound(entityNum, origin, velocity, volume, sfx, 255 /* volume if needed */);
}

void trap_S_AddRangedLoopingSound(int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx, int range) {
	cgi.S_AddLoopingSound(entityNum, origin, velocity, range, sfx, 255);
}

void trap_S_AddRealLoopingSound(int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx) {
	cgi.S_AddLoopingSound(entityNum, origin, velocity, 1250 /* range */, sfx, 255);
}

void trap_S_StopLoopingSound(int entityNum) {
	// Depending on the engine, S_StopLoopingSound might not exist
	// If it does, you can call that. If not, do nothing or revert.
}

void trap_S_UpdateEntityPosition(int entityNum, const vec3_t origin) {
	cgi.S_UpdateEntityPosition(entityNum, origin);
}

int trap_S_GetVoiceAmplitude(int entityNum) {
	return cgi.S_GetVoiceAmplitude(entityNum);
}

void trap_S_Respatialize(int entityNum, const vec3_t origin, vec3_t axis[3], int inwater) {
	cgi.S_Respatialize(entityNum, origin, axis, inwater);
}

sfxHandle_t trap_S_RegisterSound(const char* sample) {
	// Some mods call CG_DrawInformation() during loading, etc.  
	// If you need that, call it here or remove it as needed.
	// CG_DrawInformation();

	return cgi.S_RegisterSound(sample, qfalse /* or qtrue if you want precache */);
}

void trap_S_StartBackgroundTrack(const char* intro, const char* loop) {
	cgi.S_StartBackgroundTrack(intro, loop);
}

void trap_S_StartStreamingSound(const char* intro, const char* loop, int entnum, int channel, int attenuation) {
	cgi.S_StartStreamingSound(intro, loop, entnum, channel, attenuation);
}

//-------------------------------------------------
// Renderer: map, model, shaders
//-------------------------------------------------
void trap_R_LoadWorldMap(const char* mapname) {
	cgi.re_LoadWorld(mapname);
}

qhandle_t trap_R_RegisterModel(const char* name) {
	// CG_DrawInformation(); // If you want a loading screen, do it here
	return cgi.re_RegisterModel(name);
}

//----(SA) added
qboolean trap_R_GetSkinModel(qhandle_t skinid, const char* type, char* name) {
	return cgi.re_GetSkinModel(skinid, type, name);
}

qhandle_t trap_R_GetShaderFromModel(qhandle_t modelid, int surfnum, int withlightmap) {
	return cgi.re_GetShaderFromModel(modelid, surfnum, withlightmap);
}
//----(SA) end

qhandle_t trap_R_RegisterSkin(const char* name) {
	// CG_DrawInformation();
	return cgi.re_RegisterSkin(name);
}

qhandle_t trap_R_RegisterShader(const char* name) {
	// CG_DrawInformation();
	return cgi.re_RegisterShader(name);
}

qhandle_t trap_R_RegisterShaderNoMip(const char* name) {
	// CG_DrawInformation();
	return cgi.re_RegisterShaderNoMip(name);
}

void trap_R_RegisterFont(const char* fontName, int pointSize, fontInfo_t* font) {
	cgi.re_RegisterFont(fontName, pointSize, font);
}

//-------------------------------------------------
// Renderer: scene building
//-------------------------------------------------
void trap_R_ClearScene(void) {
	cgi.re_ClearScene();
}

void trap_R_AddRefEntityToScene(const refEntity_t* re) {
	cgi.re_AddRefEntityToScene(re);
}

void trap_R_AddPolyToScene(qhandle_t hShader, int numVerts, const polyVert_t* verts) {
	cgi.re_AddPolyToScene(hShader, numVerts, verts);
}

void trap_R_AddPolysToScene(qhandle_t hShader, int numVerts, const polyVert_t* verts, int numPolys) {
	cgi.re_AddPolysToScene(hShader, numVerts, verts, numPolys);
}

void trap_R_AddLightToScene(const vec3_t org, float intensity, float r, float g, float b, int overdraw) {
	cgi.re_AddLightToScene(org, intensity, r, g, b, overdraw);
}

void trap_R_AddCoronaToScene(const vec3_t org, float r, float g, float b, float scale, int id, qboolean visible) {
	cgi.re_AddCoronaToScene(org, r, g, b, scale, id, visible);
}

void trap_R_SetFog(int fogvar, int var1, int var2, float r, float g, float b, float density) {
	cgi.re_SetFog(fogvar, var1, var2, r, g, b, density);
}

void trap_R_RenderScene(const refdef_t* fd) {
	cgi.re_RenderScene(fd);
}

//-------------------------------------------------
// Renderer: 2D drawing
//-------------------------------------------------
void trap_R_SetColor(const float* rgba) {
	cgi.re_SetColor(rgba);
}

void trap_R_DrawStretchPic(float x, float y, float w, float h,
	float s1, float t1, float s2, float t2,
	qhandle_t hShader) {
	cgi.re_DrawStretchPic(x, y, w, h, s1, t1, s2, t2, hShader);
}

void trap_R_DrawRotatedPic(float x, float y, float w, float h,
	float s1, float t1, float s2, float t2,
	qhandle_t hShader, float angle) {
	cgi.re_DrawRotatedPic(x, y, w, h, s1, t1, s2, t2, hShader, angle);
}

void trap_R_DrawStretchPicGradient(float x, float y, float w, float h,
	float s1, float t1, float s2, float t2,
	qhandle_t hShader,
	const float* gradientColor, int gradientType) {
	cgi.re_DrawStretchPicGradient(x, y, w, h, s1, t1, s2, t2, hShader, gradientColor, gradientType);
}

void trap_R_ModelBounds(clipHandle_t model, vec3_t mins, vec3_t maxs) {
	cgi.re_ModelBounds(model, mins, maxs);
}

int trap_R_LerpTag(orientation_t* tag, const refEntity_t* refent, const char* tagName, int startIndex) {
	// The signature in your code may differ; adapt if needed
	return cgi.re_LerpTag(tag, refent, tagName, startIndex);
}

void trap_R_RemapShader(const char* oldShader, const char* newShader, const char* timeOffset) {
	cgi.re_RemapShader(oldShader, newShader, timeOffset);
}

//-------------------------------------------------
// CL / snapshot / usercmd
//-------------------------------------------------
void trap_GetGlconfig(glconfig_t* glconfig) {
	cgi.CL_GetGlconfig(glconfig);
}

void trap_GetGameState(gameState_t* gamestate) {
	cgi.CL_GetGameState(gamestate);
}

void trap_GetCurrentSnapshotNumber(int* snapshotNumber, int* serverTime) {
	cgi.CL_GetCurrentSnapshotNumber(snapshotNumber, serverTime);
}

qboolean trap_GetSnapshot(int snapshotNumber, snapshot_t* snapshot) {
	return cgi.CL_GetSnapshot(snapshotNumber, snapshot);
}

qboolean trap_GetServerCommand(int serverCommandNumber) {
	return (qboolean)cgi.CL_GetServerCommand(serverCommandNumber);
}

int trap_GetCurrentCmdNumber(void) {
	return cgi.CL_GetCurrentCmdNumber();
}

qboolean trap_GetUserCmd(int cmdNumber, usercmd_t* ucmd) {
	return cgi.CL_GetUserCmd(cmdNumber, ucmd);
}

void trap_SetUserCmdValue(int stateValue, int holdableValue, float sensitivityScale, int mpSetup, int mpIdentClient) {
	cgi.CL_SetUserCmdValue(stateValue, holdableValue, sensitivityScale, mpSetup, mpIdentClient);
}

void trap_SetClientLerpOrigin(float x, float y, float z) {
	cgi.CL_SetClientLerpOrigin(x, y, z);
}

//-------------------------------------------------
// Misc
//-------------------------------------------------
void testPrintInt(char* string, int i) {

}

void testPrintFloat(char* string, float f) {

}

int trap_MemoryRemaining(void) {
	return cgi.Hunk_MemoryRemaining();
}

qboolean trap_loadCamera(int camNum, const char* name) {
	return cgi.loadCamera(camNum, name);
}

void trap_startCamera(int camNum, int time) {
	cgi.startCamera(camNum, time);
}

qboolean trap_getCameraInfo(int camNum, int time, vec3_t* origin, vec3_t* angles, float* fov) {
	return cgi.getCameraInfo(camNum, time, (float *) origin, (float*)angles, fov);
}

//-------------------------------------------------
// Keys
//-------------------------------------------------
qboolean trap_Key_IsDown(int keynum) {
	return cgi.Key_IsDown(keynum);
}

int trap_Key_GetCatcher(void) {
	return cgi.Key_GetCatcher();
}

void trap_Key_SetCatcher(int catcher) {
	cgi.Key_SetCatcher(catcher);
}

int trap_Key_GetKey(const char* binding) {
	return cgi.Key_GetKey(binding);
}

//-------------------------------------------------
// Botlib PC_*
//-------------------------------------------------
int trap_PC_AddGlobalDefine(char* define) {
	return cgi.PC_AddGlobalDefine(define);
}

int trap_PC_LoadSource(const char* filename) {
	return cgi.PC_LoadSourceHandle(filename);
}

int trap_PC_FreeSource(int handle) {
	return cgi.PC_FreeSourceHandle(handle);
}

int trap_PC_ReadToken(int handle, pc_token_t* pc_token) {
	return cgi.PC_ReadTokenHandle(handle, pc_token);
}

int trap_PC_SourceFileAndLine(int handle, char* filename, int* line) {
	return cgi.PC_SourceFileAndLine(handle, filename, line);
}

//-------------------------------------------------
// More sound
//-------------------------------------------------
void trap_S_StopBackgroundTrack(void) {
	cgi.S_StopBackgroundTrack();
}

//-------------------------------------------------
// Time, math
//-------------------------------------------------
int trap_RealTime(qtime_t* qtime) {
	return cgi.Com_RealTime(qtime);
}

void trap_SnapVector(float* v) {
	cgi.Sys_SnapVector(v);
}

//-------------------------------------------------
// Server <-> Game bridging
//-------------------------------------------------
void trap_SendMoveSpeedsToGame(int entnum, char* movespeeds) {
	cgi.SV_SendMoveSpeedsToGame(entnum, movespeeds);
}

//-------------------------------------------------
// Cinematics
//-------------------------------------------------
int trap_CIN_PlayCinematic(const char* arg0, int xpos, int ypos, int width, int height, int bits) {
	return cgi.CIN_PlayCinematic(arg0, xpos, ypos, width, height, bits);
}

e_status trap_CIN_StopCinematic(int handle) {
	return (e_status)cgi.CIN_StopCinematic(handle);
}

e_status trap_CIN_RunCinematic(int handle) {
	return (e_status)cgi.CIN_RunCinematic(handle);
}

void trap_CIN_DrawCinematic(int handle) {
	cgi.CIN_DrawCinematic(handle);
}

void trap_CIN_SetExtents(int handle, int x, int y, int w, int h) {
	cgi.CIN_SetExtents(handle, x, y, w, h);
}

//-------------------------------------------------
// UI / In-game popups
//-------------------------------------------------
qboolean trap_GetEntityToken(char* buffer, int bufferSize) {
	return cgi.re_GetEntityToken(buffer, bufferSize);
}

//----(SA) add a popup
void trap_UI_Popup(const char* arg0) {
	cgi.InGame_Popup(arg0);
}

void trap_UI_LimboChat(const char* arg0) {
	cgi.CL_AddToLimboChat(arg0);
}

void trap_UI_ClosePopup(const char* arg0) {
	cgi.InGame_ClosePopup(arg0);
}

//-------------------------------------------------
// Keys and binding strings
//-------------------------------------------------
void trap_Key_GetBindingBuf(int keynum, char* buf, int buflen) {
	cgi.Key_GetBindingBuf(keynum, buf, buflen);
}

void trap_Key_SetBinding(int keynum, const char* binding) {
	cgi.Key_SetBinding(keynum, binding);
}

void trap_Key_KeynumToStringBuf(int keynum, char* buf, int buflen) {
	cgi.Key_KeynumToStringBuf(keynum, buf, buflen);
}

//-------------------------------------------------
// Translation
//-------------------------------------------------
#define MAX_VA_STRING       32000
char* trap_TranslateString(const char* string) {
	static char staticbuf[2][MAX_VA_STRING];
	static int bufcount = 0;

	char* buf = staticbuf[bufcount++ % 2];
	cgi.CL_TranslateString(string, buf);

	return buf;
}
// -NERVE - SMF

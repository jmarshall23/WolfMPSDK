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



#define CMD_BACKUP          64
#define CMD_MASK            ( CMD_BACKUP - 1 )
// allow a lot of command backups for very fast systems
// multiple commands may be combined into a single packet, so this
// needs to be larger than PACKET_BACKUP


#define MAX_ENTITIES_IN_SNAPSHOT    256

// snapshots are a view of the server at a given time

// Snapshots are generated at regular time intervals by the server,
// but they may not be sent if a client's rate level is exceeded, or
// they may be dropped by the network.
typedef struct {
	int snapFlags;                      // SNAPFLAG_RATE_DELAYED, etc
	int ping;

	int serverTime;                 // server time the message is valid for (in msec)

	byte areamask[MAX_MAP_AREA_BYTES];                  // portalarea visibility bits

	playerState_t ps;                       // complete information about the current player at this time

	int numEntities;                        // all of the entities that need to be presented
	entityState_t entities[MAX_ENTITIES_IN_SNAPSHOT];   // at the time of this snapshot

	int numServerCommands;                  // text based server commands to execute when this
	int serverCommandSequence;              // snapshot becomes current
} snapshot_t;

enum {
	CGAME_EVENT_NONE,
	CGAME_EVENT_TEAMMENU,
	CGAME_EVENT_SCOREBOARD,
	CGAME_EVENT_EDITHUD
};


/*
==================================================================

functions imported from the main executable

==================================================================
*/

#define CGAME_IMPORT_API_VERSION    3

//------------------------------------------------------------------
// cgameImport_t
//------------------------------------------------------------------
typedef struct cgameImport_s {

	//
	// Print / Error / Timing
	//
	void       (*Com_Printf)                  (const char* fmt, ...);
	void       (*Com_Error)                   (int errorLevel, const char* fmt, ...);
	int        (*Sys_Milliseconds)            (void);

	//
	// Cvars
	//
	void       (*Cvar_Register)               (vmCvar_t* cvar, const char* varName, const char* defaultValue, int flags);
	void       (*Cvar_Update)                 (vmCvar_t* cvar);
	void       (*Cvar_Set)                    (const char* varName, const char* value);
	void       (*Cvar_VariableStringBuffer)   (const char* varName, char* buffer, int bufsize);

	//
	// Command system
	//
	int        (*Cmd_Argc)                    (void);
	void       (*Cmd_ArgvBuffer)              (int arg, char* buffer, int bufferLength);
	void       (*Cmd_ArgsBuffer)              (char* buffer, int bufferLength);

	//
	// File system
	//
	int        (*FS_FOpenFileByMode)          (const char* qpath, fileHandle_t* f, fsMode_t mode);
	int        (*FS_Read)                     (void* buffer, int len, fileHandle_t f);
	int        (*FS_Write)                    (const void* buffer, int len, fileHandle_t f);
	void       (*FS_FCloseFile)               (fileHandle_t f);

	//
	// Console command buffers
	//
	void       (*Cbuf_AddText)                (const char* text);

	//
	// Add / remove console commands
	//
	void       (*CL_AddCgameCommand)          (const char* cmdName);
	void       (*Cmd_RemoveCommand)           (const char* cmdName);

	//
	// Client commands
	//
	void       (*CL_AddReliableCommand)       (const char* cmd);

	//
	// Screen
	//
	void       (*SCR_UpdateScreen)            (void);

	//
	// Collision Map
	//
	void       (*CL_CM_LoadMap)               (const char* mapname);
	int        (*CM_NumInlineModels)          (void);
	int        (*CM_InlineModel)              (int index);
	clipHandle_t (*CM_TempBoxModel)           (const vec3_t mins, const vec3_t maxs, int capsule);
	int        (*CM_PointContents)            (const vec3_t p, int brushmask);
	int        (*CM_TransformedPointContents) (const vec3_t p, int brushmask, const vec3_t origin, const vec3_t angles);
	void       (*CM_BoxTrace)                 (trace_t* results, const vec3_t start, const vec3_t end,
		const vec3_t mins, const vec3_t maxs,
		int model, int brushmask, int capsule);
	void       (*CM_TransformedBoxTrace)      (trace_t* results, const vec3_t start, const vec3_t end,
		const vec3_t mins, const vec3_t maxs,
		int model, int brushmask,
		const vec3_t origin, const vec3_t angles,
		int capsule);

	//
	// Renderer: mark fragments
	//
	int        (*re_MarkFragments)            (int numPoints, const vec3_t* points, const vec3_t projection,
											  int maxPoints, vec3_t pointBuffer, int maxFragments, markFragment_t* fragmentBuffer);

	//
	// Sound
	//
	void       (*S_StartSound)                (vec3_t origin, int entnum, int entchannel, sfxHandle_t sfx);
	void       (*S_StartSoundEx)              (vec3_t origin, int entnum, int entchannel, sfxHandle_t sfx, int flags);
	void       (*S_StartLocalSound)           (sfxHandle_t sfx, int channelNum);
	void       (*S_ClearLoopingSounds)        (void);
	void       (*S_AddLoopingSound)           (int entnum, const vec3_t origin, const vec3_t velocity,
		sfxHandle_t sfx, int range, int vol);
	void       (*S_UpdateEntityPosition)      (int entityNum, const vec3_t origin);
	int        (*S_GetVoiceAmplitude)         (int entityNum);
	void       (*S_Respatialize)              (int entityNum, const vec3_t head, vec3_t axis[3], int inwater);
	sfxHandle_t(*S_RegisterSound)             (const char* sample, qboolean precache);
	void       (*S_StartBackgroundTrack)      (const char* intro, const char* loop);
	void       (*S_StartStreamingSound)       (const char* intro, const char* loop, int entnum, int channel, int attenuation);
	void       (*S_StopBackgroundTrack)       (void);

	//
	// Renderer: model / shader registration
	//
	void       (*re_LoadWorld)                (const char* mapname);
	qhandle_t(*re_RegisterModel)            (const char* name);
	qhandle_t(*re_RegisterSkin)             (const char* name);
	qboolean(*re_GetSkinModel)             (int skinid, const char* type, char* name);
	qhandle_t(*re_GetShaderFromModel)       (int modelid, int surfnum, int withlightmap);
	qhandle_t(*re_RegisterShader)           (const char* name);
	void       (*re_RegisterFont)             (const char* fontName, int pointSize, fontInfo_t* font);
	qhandle_t(*re_RegisterShaderNoMip)      (const char* name);

	//
	// Renderer: scene building
	//
	void       (*re_ClearScene)               (void);
	void       (*re_AddRefEntityToScene)      (const refEntity_t* ent);
	void       (*re_AddPolyToScene)           (qhandle_t hShader, int numVerts, const polyVert_t* verts);
	void       (*re_AddPolysToScene)          (qhandle_t hShader, int numVerts, const polyVert_t* verts, int numPolys);
	void       (*re_AddLightToScene)          (const vec3_t org, float intensity, float r, float g, float b, int overdraw);
	void       (*re_AddCoronaToScene)         (const vec3_t org, float r, float g, float b, float scale, int id, qboolean visible);
	void       (*re_SetFog)                   (int fogvar, int var1, int var2, float r, float g, float b, float density);
	void       (*re_RenderScene)              (const refdef_t* fd);

	//
	// Renderer: 2D drawing
	//
	void       (*re_SetColor)                 (const float* rgba);
	void       (*re_DrawStretchPic)           (float x, float y, float w, float h,
		float s1, float t1, float s2, float t2,
		qhandle_t hShader);
	void       (*re_DrawRotatedPic)           (float x, float y, float w, float h,
		float s1, float t1, float s2, float t2,
		qhandle_t hShader, float angle);
	void       (*re_DrawStretchPicGradient)   (float x, float y, float w, float h,
		float s1, float t1, float s2, float t2,
		qhandle_t hShader,
		const float* gradientColor, int gradientType);

	//
	// Renderer: utility
	//
	void       (*re_ModelBounds)              (qhandle_t model, vec3_t mins, vec3_t maxs);
	int        (*re_LerpTag)                  (orientation_t* tag, const refEntity_t* refent, const char* tagName, int startIndex);
	qboolean(*re_GetEntityToken)           (char* buffer, int size);
	void       (*re_RemapShader)              (const char* oldShader, const char* newShader, const char* timeOffset);

	//
	// CL_*
	//
	void       (*CL_GetGlconfig)              (glconfig_t* config);
	void       (*CL_GetGameState)             (gameState_t* gs);
	void       (*CL_GetCurrentSnapshotNumber) (int* snapshotNumber, int* serverTime);
	qboolean(*CL_GetSnapshot)              (int snapshotNumber, snapshot_t* snapshot);
	qboolean (*CL_GetServerCommand)        (int serverCommandNumber);
	int        (*CL_GetCurrentCmdNumber)      (void);
	qboolean(*CL_GetUserCmd)               (int cmdNumber, usercmd_t* ucmd);
	void       (*CL_SetUserCmdValue)          (int userCmdValue, int flags, float sensitivityScale, int mpIdentClient, int weapon);
	void       (*CL_SetClientLerpOrigin)      (float x, float y, float z);

	//
	// Memory / key / etc.
	//
	int        (*Hunk_MemoryRemaining)        (void);
	qboolean(*Key_IsDown)                  (int keynum);
	int        (*Key_GetCatcher)              (void);
	void       (*Key_SetCatcher)              (int catcher);
	int        (*Key_GetKey)                  (const char* binding);

	//
	// libc-ish
	//
	void* (*_Memset)                    (void* dest, int c, size_t count);
	void* (*_Memcpy)                    (void* dest, const void* src, size_t count);
	char* (*_Strncpy)                   (char* dest, const char* src, size_t n);

	//
	// Math
	//
	float      (*_Sin)                       (float x);
	float      (*_Cos)                       (float x);
	float      (*_Atan2)                     (float y, float x);
	float      (*_Sqrt)                      (float x);
	float      (*_Floor)                     (float x);
	float      (*_Ceil)                      (float x);
	float      (*_Acos)                      (float x);

	//
	// Botlib PC_*
	//
	int        (*PC_AddGlobalDefine)         (char* define);
	int        (*PC_LoadSourceHandle)        (const char* filename);
	int        (*PC_FreeSourceHandle)        (int handle);
	int        (*PC_ReadTokenHandle)         (int handle, pc_token_t* pc_token);
	int        (*PC_SourceFileAndLine)       (int handle, char* filename, int* line);

	//
	// Time, Vector snapping, etc.
	//
	int        (*Com_RealTime)               (qtime_t* qtime);
	void       (*Sys_SnapVector)             (float* vec);

	//
	// Server-vs-Game bridging
	//
	void       (*SV_SendMoveSpeedsToGame)    (int entnum, char* movespeeds);

	//
	// Cinematics
	//
	int        (*CIN_PlayCinematic)          (const char* arg0, int x, int y, int w, int h, int systemBits);
	e_status   (*CIN_StopCinematic)          (int handle);
	e_status   (*CIN_RunCinematic)           (int handle);
	void       (*CIN_DrawCinematic)          (int handle);
	void       (*CIN_SetExtents)             (int handle, int x, int y, int w, int h);

	//
	// Camera system
	//
	qboolean(*loadCamera)                 (int camNum, const char* name);
	void       (*startCamera)                (int camNum, int time);
	qboolean(*getCameraInfo)              (int camNum, int time, float* origin, float* angles, float* fov);

	//
	// In-game popups / UI hooking
	//
	void       (*InGame_Popup)               (const char* menuName);
	void       (*InGame_ClosePopup)          (const char* menuName);
	void       (*CL_AddToLimboChat)          (const char* str);

	//
	// Key and binding strings
	//
	void       (*Key_GetBindingBuf)          (int keynum, char* buf, int buflen);
	void       (*Key_SetBinding)             (int keynum, const char* binding);
	void       (*Key_KeynumToStringBuf)      (int keynum, char* buf, int buflen);

	//
	// Translation
	//
	void       (*CL_TranslateString)         (const char* string, char* buf);

} cgameImport_t;

/*
==================================================================

functions exported to the main executable

==================================================================
*/

typedef struct cGameExport_s {
	// These function pointers match the cgame functions:
	void     (*Init)(int serverMessageNum, int serverCommandSequence, int clientNum);
	void     (*Shutdown)(void);
	qboolean(*ConsoleCommand)(void);
	void     (*DrawActiveFrame)(int serverTime, stereoFrame_t stereoView, qboolean demoPlayback);
	int      (*CrosshairPlayer)(void);
	int      (*LastAttacker)(void);
	void     (*KeyEvent)(int key, qboolean down);
	void     (*MouseEvent)(int dx, int dy);
	void     (*EventHandling)(int type);
	qboolean (*GetTag)(int clientNum, char* tagname, orientation_t* orient);
	qboolean (*CheckCenterView)(void);
} cGameExport_t;

//----------------------------------------------

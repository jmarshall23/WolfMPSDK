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

#ifndef __UI_PUBLIC_H__
#define __UI_PUBLIC_H__

#define UI_API_VERSION  4

typedef struct {
	connstate_t connState;
	int connectPacketCount;
	int clientNum;
	char servername[MAX_STRING_CHARS];
	char updateInfoString[MAX_STRING_CHARS];
	char messageString[MAX_STRING_CHARS];
} uiClientState_t;


// Now define the struct of function pointers:
typedef struct uiImport_s {
	// --- UI_ERROR/UI_PRINT ---
	void        (*Com_Error)(int level, const char* fmt, ...);
	void        (*Com_Printf)(const char* fmt, ...);

	// --- UI_MILLISECONDS ---
	int         (*Sys_Milliseconds)(void);

	// --- Cvar-related ---
	void        (*Cvar_Register)(vmCvar_t* vmCvar, const char* varName,
		const char* defaultValue, int flags);
	void        (*Cvar_Update)(vmCvar_t* vmCvar);
	void        (*Cvar_Set)(const char* varName, const char* value);
	float       (*Cvar_VariableValue)(const char* varName);
	void        (*Cvar_VariableStringBuffer)(const char* varName, char* buffer, int bufsize);
	void        (*Cvar_SetValue)(const char* varName, float value);
	void        (*Cvar_Reset)(const char* varName);
	cvar_t	   *(*Cvar_Get)(const char* varName, const char* value, int flags);
	void        (*Cvar_InfoStringBuffer)(int bit, char* buf, int bufsize);

	// --- Cmd / console-related ---
	int         (*Cmd_Argc)(void);
	void        (*Cmd_ArgvBuffer)(int arg, char* buffer, int bufferLength);
	void        (*Cbuf_ExecuteText)(int exec_when, const char* text);

	// --- File system ---
	int         (*FS_FOpenFileByMode)(const char* qpath, fileHandle_t* f, fsMode_t mode);
	int         (*FS_Read)(void* buffer, int len, fileHandle_t f);
	int         (*FS_Write)(const void* buffer, int len, fileHandle_t f);
	void        (*FS_FCloseFile)(fileHandle_t f);
	int         (*FS_Delete)(char* filename);
	int         (*FS_GetFileList)(const char* path, const char* extension,
		char* listbuf, int bufsize);

	// --- Renderer hooks ---
	qhandle_t(*R_RegisterModel)(const char* name);
	qhandle_t(*R_RegisterSkin)(const char* name);
	qhandle_t(*R_RegisterShaderNoMip)(const char* name);
	void        (*R_ClearScene)(void);
	void        (*R_AddRefEntityToScene)(const refEntity_t* refEntity);
	void        (*R_AddPolyToScene)(qhandle_t hShader, int numVerts, const polyVert_t* verts);
	void        (*R_AddPolysToScene)(qhandle_t hShader, int numVerts, const polyVert_t* verts, int num);
	void        (*R_AddLightToScene)(const float* org, float intensity, float r,
		float g, float b, int overdraw);
	void        (*R_AddCoronaToScene)(const vec_t* org, float r, float g,
		float b, float scale, int id, qboolean visible);
	void        (*R_RenderScene)(const refdef_t* refdef);
	void        (*R_SetColor)(const float* rgba);
	void        (*R_DrawStretchPic)(float x, float y, float w, float h,
		float s1, float t1, float s2, float t2,
		qhandle_t hShader);
	void        (*R_ModelBounds)(qhandle_t model, float* mins, float* maxs);

	// --- UI-specific calls ---
	void        (*SCR_UpdateScreen)(void);
	int         (*R_LerpTag)(orientation_t* tag, const refEntity_t* refent, const char* tagName, int startIndex);

	// --- Sound ---
	int         (*S_RegisterSound)(const char* name, qboolean compressed); // or qboolean
	void        (*S_StartLocalSound)(int sfx, int channelNum);
	void        (*S_StopBackgroundTrack)(void);
	void        (*S_StartBackgroundTrack)(const char* intro, const char* loop);

	// --- Keyboard ---
	void        (*Key_KeynumToStringBuf)(int keynum, char* buf, int buflen);
	void        (*Key_GetBindingBuf)(int keynum, char* buf, int buflen);
	void        (*Key_SetBinding)(int keynum, const char* binding);
	qboolean    (*Key_IsDown)(int keynum);
	qboolean    (*Key_GetOverstrikeMode)(void);
	void        (*Key_SetOverstrikeMode)(qboolean state);
	void        (*Key_ClearStates)(void);
	int         (*Key_GetCatcher)(void);
	void        (*Key_SetCatcher)(int catcher);

	// --- Clipboard, client, configstring ---
	void        (*GetClipboardData)(char* buf, int bufsize);
	void        (*GetClientState)(uiClientState_t* state);
	void        (*CL_GetGlconfig)(glconfig_t* config);
	int         (*GetConfigString)(int index, char* buf, int size);

	// --- Server browser / LAN ---
	void        (*LAN_LoadCachedServers)(void);
	void        (*LAN_SaveServersToCache)(void);
	int         (*LAN_AddServer)(int source, const char* name, const char* addr);
	void        (*LAN_RemoveServer)(int source, const char* addr);
	int         (*LAN_GetPingQueueCount)(void);
	void        (*LAN_ClearPing)(int n);
	void        (*LAN_GetPing)(int n, char* buf, int buflen, int* pingtime);
	void        (*LAN_GetPingInfo)(int n, char* buf, int buflen);
	int         (*LAN_GetServerCount)(int source);
	void        (*LAN_GetServerAddressString)(int source, int n, char* buf, int buflen);
	void        (*LAN_GetServerInfo)(int source, int n, char* buf, int buflen);
	int         (*LAN_GetServerPing)(int source, int n);
	void        (*LAN_MarkServerVisible)(int source, int n, qboolean visible);
	int         (*LAN_ServerIsVisible)(int source, int n);
	qboolean    (*LAN_UpdateVisiblePings)(int source);
	void        (*LAN_ResetPings)(int source);
	int         (*LAN_GetServerStatus)(char* serverAddress, char* serverStatus, int maxLen);
	int         (*LAN_CompareServers)(int source, int sortKey, int sortDir,
		int s1, int s2);

	// --- Hunk / memory usage ---
	int         (*Hunk_MemoryRemaining)(void);

	// --- CD Key ---
	void        (*CLUI_GetCDKey)(char* buf, int buflen);
	void        (*CLUI_SetCDKey)(char* buf);
	qboolean    (*CL_CDKeyValidate)(const char* key, const char* chksum);

	// --- Font ---
	void        (*R_RegisterFont)(const char* fontName, int pointSize, fontInfo_t* font);

	// --- Basic C-library helpers (memcpy, memset, etc.) ---
	void* (*_Memset)(void* dest, int c, size_t count);
	void* (*_Memcpy)(void* dest, const void* src, size_t count);
	char* (*_Strncpy)(char* dest, const char* src, size_t count);

	// --- Math functions ---
	float       (*_Sin)(float x);
	float       (*_Cos)(float x);
	float       (*_Atan2)(float y, float x);
	float       (*_Sqrt)(float x);
	float       (*_Floor)(float x);
	float       (*_Ceil)(float x);

	// --- Botlib parser calls (if used by UI) ---
	int         (*PC_AddGlobalDefine)(char* define);
	int         (*PC_LoadSourceHandle)(const char* filename);
	int         (*PC_FreeSourceHandle)(int handle);
	int         (*PC_ReadTokenHandle)(int handle, pc_token_t* pc_token);
	int         (*PC_SourceFileAndLine)(int handle, char* filename, int* line);

	// --- Real time ---
	int         (*Com_RealTime)(qtime_t* qtime);

	// --- Cinematics ---
	int         (*CIN_PlayCinematic)(const char* name, int x, int y,
		int w, int h, int flags);
	e_status    (*CIN_StopCinematic)(int handle);
	e_status    (*CIN_RunCinematic)(int handle);
	void        (*CIN_DrawCinematic)(int handle);
	void        (*CIN_SetExtents)(int handle, int x, int y, int w, int h);

	// --- Remap shader ---
	void        (*R_RemapShader)(const char* oldShader, const char* newShader, const char* timeOffset);

	// --- Misc UI calls (limbo strings, translation, autoupdate, open URL) ---
	qboolean    (*CL_GetLimboString)(int index, char* buf);
	void        (*CL_TranslateString)(const char* string, char* buf);
	void        (*CL_CheckAutoUpdate)(void);
	void        (*CL_GetAutoUpdate)(void);
	void        (*CL_OpenURL)(const char* url);

} uiImport_t;

typedef enum {
	UIMENU_NONE,
	UIMENU_MAIN,
	UIMENU_INGAME,
	UIMENU_NEED_CD,
	UIMENU_BAD_CD_KEY,
	UIMENU_TEAM,
	UIMENU_POSTGAME,
	UIMENU_NOTEBOOK,
	UIMENU_CLIPBOARD,
	UIMENU_HELP,
	UIMENU_BOOK1,           //----(SA)	added
	UIMENU_BOOK2,           //----(SA)	added
	UIMENU_BOOK3,           //----(SA)	added
	UIMENU_WM_PICKTEAM,         // NERVE - SMF
	UIMENU_WM_PICKPLAYER,       // NERVE - SMF
	UIMENU_WM_QUICKMESSAGE,     // NERVE - SMF
	UIMENU_WM_QUICKMESSAGEALT,  // NERVE - SMF
	UIMENU_WM_LIMBO,            // NERVE - SMF
	UIMENU_WM_AUTOUPDATE        // NERVE - DHM
} uiMenuCommand_t;

#define SORT_HOST           0
#define SORT_MAP            1
#define SORT_CLIENTS        2
#define SORT_GAME           3
#define SORT_PING           4
#define SORT_PUNKBUSTER     5

typedef struct uiExport_s {
	// Maps to: UI_INIT
	void        (*Init)(qboolean isRestart);

	// Maps to: UI_SHUTDOWN
	void        (*Shutdown)(void);

	// Maps to: UI_KEY_EVENT
	void        (*KeyEvent)(int key, qboolean down);

	// Maps to: UI_MOUSE_EVENT
	void        (*MouseEvent)(int dx, int dy);

	// Maps to: UI_REFRESH
	void        (*Refresh)(int serverTime);

	// Maps to: UI_IS_FULLSCREEN
	qboolean(*IsFullscreen)(void);

	// Maps to: UI_SET_ACTIVE_MENU
	void        (*SetActiveMenu)(uiMenuCommand_t menu);

	// Maps to: UI_GET_ACTIVE_MENU
	uiMenuCommand_t(*GetActiveMenu)(void);

	// Maps to: UI_CONSOLE_COMMAND
	qboolean(*ConsoleCommand)(int realTime);

	// Maps to: UI_DRAW_CONNECT_SCREEN
	void        (*DrawConnectScreen)(qboolean overlay);

	// Maps to: UI_CHECKEXECKEY
	qboolean    (*CheckExecKey)(int key);
} uiExport_t;

#endif

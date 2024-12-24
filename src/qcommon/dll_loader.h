#pragma once

#include <cstring>    // for memset, strncpy, etc.
// Include any headers you need for your environment (e.g., Windows headers).
// #include <windows.h>   // Typically needed for HINSTANCE, LoadLibrary, etc.

// Forward declarations or includes for your Sys_* functions:
void  Sys_UnloadDll(void* dllHandle);
void* Sys_LoadDll(const char* name, char* fqpath);
void* Sys_GetProcAddress(void* dllHandle, const char* procName);

class DllLoader {
private:
	void* m_dllHandle;
	char  m_fqpath[MAX_QPATH];

public:
	// Constructor: Load the DLL
	DllLoader(const char* dllName)
		: m_dllHandle(nullptr)
	{
		memset(m_fqpath, 0, sizeof(m_fqpath));
		m_dllHandle = Sys_LoadDll(dllName, m_fqpath);
	}

	// Destructor: Unload the DLL
	~DllLoader() {
		if (m_dllHandle) {
			Sys_UnloadDll(m_dllHandle);
			m_dllHandle = nullptr;
		}
	}

	// Returns whether the DLL is loaded successfully
	bool IsLoaded() const {
		return (m_dllHandle != nullptr);
	}

	// Optional: retrieve the fully qualified path used to load the DLL
	const char* GetLoadedPath() const {
		return m_fqpath;
	}

	// Template method to call dllEntry as: ExportT* dllEntry(ImportT* imports)
	template <typename ExportT, typename ImportT>
	ExportT* GetDllEntry(ImportT* imports) {
		if (!m_dllHandle) {
			return nullptr;
		}

		using DllEntryFunc = ExportT * (*)(ImportT*);

		// Retrieve the function pointer from the DLL
		DllEntryFunc entry = reinterpret_cast<DllEntryFunc>(
			Sys_GetProcAddress(m_dllHandle, "dllEntry")
			);

		// If we failed to get the function pointer, return nullptr
		if (!entry) {
			return nullptr;
		}

		// Call the function with the given imports
		return entry(imports);
	}
};


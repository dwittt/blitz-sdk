#include "hooks.hpp"

#include <stdexcept>
#include <intrin.h>

#include "../ext/minhook/minhook.h"

#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_win32.h"
#include "../ext/imgui/imgui_impl_dx9.h"
#include "../int/usercmd.hpp"
#include "../src/menu/gui.h"

template <typename T>
static T* GetInterface(const char* name, const char* library)
{
	const HINSTANCE handle = GetModuleHandle(library);

	if (!handle)
		return nullptr;

	using Fn = T * (*)(const char*, int*);
	const Fn CreateInterface = reinterpret_cast<Fn>(GetProcAddress(handle, "CreateInterface"));

	return CreateInterface(name, nullptr);
}

static void* g_Client = nullptr;
static void* g_ClientMode = nullptr;



constexpr unsigned int dwLocalPlayer = 0xDE7964;
constexpr unsigned int m_fFlags = 0x104;



void hooks::Setup()
{

	g_Client = GetInterface<void>("VClient018", "client.dll");

	g_ClientMode = **reinterpret_cast<void***>((*reinterpret_cast<unsigned int**>(g_Client))[10] + 5);


	if (MH_Initialize())
		throw std::runtime_error("Unable to initialize minhook");
	// end-scene hook
	if (MH_CreateHook(
		VirtualFunction(gui::device, 42),
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	)) throw std::runtime_error("Unable to hook end scene()");


	// create-move hook
	if (MH_CreateHook(
		(*static_cast<void***>(g_ClientMode))[24],
		&CreateMoveHook,
		reinterpret_cast<void**>(&CreateMoveOriginal)
	)) throw std::runtime_error("Unable to hook create move()");

	// device hook
	if (MH_CreateHook(
		VirtualFunction(gui::device, 16),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	)) throw std::runtime_error("Unable to hook reset()");


	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Unable to enable hooks");

	gui::DestroyDirectX();




}

void hooks::Destroy() noexcept
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept
{
	static const auto returnAddress = _ReturnAddress();

	const auto result = EndSceneOriginal(device, device);

	// stop endscene getting called twice
	if (_ReturnAddress() == returnAddress)
		return result;

	if (!gui::setup)
		gui::SetupMenu(device);

	if (gui::open)
		gui::Render();

	return result;
}

bool __stdcall hooks::CreateMoveHook(float frameTime, UserCmd* cmd)
{
	const bool result = CreateMoveOriginal(g_ClientMode, frameTime, cmd);

	if (!cmd || !cmd->commandNumber)
		return result;



	byte* send_packet = *(byte**)((reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) - sizeof(uintptr_t)) - 0x1C);

	

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

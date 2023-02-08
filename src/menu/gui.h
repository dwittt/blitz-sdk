#pragma once
#include <d3d9.h>
#include "../../ext/imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../../ext/imgui/imgui_internal.h"



namespace gui
{
	// show menu?
	inline bool open = true;

	// is menu setup?
	inline bool setup = false;

	// winapi related
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = { };
	inline WNDPROC originalWindowProcess = nullptr;

	// dx stuff
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline LPDIRECT3D9 d3d9 = nullptr;

	bool SetupWindowClass(const char* windowClassName) noexcept;
	void DestroyWindowClass() noexcept;

	bool SetupWindow(const char* windowName) noexcept;
	void DestroyWindow() noexcept;

	bool SetupDirectX() noexcept;
	void DestroyDirectX() noexcept;

	// setup device
	void Setup();

	void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;
	void Destroy() noexcept;

	void Render() noexcept;
}




inline float content_anim = 0.f;

namespace custom {
	bool tab(const char* icon, const char* label, bool selected);
	bool subtab(const char* label, bool selected);
	void begin_child(const char* name, ImVec2 size);
	void end_child();
	bool collapse_button(bool collapsed);
	void ImRotateStart();
	ImVec2 ImRotationCenter();
	void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter());
}

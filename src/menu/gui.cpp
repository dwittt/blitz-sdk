#include "gui.h"

#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_impl_win32.h"
#include "../../ext/imgui/imgui_impl_dx9.h"

#include <stdexcept>

#include "../variables.hpp"
#include "customs.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

// window process
LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

static int tab = 0;
static int tab2 = 0;

bool gui::SetupWindowClass(const char* windowClassName) noexcept
{
	// populate window class
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	// register
	if (!RegisterClassEx(&windowClass))
		return false;

	return true;
}

void gui::DestroyWindowClass() noexcept
{
	UnregisterClass(
		windowClass.lpszClassName,
		windowClass.hInstance
	);
}

bool gui::SetupWindow(const char* windowName) noexcept
{
	// create temp window
	window = CreateWindow(
		windowClass.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		100,
		100,
		0,
		0,
		windowClass.hInstance,
		0
	);

	if (!window)
		return false;

	return true;
}

void gui::DestroyWindow() noexcept
{
	if (window)
		DestroyWindow(window);
}

bool gui::SetupDirectX() noexcept
{
	const auto handle = GetModuleHandle("d3d9.dll");

	if (!handle)
		return false;

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(
		handle,
		"Direct3DCreate9"
	));

	if (!create)
		return false;

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
		return false;

	D3DPRESENT_PARAMETERS params = { };
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&params,
		&device
	) < 0) return false;

	return true;
}

void gui::DestroyDirectX() noexcept
{
	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (d3d9)
	{
		d3d9->Release();
		d3d9 = NULL;
	}
}

void gui::Setup()
{
	if (!SetupWindowClass("hackClass001"))
		throw std::runtime_error("Failed to create window class.");

	if (!SetupWindow("Hack Window"))
		throw std::runtime_error("Failed to create window.");

	if (!SetupDirectX())
		throw std::runtime_error("Failed to create device.");

	DestroyWindow();
	DestroyWindowClass();
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
	auto params = D3DDEVICE_CREATION_PARAMETERS{ };
	device->GetCreationParameters(&params);

	window = params.hFocusWindow;

	originalWindowProcess = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
	);


	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	setup = true;
}

void gui::Destroy() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// retore wnd proc
	SetWindowLongPtr(
		window,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(originalWindowProcess)
	);

	DestroyDirectX();
}

void gui::Render() noexcept
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	ImFontConfig config;
	
	config.RasterizerMultiply = 1.125f;
	




	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("menu", 0, ImGuiWindowFlags_NoDecoration);
	{
		ImGui::PopStyleVar();

		ImGuiWindow* window = ImGui::GetCurrentWindow();

		ImDrawList* draw = ImGui::GetWindowDrawList();
		ImVec2 p = ImGui::GetWindowPos();
		auto s = ImVec2{}, gs = ImVec2{ 620, 460 };

		static int tab1 = 0;



		ImGui::SetWindowSize({ 620, 460 });

		static bool navbar_collapsed = true;
		static float navbar_width = 0.f; navbar_width = ImLerp(navbar_width, navbar_collapsed ? 0.f : 1.f, 0.04f);
		content_anim = ImLerp(content_anim, 1.f, 0.04f);

		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !navbar_collapsed);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, content_anim * (navbar_width > 0.005f ? 1 / navbar_width / 2 : 1.f));
		ImGui::SetCursorPos({ 81, 25 });
		
		s = ImVec2(ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2, ImGui::GetWindowSize().y - ImGui::GetStyle().WindowPadding.y * 2); p = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y); 
		draw->AddRectFilled(p, ImVec2(p.x + s.x, p.y + s.y), ImColor(33, 33, 33)); //tabs
		draw->AddRectFilled(ImVec2(p.x, p.y + 25), ImVec2(p.x + s.x, p.y + s.y - 25), ImColor(25, 25, 25));

		draw->AddLine(ImVec2(p.x, p.y + 25), ImVec2(p.x + s.x, p.y + 25), ImColor(255, 235, 205)); // tab seperator

		draw->AddLine(ImVec2(p.x, p.y + s.y - 25), ImVec2(p.x + s.x, p.y + s.y - 25), ImColor(255, 235, 205));

		ImGui::SetCursorPosX(20);
		ImGui::SetCursorPosY(10);
		ImGui::BeginGroup();

		if (custom::tab("rage", tab == 0))tab = 0; ImGui::SameLine();
		if (custom::tab("anti-resolve", tab == 2))tab = 2; ImGui::SameLine();
		if (custom::tab("visuals", tab == 3))tab = 3; ImGui::SameLine();
		if (custom::tab("misc", tab == 4))tab = 4; ImGui::SameLine();
		if (custom::tab("config", tab == 5))tab = 5; ImGui::SameLine();
		if (custom::tab("lua", tab == 6))tab = 6;


		if (tab == 0)
		{
			
			{
				ImGui::SetCursorPosY(35);
				ImGui::BeginGroup();
				ImGui::SetCursorPosX(15);
				ImGui::MenuChild("general", ImVec2(300, 205), false);
				{
					
				}
				ImGui::EndChild();
				ImGui::EndGroup();
			}
			{
				ImGui::SetCursorPosY(240);
				ImGui::BeginGroup();
				ImGui::SetCursorPosX(15);
				ImGui::MenuChild("other", ImVec2(300, 180), false);
				{

				}
				ImGui::EndChild();
				ImGui::EndGroup();
			}
			{
				ImGui::SetCursorPosY(35);
				ImGui::BeginGroup();
				ImGui::SetCursorPosX(320);
				ImGui::MenuChild("settings", ImVec2(285, 385), false);
				{
					
				}
				ImGui::EndChild();
				ImGui::EndGroup();
			}
		}

			
	}
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
)
{
	// toogle menu
	if (GetAsyncKeyState(VK_INSERT) & 1)
		gui::open = !gui::open;

	// pass messages to imgui
	if (gui::open && ImGui_ImplWin32_WndProcHandler(
		window,
		message,
		wideParam,
		longParam
	)) return 1L;

	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParam,
		longParam
	);
}

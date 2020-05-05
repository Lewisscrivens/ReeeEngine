#include "UserInterfaceModule.h"
#include "../Application.h"
#include "../ReeeLog.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_impl_dx11.h"

namespace ReeeEngine
{
	UserInterfaceModule::UserInterfaceModule() : Module("User Interface Module"), appPointer(Application::GetEngine()) {}

	void UserInterfaceModule::InitModule()
	{
		// Create context.
		ImGui::CreateContext();

		// Setup config for imgui.
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigDockingWithShift = true;

		// Setup style.
		ImGui::StyleColorsDark();

		// Get pointer to the window from the app.
		Window* currWindow = &appPointer.GetWindow();
		Graphics* currGraphics = &currWindow->GetGraphics();

		// Initalise Imgui for win32 and DirectX.
		ImGui_ImplWin32_Init(currWindow->GetHwnd());
		ImGui_ImplDX11_Init(currGraphics->GetDevice(), currGraphics->GetContext());

		// BUG FIX: Forcing resize somehow runs important init features for imgui.
		// Prevents the mouse becoming out of sync until the window is resized.
		currWindow->ForceResizeWindow();
	}

	void UserInterfaceModule::ShutdownModule()
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
	}

	void UserInterfaceModule::Tick(float deltaTime)
	{
		//...
	}

	void UserInterfaceModule::OnDelegate(Delegate& del)
	{
		// Ensure the context is set before dispatching delegate events.
		if (ImGui::GetCurrentContext() == NULL)
		{
			REEE_LOG(Log, "Imgui context is not set when trying to dispatch delegate events.");
			return;
		}

		// Dispatch any passed delegate events.
		DelegateDispatcher Dispatcher(del);

		// Input dispatching.
		Dispatcher.Dispatch<MousePressedDelegate>(BIND_DELEGATE(UserInterfaceModule::MouseButtonPressed));
		Dispatcher.Dispatch<MouseReleasedDelegate>(BIND_DELEGATE(UserInterfaceModule::MouseButtonReleased));
		Dispatcher.Dispatch<MouseMovedDelegate>(BIND_DELEGATE(UserInterfaceModule::MouseMoved));
		Dispatcher.Dispatch<MouseScrolledDelegate>(BIND_DELEGATE(UserInterfaceModule::MouseScrolled));
		Dispatcher.Dispatch<KeyPressedDelegate>(BIND_DELEGATE(UserInterfaceModule::KeyPressed));
		Dispatcher.Dispatch<KeyReleasedDelegate>(BIND_DELEGATE(UserInterfaceModule::KeyReleased));
		Dispatcher.Dispatch<CharPressedDelegate>(BIND_DELEGATE(UserInterfaceModule::CharPressed));
	}

	void UserInterfaceModule::BeginFrame()
	{
		// Prepare to render.
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);
	}

	void UserInterfaceModule::OnImGuiRender()
	{
		// NOTE: Demo imgui code for testing...
		//static bool show_demo_window = true;
		//if (show_demo_window)
		//{
		//	ImGui::ShowDemoWindow(&show_demo_window);
		//}
	}

	void UserInterfaceModule::EndFrame()
	{
		// Render.
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	bool UserInterfaceModule::MouseButtonPressed(MousePressedDelegate& del)
	{
		// Get pointer to the window from the app.
		Window* currWindow = &appPointer.GetWindow();

		// Handle passing mouse down input to imgui.
		// NOTE: Taken from WinMessageProc in imgui win32.h
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == NULL)
			::SetCapture(currWindow->GetHwnd());
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[(int)del.GetButton()] = true;
		if (io.WantCaptureMouse) return true;
		return false;
	}

	bool UserInterfaceModule::MouseButtonReleased(MouseReleasedDelegate& del)
	{
		// Get pointer to the window from the app.
		Window* currWindow = &appPointer.GetWindow();

		// Handle input for mouse release for imgui.
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[(int)del.GetButton()] = false;
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == currWindow->GetHwnd())
			::ReleaseCapture();
		return false;
	}

	bool UserInterfaceModule::MouseMoved(MouseMovedDelegate& del)
	{
		// Handle input for mouse movement for imgui.
		ImGuiIO& io = ImGui::GetIO();
		Vector2D mousePos = del.GetMousePos();
		io.MousePos = ImVec2(mousePos.X, mousePos.Y);
		return false;
	}

	bool UserInterfaceModule::MouseScrolled(MouseScrolledDelegate& del)
	{
		// Handle input for mouse scrolling for imgui.
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += del.GetScrollDelta();
		return false;
	}

	bool UserInterfaceModule::KeyPressed(KeyPressedDelegate& del)
	{
		// Handle input for keyboard input pressed for imgui.
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[del.GetKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[VK_LCONTROL] || io.KeysDown[VK_RCONTROL];
		io.KeyShift = io.KeysDown[VK_LSHIFT] || io.KeysDown[VK_RSHIFT];
		io.KeyCtrl = io.KeysDown[VK_LMENU] || io.KeysDown[VK_RMENU];
		io.KeySuper = io.KeysDown[VK_LWIN] || io.KeysDown[VK_RWIN];
		return false;
	}

	bool UserInterfaceModule::KeyReleased(KeyReleasedDelegate& del)
	{
		// Handle input for keyboard input released for imgui.
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[del.GetKeyCode()] = false;
		io.KeyCtrl = !io.KeysDown[VK_LCONTROL] && !io.KeysDown[VK_RCONTROL];
		io.KeyShift = !io.KeysDown[VK_LSHIFT] && !io.KeysDown[VK_RSHIFT];
		io.KeyCtrl = !io.KeysDown[VK_LMENU] && !io.KeysDown[VK_RMENU];
		io.KeySuper = !io.KeysDown[VK_LWIN] && !io.KeysDown[VK_RWIN];
		return false;
	}

	bool UserInterfaceModule::CharPressed(CharPressedDelegate& del)
	{
		// Handle input for keyboard character input for imgui.
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter((unsigned short)del.GetKeyCode());
		return false;
	}
}
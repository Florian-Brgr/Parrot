#include "Ptpch.hpp"
#include "glad/glad.h"
#include "Window_GLFW.hpp"
#include "Debug/Debugstream.hpp"

#include "Scene/Scene.hpp"

namespace Parrot
{
	void Internal_SetBoundWindow(Window* window);
	void Internal_SetMouseButtonState(MouseButton button, MouseButtonState state);
	void Internal_SetKeyboardKeyState(KeyCode keyCode, KeyState state);

	static GLFWwindow* s_MainWindow = nullptr;
	static std::unordered_map<GLFWwindow*, Scene*> s_WindowSceneMap;

	Window_GLFW::Window_GLFW(const Asset::WindowAsset& asset)
		: Window(asset), m_Window(nullptr), m_Title(asset.GetTag())
	{
		if (s_WindowSceneMap.empty())
		{
			if (!glfwInit())
			{
				DebugOut << GLFWInitError << "Window: " << m_Title << Debugstream::EndMsg;
				Close();
				return;
			}
			DebugOut << "GLFW initialized successfully." << Debugstream::EndMsg;
		}
		m_Window = glfwCreateWindow(asset.size.x, asset.size.y, m_Title.c_str(), nullptr, s_MainWindow);
		if (!m_Window)
		{
			DebugOut << WindowCreationError << "Window: " << m_Title << Debugstream::EndMsg;
			Close();
			return;
		}
		DebugOut << "Window \"" << m_Title << "\" created successfully." << Debugstream::EndMsg;
		Bind();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			DebugOut << GLEWInitError << "Window: " << m_Title << Debugstream::EndMsg;
			Close();
			return;
		}
		DebugOut << "GLEW for Window \"" << m_Title << "\" initialized successfully." << Debugstream::EndMsg;

	    // Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int32_t width, int32_t height)
	    {
	    	glViewport(0, 0, width, height);
			s_WindowSceneMap[window]->RaiseEvent(Event(EventType::WindowResize, Math::Vec2u((uint32_t)width, (uint32_t)height)));
	    });
	    glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* window, int32_t state)
	    {
			s_WindowSceneMap[window]->RaiseEvent(Event(EventType::WindowMaximize));
		});
	    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
	    {
			s_WindowSceneMap[window]->RaiseEvent(Event(EventType::CloseRequest));
		});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
		{
			s_WindowSceneMap[window]->RaiseEvent(Event(EventType::MouseMove, Math::Vec2u((uint32_t)x, (uint32_t)y)));
		});
	    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int32_t button, int32_t action, int32_t scanfield)
	    {
			Internal_SetMouseButtonState((MouseButton)button, (MouseButtonState)action);
			s_WindowSceneMap[window]->RaiseEvent(Event(EventType::MousePress, (MouseButton)button, (MouseButtonState)action));
		});
	    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
	    {
			s_WindowSceneMap[window]->RaiseEvent(Event(EventType::MouseScroll, (int32_t)xOffset, (int32_t)yOffset));
		});
	    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
	    {
	    	Internal_SetKeyboardKeyState((KeyCode)key, (KeyState)action);
			s_WindowSceneMap[window]->RaiseEvent(Event(EventType::KeyPress, (KeyCode)key, (KeyState)action));
		});
		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int32_t focus)
		{
			if (focus == GLFW_TRUE)
				s_WindowSceneMap[window]->RaiseEvent(Event(EventType::WindowGotFocus));
			else
				s_WindowSceneMap[window]->RaiseEvent(Event(EventType::WindowLostFocus));
		});
		if (s_WindowSceneMap.empty())
		{
			s_MainWindow = m_Window;
			glfwSwapInterval(1);
		}
		else
			glfwSwapInterval(0);
		s_WindowSceneMap[m_Window] = &LoadedScene();
	}

	Window_GLFW::~Window_GLFW()
	{
		if (m_Window)
		{
			s_WindowSceneMap.erase(m_Window);
			glfwDestroyWindow(m_Window);
		}
		if (s_WindowSceneMap.empty())
		{
			glfwTerminate();
			s_MainWindow = nullptr;
		}
	}

	void Window_GLFW::Bind() const
	{
		if (glfwGetCurrentContext() != m_Window)
		{
			glfwMakeContextCurrent(m_Window);
			Internal_SetBoundWindow((Window*)this);
		}
	}
	void Window_GLFW::SetTitle(const std::string& title)
	{
		m_Title = title;
		glfwSetWindowTitle(m_Window, title.c_str());
	}
	const std::string& Window_GLFW::GetTitle() const
	{
		return m_Title;
	}
	void Window_GLFW::SetCursorPos(const Math::Vec2i& pos)
	{
		glfwSetCursorPos(m_Window, (double)pos.x, (double)pos.y);
	}
	Math::Vec2i Window_GLFW::GetCursorPos() const
	{
		double x, y;
		glfwGetCursorPos(m_Window, &x, &y);
		return Math::Vec2i((int32_t)x, (int32_t)y);
	}
	void Window_GLFW::ShowCursor(bool state) const
	{
		if (state)
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else 
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	void Window_GLFW::Maximized(bool state)
	{
		if (state)
			glfwMaximizeWindow(m_Window);
		else
			glfwRestoreWindow(m_Window);
	}
	void Window_GLFW::Minimized(bool state)
	{
		if (state)
			glfwIconifyWindow(m_Window);
		else
			glfwRestoreWindow(m_Window);
	}

	void Window_GLFW::SetSize(const Math::Vec2u& dim)
	{
		glfwSetWindowSize(m_Window, (int32_t)dim.x, (int32_t)dim.y);
	}
	Math::Vec2u Window_GLFW::GetSize() const
	{
		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		return Math::Vec2u((uint32_t)width, (uint32_t)height);
	}
	void Window_GLFW::Resizable(bool state)
	{
		glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, (int32_t)state);
	}

	void Window_GLFW::SetWindowPos(Math::Vec2i pos)
	{
		glfwSetWindowPos(m_Window, pos.x, pos.y);
	}
	Math::Vec2i Window_GLFW::GetWindowPos()
	{
		Math::Vec2i pos;
		glfwGetWindowPos(m_Window, &pos.x, &pos.y);
		return pos;
	}
	void Window_GLFW::Refresh()
	{
		Bind();
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
	void Window_GLFW::GainFocus()
	{
		glfwFocusWindow(m_Window);
	}
	bool Window_GLFW::HasFocus() const
	{
		return glfwGetWindowAttrib(m_Window, GLFW_FOCUSED);
	}
	void Window_GLFW::Clear()
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
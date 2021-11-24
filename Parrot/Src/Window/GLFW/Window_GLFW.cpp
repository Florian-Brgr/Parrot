#include "Ptpch.hpp"
#include "glad/glad.h"
#include "Window_GLFW.hpp"
#include "Scene/Scene.hpp"
#include "Debug/InternalLog.hpp"
#include "Input/InternalInput.hpp"
#include "Core/InternalApplication.hpp"

namespace Parrot
{
	static GLFWwindow* s_MainWindow = nullptr;
	static std::unordered_map<GLFWwindow*, Scene*> s_WindowSceneMap;

	Window_GLFW::Window_GLFW(const PtWindow& ptWindow)
		: Window(ptWindow), m_Window(nullptr), m_Title(ptWindow.GetFilepath().GetFilename().GetName())//, m_IsFocused(true)
	{
		if (s_WindowSceneMap.empty())
		{
			InternalLog::LogAssert(glfwInit(), "GLFW initialization failed!");
			InternalLog::LogInfo("GLFW initialization successful!");
		}
		m_Window = glfwCreateWindow(ptWindow.GetData().size.x, ptWindow.GetData().size.y, m_Title.c_str(), nullptr, s_MainWindow);
		InternalLog::LogAssert(m_Window, "Window \"%\" creation failed!", m_Title);
		InternalLog::LogInfo("Window \"%\" creation successful!", m_Title);
		Bind();

		InternalLog::LogAssert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "GLAD initialization failed!");
		InternalLog::LogInfo("GLAD initialization successful!");

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
			s_MainWindow = m_Window;
		s_WindowSceneMap[m_Window] = &GetLoadedScene();
	    glfwSwapInterval(1);
	}

	Window_GLFW::~Window_GLFW()
	{
		s_WindowSceneMap.erase(m_Window);
		glfwDestroyWindow(m_Window);
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
			Application::Internal_SetBoundWindow((Window*)this);
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

	void Window_GLFW::Maximize()
	{
		glfwMaximizeWindow(m_Window);
	}
	void Window_GLFW::Minimize()
	{
		glfwIconifyWindow(m_Window);
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
	void Window_GLFW::Clear()
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
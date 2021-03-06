#pragma once
#include "Window/Window.hpp"
#include "GLFW/glfw3.h"

namespace Parrot
{
	class Window_GLFW : public Window
	{
	public:
		Window_GLFW(const Asset::WindowAsset& asset);
		~Window_GLFW();

		virtual void SetTitle(const std::string& title) override;
		virtual const std::string& GetTitle() const override;

		virtual void SetCursorPos(const Math::Vec2i& pos) override;
		virtual Math::Vec2i GetCursorPos() const override;
		virtual void ShowCursor(bool state) const override;

		virtual void Maximized(bool state) override;
		virtual void Minimized(bool state) override;
	
		virtual void SetSize(const Math::Vec2u& dim) override;
		virtual Math::Vec2u GetSize() const override;
		virtual void Resizable(bool state) override;

		virtual void SetWindowPos(Math::Vec2i pos) override;
		virtual Math::Vec2i GetWindowPos() override;

		virtual void GainFocus() override;
		virtual bool HasFocus() const override;

		virtual void Bind() const override;
		virtual void Clear() override;
		virtual void Refresh() override;
	private:
		GLFWwindow* m_Window;
		std::string m_Title;
	};
}
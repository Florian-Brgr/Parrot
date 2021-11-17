#include "Ptpch.hpp"
#include "InternalInput.hpp"

namespace Parrot
{
	static MouseButtonState s_MouseButtons[3] = { MouseButtonState::Released };
	static KeyState s_Keys[350] = { KeyState::Released };
	
	MouseButtonState GetButtonState(MouseButton button)
	{
		return s_MouseButtons[(uint32_t)button];
	}
		
	KeyState GetKeyState(KeyCode key)
	{
		return s_Keys[(uint32_t)key];
	}

	void SetMouseButtonState(MouseButton button, MouseButtonState state)
	{
		s_MouseButtons[(uint32_t)button] = state;
	}
	void SetKeyboardKeyState(KeyCode keyCode, KeyState state)
	{
		s_Keys[(uint32_t)keyCode] = state;
	}
}
#pragma once
#include "ParrotScript.hpp"
using namespace Parrot;

class Controller3D : public Component::Script
{
public:
	float speed = 0.1f;
	float mSense = 0.001f;
private:
	Window* wndw;
	Component::Camera* cam;
public:
	using Script::Script;

	virtual void OnCreate() override
	{
		wndw = &sceneObj.GetScene().GetWindow();
		cam = &sceneObj.GetComponent<Component::Camera>();
	}

	virtual void OnUpdate() override
	{
		// camera rotation
		Math::Vec2i dMouse = wndw->GetCursorPos() - (Math::Vec2i)((Math::Vec2f)wndw->GetSize() * 0.5f);
		sceneObj.transform.rot.xy += (Math::Vec2f)dMouse * mSense;
		// lock Pitch between -90 to 90 degrees
		if (sceneObj.transform.rot.y > Math::PI_HALFS)
			sceneObj.transform.rot.y = Math::PI_HALFS;
		else if (sceneObj.transform.rot.y < -Math::PI_HALFS)
			sceneObj.transform.rot.y = -Math::PI_HALFS;

		if (GetKeyState(KeyCode::KEY_UP) != KeyState::Released)
			cam->foV += 0.01f;
		else if (GetKeyState(KeyCode::KEY_DOWN) != KeyState::Released)
			cam->foV -= 0.01f;
		
		// movement
		if (GetKeyState(KeyCode::KEY_W) != KeyState::Released)
			sceneObj.transform.pos += cam->Dir() * speed;
		if (GetKeyState(KeyCode::KEY_S) != KeyState::Released)
			sceneObj.transform.pos -= cam->Dir() * speed;

		if (GetKeyState(KeyCode::KEY_D) != KeyState::Released)
			sceneObj.transform.pos += cam->DirRight() * speed;
		if (GetKeyState(KeyCode::KEY_A) != KeyState::Released)
			sceneObj.transform.pos -= cam->DirRight() * speed;

		if (GetKeyState(KeyCode::KEY_SPACE) != KeyState::Released)
			sceneObj.transform.pos += cam->DirUp() * speed;
		if (GetKeyState(KeyCode::KEY_LEFT_CONTROL) != KeyState::Released)
			sceneObj.transform.pos -= cam->DirUp() * speed;

		wndw->SetCursorPos(wndw->GetSize() / 2); // center cursor
	}
};

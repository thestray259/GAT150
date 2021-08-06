#include "InputSystem.h"
#include <SDL.h>

namespace nc
{
	void InputSystem::Startup()
	{
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(&numKeys);
		keyboardState.resize(numKeys); 

		std::copy(keyboardStateSDL, keyboardStateSDL, keyboardState.begin()); 
		prevKeyboardState = keyboardState;
	}

	void InputSystem::Shutdown()
	{

	}

	void InputSystem::Update(float dt)
	{
		prevKeyboardState = keyboardState;

		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(nullptr); 
		std::copy(keyboardStateSDL, keyboardStateSDL, keyboardState.begin());
	}

	nc::InputSystem::eKeyState InputSystem::GetKeyState(int id)
	{
		eKeyState state = eKeyState::Idle;
		bool keyDown = IsKeyDown(id);
		bool prevKeyDown = IsPreviousKeyDown(id);

		if (keyDown) 
		{
			state = (prevKeyDown) ? eKeyState::Pressed : eKeyState::Held;
		}
		else 
		{ 
			state = (prevKeyDown) ? eKeyState::Held : eKeyState::Release;
		}

		return state;
	}

	bool InputSystem::IsKeyDown(int id)
	{
		//return <keyboard state at index[id]>;
		return keyboardState[id];
	}

	bool InputSystem::IsPreviousKeyDown(int id)
	{
		//return <previous keyboard state at index[id]>;
		return prevKeyboardState[id];
	}
}
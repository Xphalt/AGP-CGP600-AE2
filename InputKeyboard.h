#pragma once
#include <dinput.h>


class InputKeyboard
{
public:
	~InputKeyboard();

	HRESULT Initialise(HINSTANCE _hInstance, HWND _hwnd);
	void ReadInputStates();
	bool IsKeyPressed(unsigned char _keycode) { return m_keyboardKeyStates[_keycode] & 0x80; }

private:
	IDirectInput8* m_pDirectInput{ nullptr };
	IDirectInputDevice8* m_pKeyboardDevice{ nullptr };
	unsigned char m_keyboardKeyStates[256];
};
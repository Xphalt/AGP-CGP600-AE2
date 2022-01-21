#include "InputKeyboard.h"
InputKeyboard::~InputKeyboard()
{
    if (m_pKeyboardDevice) { m_pKeyboardDevice->Unacquire(); }
    if (m_pKeyboardDevice) { m_pKeyboardDevice->Release(); }
    if (m_pDirectInput) { m_pDirectInput->Release(); }

}

HRESULT InputKeyboard::Initialise(HINSTANCE _hInstance, HWND _hwnd)
{
    HRESULT hr;
    ZeroMemory(m_keyboardKeyStates, sizeof(m_keyboardKeyStates));
    hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);

    if (FAILED(hr)) { return hr; }

    hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL);

    if (FAILED(hr)) { return hr; }

    hr = m_pKeyboardDevice->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

    if (FAILED(hr)) { return hr; }

    hr = m_pKeyboardDevice->Acquire();

    if (FAILED(hr)) { return hr; }

    return S_OK;
}

void InputKeyboard::ReadInputStates()
{
    HRESULT hr;
    hr = m_pKeyboardDevice->GetDeviceState(sizeof(m_keyboardKeyStates), (LPVOID)&m_keyboardKeyStates);

    if (FAILED(hr)) { if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) { m_pKeyboardDevice->Acquire(); } }
}

void InputKeyboard::Update(GameObject& _gameObject)
{
    ReadInputStates();
    if (IsKeyPressed(DIK_W)) { _gameObject; }
    if (IsKeyPressed(DIK_S)) { _gameObject; }
    if (IsKeyPressed(DIK_A)) { _gameObject; }
    if (IsKeyPressed(DIK_D)) { _gameObject; }
}
#include "KeyboardInput.h"
#include <d3d11.h>
#include <dxgi.h>
#include <D3DX11.h>
#include <windows.h>
#include <dxerr.h>

KeyboardInput::~KeyboardInput()
{
    if (m_pKeyboardDevice) { m_pKeyboardDevice->Unacquire(); }
    if (m_pKeyboardDevice) { m_pKeyboardDevice->Release(); }
    if (m_pDirectInput) { m_pDirectInput->Release(); }

}

HRESULT KeyboardInput::Initialise(HINSTANCE _hInstance, HWND _hwnd)
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

void KeyboardInput::ReadInputStates()
{
    HRESULT hr;
    hr = m_pKeyboardDevice->GetDeviceState(sizeof(m_keyboardKeyStates), (LPVOID)&m_keyboardKeyStates);

    if (FAILED(hr)) { if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) { m_pKeyboardDevice->Acquire(); } }
}
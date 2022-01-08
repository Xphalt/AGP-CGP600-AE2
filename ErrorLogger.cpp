#pragma once
#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::LogError(std::string _errorMessage)
{
	std::string errorMessage = "Error: " + _errorMessage;
	MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_ICONERROR);
}

void ErrorLogger::LogError(HRESULT _hr, std::string _errorMessage)
{
	_com_error error(_hr);
	std::wstring errorMessage = L"Error: " + StringConverter::StringToWide(_errorMessage)
		+ L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, errorMessage.c_str(), L"Error", MB_ICONERROR);
}
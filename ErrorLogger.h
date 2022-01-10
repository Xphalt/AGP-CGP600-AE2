#pragma once
#include "StringConverter.h"
#include <Windows.h>

class ErrorLogger
{
public:
	static void LogError(std::string _errorMessage);
	static void LogError(HRESULT _hr, std::string _errorMessage);
};
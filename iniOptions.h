#pragma once
#ifndef INI_OPTIONS_H
#define INI_OPTIONS_H
#include "./inc/SimpleIni.h"

class IniOptions
{
protected:
	CSimpleIniA generalIni{};
	char keybind;
	int getIniValue(const char* value);
	IniOptions& loadOptions();
public:
	IniOptions();
	int getKeybind();
};
#endif

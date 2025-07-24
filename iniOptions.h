#pragma once
#ifndef INI_OPTIONS_H
#define INI_OPTIONS_H
#include "./inc/SimpleIni.h"

class IniOptions
{
protected:
	CSimpleIniA generalIni{};
	char spinKeybind;
	char holsterKeybind;
	bool allowMounted;
	int getIniValue(const char* value);
	bool getBoolIniValue(const char* value);
	IniOptions& loadOptions();
public:
	IniOptions();
	int getSpinKeybind();
	int getHolsterKeybind();
	bool getAllowMounted();
};
#endif

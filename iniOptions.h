#pragma once
#ifndef INI_OPTIONS_H
#define INI_OPTIONS_H
#include "./inc/SimpleIni.h"

class IniOptions
{
protected:
	CSimpleIniA generalIni{};
	long long spinKeybind;
	long long holsterKeybind;
	bool allowMounted;
	long long getIniValue(const char* value);
	bool getBoolIniValue(const char* value);
	IniOptions& loadOptions();
public:
	IniOptions();
	long long getSpinKeybind();
	long long getHolsterKeybind();
	bool getAllowMounted();
};
#endif

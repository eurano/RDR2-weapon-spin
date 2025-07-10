#include "IniOptions.h"

IniOptions::IniOptions()
{
	this->keybind = 0x54;
	this->loadOptions();
}

int IniOptions::getIniValue(const char* value) {
	if (value != nullptr) {
		return std::atoi(value);
	}
	else {
		return this->keybind;
	}
}

IniOptions& IniOptions::loadOptions()
{
	SI_Error siError = this->generalIni.LoadFile("Spin.ini");
	if (siError < 0) throw "Error!Could not find the ini file";

	this->keybind = this->getIniValue(this->generalIni.GetValue("GENERAL", "keybind"));

	return *this;
}

int IniOptions::getKeybind()
{
	return this->keybind;
}

#include "IniOptions.h"

IniOptions::IniOptions()
{
	this->spinKeybind = 0x54;
	this->holsterKeybind = 0x09;
	this->allowMounted = false;
	this->loadOptions();
}

int IniOptions::getIniValue(const char* value) {
	if (value != nullptr) {
		return std::atoi(value);
	}
}

bool IniOptions::getBoolIniValue(const char* value) {
	return (value != nullptr && std::string(value) == "true");
}

IniOptions& IniOptions::loadOptions()
{
	SI_Error siError = this->generalIni.LoadFile("Spin.ini");
	if (siError < 0) throw "Error!Could not find the ini file";

	this->spinKeybind = this->getIniValue(this->generalIni.GetValue("GENERAL", "spinKeybind", "0x54"));
	this->holsterKeybind = this->getIniValue(this->generalIni.GetValue("GENERAL", "holsterKeybind", "0x09"));
	this->allowMounted = this->getBoolIniValue(this->generalIni.GetValue("GENERAL", "allowMounted", "false"));

	return *this;
}

int IniOptions::getSpinKeybind()
{
	return this->spinKeybind;
}

int IniOptions::getHolsterKeybind()
{
	return this->holsterKeybind;
}

bool IniOptions::getAllowMounted()
{
	return this->allowMounted;
}

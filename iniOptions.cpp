#include "IniOptions.h"
#include <cstdlib> 

IniOptions::IniOptions()
{
	this->spinKeybind = 3350541322;
	this->holsterKeybind = 3901091606;
	this->allowMounted = false;
	this->loadOptions();
}

long long IniOptions::getIniValue(const char* value) {
	char* endPtr;
	if (value != nullptr) {
		return strtoll(value, &endPtr, 10);
	}
}

bool IniOptions::getBoolIniValue(const char* value) {
	return (value != nullptr && std::string(value) == "true");
}

IniOptions& IniOptions::loadOptions()
{
	SI_Error siError = this->generalIni.LoadFile("Spin.ini");
	if (siError < 0) throw "Error!Could not find the ini file";

	this->spinKeybind = this->getIniValue(this->generalIni.GetValue("GENERAL", "spinKeybind", "3350541322"));
	this->holsterKeybind = this->getIniValue(this->generalIni.GetValue("GENERAL", "holsterKeybind", "3901091606"));
	this->allowMounted = this->getBoolIniValue(this->generalIni.GetValue("GENERAL", "allowMounted", "false"));

	return *this;
}

long long IniOptions::getSpinKeybind()
{
	return this->spinKeybind;
}

long long IniOptions::getHolsterKeybind()
{
	return this->holsterKeybind;
}

bool IniOptions::getAllowMounted()
{
	return this->allowMounted;
}

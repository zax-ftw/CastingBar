#include "Settings.h"

using namespace SKSE;

void GetValue(CSimpleIniA& a_ini, int& a_value, const char* a_section,
	const char* a_key, const char* a_comment = nullptr)
{
	a_value = static_cast<int>(a_ini.GetLongValue(a_section, a_key, a_value));
	a_ini.SetLongValue(a_section, a_key, a_value, a_comment);
}

void GetValue(CSimpleIniA& a_ini, float& a_value, const char* a_section,
	const char* a_key, const char* a_comment = nullptr)
{
	a_value = static_cast<float>(a_ini.GetDoubleValue(a_section, a_key, a_value));
	a_ini.SetDoubleValue(a_section, a_key, a_value, a_comment);
}

void GetValue(CSimpleIniA& a_ini, bool& a_value, const char* a_section,
	const char* a_key, const char* a_comment = nullptr)
{
	a_value = a_ini.GetBoolValue(a_section, a_key, a_value);
	a_ini.SetBoolValue(a_section, a_key, a_value, a_comment);
};

void GetValue(CSimpleIniA& a_ini, std::string& a_value, const char* a_section,
	const char* a_key, const char* a_comment = nullptr)
{
	a_value = a_ini.GetValue(a_section, a_key, a_value.c_str());
	a_ini.SetValue(a_section, a_key, a_value.c_str(), a_comment);
};

void Settings::ReadSettings()
{
	logger::info("Reading settings...");

	auto plugin = PluginDeclaration::GetSingleton();
	auto path = fmt::format("Data\\SKSE\\Plugins\\{}.ini", plugin->GetName());

    CSimpleIniA ini;
    ini.SetUnicode();

    ini.LoadFile(path.c_str());

	GetValue(ini, EnableForSpell, "General", "EnableForSpell", ";Enable the widget for spell casting");
	GetValue(ini, EnableForShout, "General", "EnableForShout", ";Enable the widget for shouting");
	GetValue(ini, EnableForBow, "General", "EnableForBow", ";Enable the widget for bow drawing");

	GetValue(ini, WidgetPositionX, "Position", "WidgetPositionX", ";The X Position for the widget");
    GetValue(ini, WidgetPositionY, "Position", "WidgetPositionY", ";The Y Position for the widget");
    GetValue(ini, WidgetScaleX, "Position", "WidgetScaleX", ";The X scale of the widget");
    GetValue(ini, WidgetScaleY, "Position", "WidgetScaleY", ";The Y scale of the widget");

    ini.SaveFile(path.c_str());
}

#include "Settings.h"

using namespace SKSE;

void Settings::Load()
{
    constexpr auto path = L"Data/SKSE/Plugins/CastingBar.ini";

    CSimpleIniA ini;
    ini.SetUnicode();

    ini.LoadFile(path);

    GetValue(ini, flashWhenCharged, "Widget", "bFlashWhenCharged", ";Casting bar flashes when fully charged");
    GetValue(ini, widgetXPosition, "Widget", "fPositionX", ";The X Position for the widget");
    GetValue(ini, widgetYPosition, "Widget", "fPositionY", ";The Y Position for the widget");
    GetValue(ini, widgetXScale, "Widget", "fScaleX", ";The X scale of the widget");
    GetValue(ini, widgetYScale, "Widget", "fScaleY", ";The Y scale of the widget");

    ini.SaveFile(path);
}

void Settings::GetValue(CSimpleIniA& a_ini, std::uint32_t& a_value, const char* a_section, 
    const char* a_key, const char* a_comment)
{
    a_value = static_cast<std::uint32_t>(a_ini.GetLongValue(a_section, a_key, a_value));
    a_ini.SetLongValue(a_section, a_key, a_value, a_comment);
}

void Settings::GetValue(CSimpleIniA& a_ini, float& a_value, const char* a_section, 
    const char* a_key, const char* a_comment)
{
    a_value = static_cast<float>(a_ini.GetDoubleValue(a_section, a_key, a_value));
    a_ini.SetDoubleValue(a_section, a_key, a_value, a_comment);
}

void Settings::GetValue(CSimpleIniA& a_ini, bool& a_value, const char* a_section, 
    const char* a_key, const char* a_comment)
{
    a_value = a_ini.GetBoolValue(a_section, a_key, a_value);
    a_ini.SetBoolValue(a_section, a_key, a_value, a_comment);
};

void Settings::GetValue(CSimpleIniA& a_ini, std::string& a_value, const char* a_section, 
    const char* a_key, const char* a_comment)
{
    a_value = a_ini.GetValue(a_section, a_key, a_value.c_str());
    a_ini.SetValue(a_section, a_key, a_value.c_str(), a_comment);
};

#include "Settings.h"

using namespace SKSE;

void Settings::Load()
{
    constexpr auto path = L"Data/SKSE/Plugins/CastingBar.ini";

    CSimpleIniA ini;
    ini.SetUnicode();

    ini.LoadFile(path);

    GetValue(ini, flashWhenCharged, "Widget", "bFlashWhenCharged",";Casting bar flashes when the spell is fully charged");
    GetValue(ini, widgetXPosition, "Widget", "fPositionX", ";The X Position for the widget, default is 961.000000");
    GetValue(ini, widgetYPosition, "Widget", "fPositionY", ";The Y Position for the widget, default is 1040.000000");
    GetValue(ini, widgetXScale, "Widget", "fScaleX", ";The X scale of the widget, default is 75.000000");
    GetValue(ini, widgetYScale, "Widget", "fScaleY", ";The Y scale of the widget, default is 75.000000");

    ini.SaveFile(path);
}

void Settings::GetValue(CSimpleIniA& a_ini, std::uint32_t& a_value, const char* a_section, 
    const char* a_key, const char* a_comment)
{
    try {
        a_value = stl::string::lexical_cast<std::uint32_t>(a_ini.GetValue(a_section, a_key, "0"));
        a_ini.SetValue(a_section, a_key, std::to_string(a_value).c_str(), a_comment);
    } catch (...) {

    }
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
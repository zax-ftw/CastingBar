#pragma once

class Settings
{
public:
	[[nodiscard]] static Settings* GetSingleton()
	{
		static Settings singleton;
		return std::addressof(singleton);
	}

	void Load();

	bool flashWhenCharged{ true };
	float widgetXPosition{ 961.0f };
	float widgetYPosition{ 1040.0f };
	float widgetXScale{ 75.0f };
	float widgetYScale{ 75.0f };

private:
	void GetValue(CSimpleIniA& a_ini, std::uint32_t& a_value, const char* a_section, const char* a_key, const char* a_comment);
	void GetValue(CSimpleIniA& a_ini, float& a_value, const char* a_section, const char* a_key, const char* a_comment);
	void GetValue(CSimpleIniA& a_ini, bool& a_value, const char* a_section, const char* a_key, const char* a_comment);
    void GetValue(CSimpleIniA& a_ini, std::string& a_value, const char* a_section, const char* a_key, const char* a_comment);
};

#pragma once

struct Settings
{
	static void ReadSettings();

	// General
	static inline bool EnableForSpell = true;
	static inline bool EnableForShout = true;
	static inline bool EnableForBow = false;

	// Position
	static inline float WidgetPositionX = 961.0f;
	static inline float WidgetPositionY = 1040.0f;
	static inline float WidgetScaleX = 75.0f;
	static inline float WidgetScaleY = 75.0f;
};

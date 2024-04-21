#pragma once
#include <SimpleIni.h>
#include <robin_hood.h>
#include "Utils.h"

class Settings
{
public:
	[[nodiscard]] static Settings* GetSingleton();

	void Load();

	enum SkyrimFont : int
	{
		NoFont = -1,
		EverywhereFont,
		EverywhereMediumFont,
		EverywhereBoldFont,
		HandwrittenFont,
		SkyrimBooks,
		DaedricFont,
		MageScriptFont,
		DwemerFont,
		FalmerFont,
		DragonFont,
		SkyrimBooks_UnreadableFont,
		SkyrimSymbolsFont,
		ConsoleFont,
	};

	const char* GetFontColor() {
		if (tweaks.color == "") {
			return "";
		} else {
			return std::format("<font color='{}'>", tweaks.color).c_str();
		}
	}

	const char* GetFontSize()
	{
		if (tweaks.size == -1) {
			return "";
		} else {
			return std::format("<font size='{}'>", tweaks.size).c_str();
		}
	}

	const char* GetFontFace() {
		switch (tweaks.font) {
		case EverywhereFont:
			return "<font face='$EverywhereFont'>";
		case EverywhereMediumFont:
			return "<font face='$EverywhereMediumFont'>";
		case EverywhereBoldFont:
			return "<font face='$EverywhereBoldFont'>";
		case HandwrittenFont:
			return "<font face='$HandwrittenFont'>";
		case SkyrimBooks:
			return "<font face='$SkyrimBooks'>";
		case DaedricFont:
			return "<font face='$DaedricFont'>";
		case MageScriptFont:
			return "<font face='$MageScriptFont'>";
		case DwemerFont:
			return "<font face='$DwemerFont'>";
		case FalmerFont:
			return "<font face='$FalmerFont'>";
		case DragonFont:
			return "<font face='$DragonFont'>";
		case SkyrimBooks_UnreadableFont:
			return "<font face='$SkyrimBooks_UnreadableFont'>";
		case SkyrimSymbolsFont:
			return "<font face='$SkyrimSymbolsFont'>";
		case ConsoleFont:
			return "<font face='$ConsoleFont'>";
		default:
			return "";
		}
	}

	const char* GetAlignment() {
		if (tweaks.alignment == "") {
			return "";
		} else {
			return std::format("<p align='{}'>", tweaks.alignment).c_str();
		}
	}

	const char* GetFontString()
	{
		std::string* font = new std::string(GetFontFace());
		font->append(GetFontColor());
		font->append(GetFontSize());
		font->append(GetAlignment());
		return font->c_str();
	}

	struct Tweaks
	{
		void Load(CSimpleIniA& a_ini);

		std::string prefix{ "" };
		std::string suffix{ "" };
		SkyrimFont font{ -1 };
		std::string color{ "" };
		int size{ -1 };
		std::string alignment{ "" };
	} tweaks;

private:
	struct detail
	{
		template <class T>
		static void get_value(CSimpleIniA& a_ini, T& a_value, const char* a_section, const char* a_key, const char* a_comment)
		{
			
			if constexpr (std::is_same_v<bool, T>) {
				a_value = a_ini.GetBoolValue(a_section, a_key, a_value);
				a_ini.SetBoolValue(a_section, a_key, a_value, a_comment);

				GetSingleton()->settingsMap.emplace(a_key, a_value);
			} else if constexpr (std::is_floating_point_v<T>) {
				a_value = static_cast<T>(a_ini.GetDoubleValue(a_section, a_key, a_value));
				a_ini.SetDoubleValue(a_section, a_key, a_value, a_comment);

				GetSingleton()->settingsMap.emplace(a_key, a_value != 1.0);
			} else if constexpr (std::is_arithmetic_v<T> || std::is_enum_v<T>) {
				a_value = utils::stl::string::lexical_cast<T>(a_ini.GetValue(a_section, a_key, std::to_string(a_value).c_str()));
				a_ini.SetValue(a_section, a_key, std::to_string(a_value).c_str(), a_comment);

				GetSingleton()->settingsMap.emplace(a_key, a_value != 0);
			} else {
				a_value = a_ini.GetValue(a_section, a_key, a_value.c_str());
				a_ini.SetValue(a_section, a_key, a_value.c_str(), a_comment);
			}
		}
	};

	robin_hood::unordered_flat_map<std::string, bool> settingsMap{};
};

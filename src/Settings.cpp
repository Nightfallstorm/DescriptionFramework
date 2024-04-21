#include "Settings.h"

Settings* Settings::GetSingleton()
{
	static Settings singleton;
	return std::addressof(singleton);
}

void Settings::Load()
{
	constexpr auto path = L"Data/SKSE/Plugins/DescriptionFramework.ini";

	CSimpleIniA ini;
	ini.SetUnicode();

	ini.LoadFile(path);

	//TWEAKS
	tweaks.Load(ini);

	ini.SaveFile(path);
}

void Settings::Tweaks::Load(CSimpleIniA& a_ini)
{
	const char* section = "Tweaks";

	detail::get_value(a_ini, prefix, section, "sPrefix", ";Prefix for each description (ex: MyCustomPrefix = \"MyCustomPrefixOriginalDescription\"");
	detail::get_value(a_ini, suffix, section, "sSuffix", ";Suffix for each description (ex: MyCustomSuffix = \"OriginalDescriptionMyCustomSuffix\"");
	detail::get_value(a_ini, font, section, "iFont", ";Custom built-in skyrim font choice to apply to each description\n;Font choices: NoFont(-1, no custom font change), EverywhereFont(0),EverywhereMediumFont(1),EverywhereBoldFont(2)\n;HandwrittenFont(3),SkyrimBooks(4),DaedricFont(5), MageScriptFont(6), DwemerFont(7), FalmerFont(8), DragonFont(9),\n;SkyrimBooks_UnreadableFont(10),SkyrimSymbolsFont(11),ConsoleFont(12)\n;Note: Prefixes/suffixes will also receive the font choice specified here\n;Note: Some custom descriptions can overwrite the font choice if their description also specifies a font\n;Ex: \"<font face='$MY_FONT_HERE'>Some custom description</font>\" will overwrite any font choice specified here\n;Note: Daedric,MageScript,Dwemer,Falmer and Dragon use custom symbols, and are NOT readable without translation\n;Note: SkyrimBooks_UnreadableFont and SkyrimSymbolsFont are completely unreadable\n;-1 for default");
	detail::get_value(a_ini, color, section, "sColor", ";Custom font color for each description in the form of a color hex (ex: #FFFFFF is white, #000000 = black)\n;For a color picker website, try https://www.w3schools.com/colors/colors_picker.asp\n;Leave empty for default");
	detail::get_value(a_ini, size, section, "iSize", ";Custom font size for each description; -1 for default");



	logger::info("Loaded settings");
	logger::info("prefix={}", prefix);
	logger::info("suffix={}", suffix);
	logger::info("font={}", font);
	logger::info("color={}", color);
	logger::info("size={}", size);
}

#pragma once

namespace utils
{
	using _GetFormEditorID = const char* (*)(std::uint32_t);

	std::vector<std::string> split_string(std::string& a_string, char a_delimiter);

	std::string GetFormEditorID(const RE::TESForm* a_form);
}


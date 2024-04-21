#pragma once

namespace utils
{
	using _GetFormEditorID = const char* (*)(std::uint32_t);

	std::vector<std::string> split_string(std::string& a_string, char a_delimiter);

	std::string GetFormEditorID(const RE::TESForm* a_form);

	namespace stl
	{
		namespace string
		{
			template <class T>
			T lexical_cast(const std::string& a_str, bool a_hex = false)
			{
				auto base = a_hex ? 16 : 10;

				if constexpr (std::is_floating_point_v<T>) {
					return static_cast<T>(std::stof(a_str));
				} else if constexpr (std::is_signed_v<T>) {
					return static_cast<T>(std::stoi(a_str, nullptr, base));
				} else if constexpr (sizeof(T) == sizeof(std::uint64_t)) {
					return static_cast<T>(std::stoull(a_str, nullptr, base));
				} else {
					return static_cast<T>(std::stoul(a_str, nullptr, base));
				}
			}
		}
	}
}


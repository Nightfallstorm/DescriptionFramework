#pragma once
#include <utility>

class ConfigurationDatabase
{
public:
	inline static ConfigurationDatabase*& GetSingleton()
	{
		static ConfigurationDatabase* _this_database = nullptr;
		if (!_this_database)
			_this_database = new ConfigurationDatabase();
		return _this_database;
	}

	/*
	@brief Safely de-allocate the memory space used by DataBase.
	*/
	static void Dealloc()
	{
		delete GetSingleton();
		GetSingleton() = nullptr;
	}

	// Load entries from the various entry files
	void Initialize();

	// Get object's description. Script descriptions take priority over descriptions from configuration files
	std::string GetDescriptionForObject(RE::TESBoundObject* a_object);

	// Get object's description if it was set by a script
	std::string GetScriptDescriptionForObject(RE::TESBoundObject* a_object);

	// Set description from papyrus
	void SetScriptDescriptionForObject(RE::TESBoundObject* a_object, std::string a_description);

	// Set from papyrus by a mod
	void ResetScriptDescriptionnForObject(RE::TESBoundObject* a_object);

private:
	void parseConfigs(std::filesystem::path configFile);
	void parseLine(std::string line);
	std::map<RE::FormID, std::pair<std::string, int>> tempMap;  // Temporary map while populating configs
	std::map<RE::FormID, std::string> descriptionMap;                      // Map of descriptions as determined by initial configuration files
	std::map<RE::FormID, std::string> scriptDescriptionMap;                // Map of descriptions as set from Papyrus at runtime, these always take priority
};

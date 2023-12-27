#pragma once
#include <utility>

struct DescriptionConfiguration
{
	std::string description = "";
	int priority = 0;
};


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

	DescriptionConfiguration* GetConfigurationForObject(RE::TESBoundObject* a_object);

private:
	void parseConfigs(std::filesystem::path configFile);
	void parseLine(std::string line);
	std::map<RE::FormID, DescriptionConfiguration*> descriptionMap;
};


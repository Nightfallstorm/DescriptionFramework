#pragma once
#include <utility>

struct DescriptionConfiguration
{
	RE::TESBoundObject* object;
	std::string description;
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
	std::vector<DescriptionConfiguration*> entries;
};


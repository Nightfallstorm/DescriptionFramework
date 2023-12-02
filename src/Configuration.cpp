#pragma once
#include "Configuration.h"
#include "Utils.h"
#include "MergeMapperPluginAPI.h"


std::string sanitizeLine(std::string line) {
	auto parsingLine = std::string(line);
	// Strip the comments and the whitespace
	if (parsingLine.find('#') != std::string::npos) {
		parsingLine.erase(parsingLine.find('#'));
	}

	return parsingLine;
}

template <class T>
T* GetFormFromString(std::string line)
{
	auto form = RE::TESForm::LookupByEditorID(line);
	if (form && form->As<T>()) {
		return form->As<T>();
	}

	if (line.find('~') == std::string::npos) {
		logger::error("		missing plugin: {}", line);
		return nullptr;
	}

	auto plugin = line.substr(line.find('~') + 1);
	RE::FormID formID = std::stoul(line.substr(0, line.find('~')), nullptr, 16);
	if (g_mergeMapperInterface) {
		auto mergeForm = g_mergeMapperInterface->GetNewFormID(plugin.c_str(), formID);
		plugin = mergeForm.first;
		formID = mergeForm.second;
	}

	form = RE::TESDataHandler::GetSingleton()->LookupForm(formID, plugin);
	if (form == nullptr) {
		logger::error("		invalid form ID: {}", line);
		return nullptr;
	}

	if (form->As<T>()) {
		return form->As<T>();
	}
	return nullptr;
}

void ConfigurationDatabase::parseConfigs(std::filesystem::path configFile)
{
	logger::info("Parsing file {}", configFile.string().c_str());
	std::fstream config;
	config.open(configFile, std::ios::in);
	if (!config.is_open()) {
		logger::error("Couldn't open file {}", configFile.string().c_str());
		return;
	}
	std::string line;
	while (std::getline(config, line)) {
		line = sanitizeLine(line);
		if (line.empty()) {
			continue;
		}
		logger::info("	Parsing line {}", line);
		auto tokens = utils::split_string(line, '|');
		if (tokens.size() < 2) {
			logger::error("		Invalid line setup, not enough info!");
			continue;
		}
		auto object = GetFormFromString<RE::TESBoundObject>(tokens[0]);
		auto& description = tokens[1];
		int priority = 0;
		if (tokens.size() >= 3) {
			priority = std::stoi(tokens[2]);
		}
		if (!object || description.size() <= 0) {
			continue;
		}

		DescriptionConfiguration* config = new DescriptionConfiguration();
		config->description = description;
		config->priority = priority;

		if (descriptionMap.contains(object->formID) && descriptionMap[object->formID]->priority >= priority) {
			auto desc = descriptionMap[object->formID];
			logger::info("		Overwritten by \'{}\' with priority {}", desc->description, desc->priority);
			continue;
		}

		if (descriptionMap.contains(object->formID) && descriptionMap[object->formID]->priority < priority) {
			auto desc = descriptionMap[object->formID];
			logger::info("		Overwriting previous entry \'{}\' with priority {}", desc->description, desc->priority);
		}

		descriptionMap[object->formID] = config;
		logger::info("		Entry inserted successfully!");
	}
}

void ConfigurationDatabase::Initialize() {
	logger::info("Reading descriptions configs...");

	constexpr auto path = L"Data";

	std::vector<std::filesystem::path> filePaths;
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		try {
			if (!entry.path().filename().string().ends_with("_DESC.ini")) {
				continue;
			}
			 
			filePaths.push_back(entry.path());
		} catch(...) {
			continue;
		}		
	}

	std::sort(filePaths.begin(), filePaths.end());

	for (const auto& entry : filePaths) {
		try {
			logger::info("Parsing {}", entry.string());
			parseConfigs(entry);
		} catch (...) {
			logger::error("Error parsing {}", entry.string());
			continue;
		}	
	}

	logger::info("Config APIs fully parsed!");
}

DescriptionConfiguration* ConfigurationDatabase::GetConfigurationForObject(RE::TESBoundObject* a_object) {
	if (!a_object) {
		return nullptr;
	}

	if (descriptionMap.contains(a_object->formID)) {
		return descriptionMap[a_object->formID];
	}

	return nullptr;
}

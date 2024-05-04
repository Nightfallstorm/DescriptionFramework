#include "Papyrus.h"
#include "Configuration.h"

namespace Papyrus
{
	inline std::vector<std::int32_t> GetVersion(RE::StaticFunctionTag*)
	{
		return { Version::MAJOR, Version::MINOR, Version::PATCH };
	}

	inline RE::TESBoundObject* GetBaseObject(RE::TESForm* a_object)
	{
		if (a_object->Is(RE::FormType::Reference)) {
			return a_object->As<RE::TESObjectREFR>()->GetBaseObject();
		}	

		if (a_object->As<RE::TESBoundObject>()) {
			return a_object->As<RE::TESBoundObject>();
		}

		return nullptr;
	}

	RE::BSFixedString GetDescription(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_object) {
		auto a_base = GetBaseObject(a_object);
		if (!a_base) {
			a_vm->TraceStack("Base object is empty", a_stackID);
		}
		return ConfigurationDatabase::GetSingleton()->GetDescriptionForObject(a_base);
	}

	RE::BSFixedString GetScriptedDescription(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_object)
	{
		auto a_base = GetBaseObject(a_object);
		if (!a_base) {
			a_vm->TraceStack("Base object is empty", a_stackID);
		}
		return ConfigurationDatabase::GetSingleton()->GetScriptDescriptionForObject(a_base);
	}

	void SetDescription(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_object, RE::BSFixedString a_desc)
	{
		auto a_base = GetBaseObject(a_object);
		if (!a_base) {
			a_vm->TraceStack("Base object is empty", a_stackID);
		}

		ConfigurationDatabase::GetSingleton()->SetScriptDescriptionForObject(a_base, std::string(a_desc));
	}

	void ResetDescription(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESForm* a_object)
	{
		auto a_base = GetBaseObject(a_object);
		if (!a_base) {
			a_vm->TraceStack("Base object is empty", a_stackID);
		}

		ConfigurationDatabase::GetSingleton()->ResetScriptDescriptionnForObject(a_base);
	}

	bool Bind(VM* a_vm)
	{
		if (!a_vm) {
			logger::critical("couldn't get VM State"sv);
			return false;
		}

		logger::info("Binding functions..."sv);

		
		BIND(GetVersion, true); 

		logger::info("Registered GetVersion"sv);

		BIND(GetDescription);

		logger::info("Registed GetDescription"sv);

		BIND(GetScriptedDescription);

		logger::info("Registed GetScriptedDescription"sv);

		BIND(SetDescription);

		logger::info("Registed SetDescription"sv);

		BIND(ResetDescription);

		logger::info("Registed ResetDescription"sv);

		return true;
	}
}

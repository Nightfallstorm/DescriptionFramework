#pragma once

namespace Papyrus
{
	#define BIND(a_method, ...) a_vm->RegisterFunction(#a_method##sv, script, a_method __VA_OPT__(, ) __VA_ARGS__)
	using VM = RE::BSScript::Internal::VirtualMachine;
	using StackID = RE::VMStackID;

	inline constexpr auto script = "DescriptionFramework"sv;

	bool Bind(VM* a_vm);
}

#pragma once
#include "Configuration.h"

namespace hooks
{
	static inline RE::TESObjectBOOK* book;
	static inline RE::TESObjectMISC* currentForm;

	struct DescriptionHook
	{
		static void thunk(RE::TESDescription* a_self, RE::BSString& a_out, RE::TESForm* a_parent, std::uint32_t a_fieldType)
		{
			if (a_parent != book || !ConfigurationDatabase::GetSingleton()->GetConfigurationForObject(currentForm)) {
				return func(a_self, a_out, a_parent, a_fieldType);
			}
			auto& description = ConfigurationDatabase::GetSingleton()->GetConfigurationForObject(currentForm)->description;
			a_out = description;
		};

		static inline REL::Relocation<decltype(thunk)> func;

		// Install our hook at the specified address
		static inline void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(51019, 51897), REL::VariantOffset(0xE46, 0x1002, 0xD64) };
			stl::write_thunk_call<DescriptionHook>(target.address());

			logger::info("DescriptionHook hooked at address {:x}", target.address());
			logger::info("DescriptionHook hooked at offset {:x}", target.offset());
		}
	};

	struct ItemCardPopulateHook
	{
		static void thunk(RE::ItemCard* itemCard, RE::TESBoundObject** a_item, char a3)
		{
			if (!(*a_item)->Is(RE::FormType::Misc)) {
				return func(itemCard, a_item, a3);
			}
			initializeBook((*a_item)->As<RE::TESObjectMISC>());
			auto orig = *a_item;
			*a_item = book;
			func(itemCard, a_item, a3);
			*a_item = orig;
		};

		static inline void initializeBook(RE::TESObjectMISC* a_item)
		{
			book = RE::TESForm::LookupByID(0xb7736)->As<RE::TESObjectBOOK>();
			book->weight = a_item->weight;
			book->fullName = a_item->fullName;
			book->value = a_item->value;
			currentForm = a_item;
		}

		static inline REL::Relocation<decltype(thunk)> func;

		// Install our hook at the specified address
		static inline void Install()
		{
			REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(50005, 50949), REL::VariantOffset(0x80, 0x80, 0x80) };
			stl::write_thunk_call<ItemCardPopulateHook>(target.address());

			logger::info("ItemCardPopulateHook hooked at address {:x}", target.address());
			logger::info("ItemCardPopulateHook hooked at offset {:x}", target.offset());

			REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(50201, 51130), REL::VariantOffset(0xB2, 0xB2, 0xB2) };
			stl::write_thunk_call<ItemCardPopulateHook>(target2.address());

			logger::info("ItemCardPopulateHook hooked at address {:x}", target2.address());
			logger::info("ItemCardPopulateHook hooked at offset {:x}", target2.offset());

			REL::Relocation<std::uintptr_t> target3{ RELOCATION_ID(50297, 51218), REL::VariantOffset(0x35, 0x35, 0x35) };
			stl::write_thunk_call<ItemCardPopulateHook>(target3.address());

			logger::info("ItemCardPopulateHook hooked at address {:x}", target3.address());
			logger::info("ItemCardPopulateHook hooked at offset {:x}", target3.offset());

			REL::Relocation<std::uintptr_t> target4{ RELOCATION_ID(50674, 51569), REL::VariantOffset(0x80, 0x7A, 0x80) };
			stl::write_thunk_call<ItemCardPopulateHook>(target4.address());

			logger::info("ItemCardPopulateHook hooked at address {:x}", target4.address());
			logger::info("ItemCardPopulateHook hooked at offset {:x}", target4.offset());

			REL::Relocation<std::uintptr_t> target5{ RELOCATION_ID(50973, 51852), REL::VariantOffset(0x80, 0x7A, 0x80) };
			stl::write_thunk_call<ItemCardPopulateHook>(target5.address());

						

			logger::info("ItemCardPopulateHook hooked at address {:x}", target5.address());
			logger::info("ItemCardPopulateHook hooked at offset {:x}", target5.offset());

			if (REL::Module::IsAE()) {
				REL::Relocation<std::uintptr_t> target6{ RELOCATION_ID(0, 51458), REL::VariantOffset(0x0, 0x87, 0x0) };
				stl::write_thunk_call<ItemCardPopulateHook>(target6.address());

				logger::info("ItemCardPopulateHook hooked at address {:x}", target6.address());
				logger::info("ItemCardPopulateHook hooked at offset {:x}", target6.offset());
			}
		}
	};

	static inline void InstallHooks()
	{
		ItemCardPopulateHook::Install();
		DescriptionHook::Install();
	}
}

#pragma once
#include "Configuration.h"
#include "Settings.h"

namespace hooks
{
	struct ItemCardPopulateHook
	{
		static inline std::uint32_t ICT_ARMOR = 1;
		static inline std::uint32_t ICT_BOOK = 4;
		static inline std::uint32_t ICT_POTION = 6;
		static inline auto htmlRegex = std::regex(".*<.*>");

		static void thunk(RE::ItemCard* itemCard, RE::TESBoundObject** a_item, char a3)
		{
			if (!itemCard || !a_item || !*a_item || getDescription(*a_item) == "") {
				return func(itemCard, a_item, a3);
			}

			func(itemCard, a_item, a3);
			handleMiscItems(itemCard, *a_item);
			handleSoulGems(itemCard, *a_item);
			handleBooks(itemCard, *a_item);
			handleIngredients(itemCard, *a_item);
			handleEffectsItems(itemCard, *a_item);
			handleDescriptionItems(itemCard, *a_item);
		};

		static const char* getDescription(RE::TESBoundObject* a_item)
		{
			auto settings = Settings::GetSingleton();
			auto database = ConfigurationDatabase::GetSingleton();
			if (!database->GetConfigurationForObject(a_item)) {
				return "";
			}
			auto desc = database->GetConfigurationForObject(a_item)->description.c_str();
			auto prefix = settings->tweaks.prefix != "" ? settings->tweaks.prefix + " " : "";
			auto suffix = settings->tweaks.suffix != "" ? " " + settings->tweaks.suffix : "";
			auto font = settings->GetFontString();

			std::string* description = new std::string(font);
			description->append(prefix);
			description->append(desc);
			description->append(suffix);
			logger::info("Using description {}", description->c_str());
			return description->c_str();
		}

		static void handleMiscItems(RE::ItemCard* itemCard, RE::TESBoundObject* a_item)
		{
			auto newType = ICT_BOOK;  // Fake the MISC object as a BOOK so the UI will load the description label
			auto desc = std::string(getDescription(a_item));
			if (desc.size() > 50 || std::regex_match(desc, htmlRegex)) {
				// Use potion's HTML text instead for the auto shrink on large texts and for HTML stuff
				// Note: On small texts the text is misaligned :(
				logger::info("Using html on {}", desc);
				newType = ICT_POTION;
			}
			if (a_item->Is(RE::FormType::Misc) || a_item->Is(RE::FormType::Light) || a_item->Is(RE::FormType::KeyMaster)) {
				auto typeValue = RE::GFxValue(newType);
				itemCard->obj.SetMember("type", typeValue);
			}
		}

		static void handleEffectsItems(RE::ItemCard* itemCard, RE::TESBoundObject* a_item)
		{
			// Potions, Food (Plus any items faked as a potion/food)
			RE::GFxValue origEffect = RE::GFxValue("");
			std::string origEffectDesc = "";
			if (itemCard->obj.HasMember("effects") && itemCard->obj.GetMember("effects", &origEffect)) {
				std::string origString = origEffect.GetString();
				origString.erase(remove(origString.begin(), origString.end(), ' '), origString.end());
				if (!origString.empty()) {
					origEffectDesc = std::string(origEffect.GetString()) + "\n";
				}
			}
			auto effectsDesc = origEffectDesc + getDescription(a_item);
			auto effectsValue = RE::GFxValue(effectsDesc);
			itemCard->obj.SetMember("effects", effectsValue);
		}

		static void handleDescriptionItems(RE::ItemCard* itemCard, RE::TESBoundObject* a_item)
		{
			// Books (Plus any items faked as a book)
			auto descriptionValue = RE::GFxValue(getDescription(a_item));
			itemCard->obj.SetMember("description", descriptionValue);
		}

		static void handleIngredients(RE::ItemCard* itemCard, RE::TESBoundObject* a_item)
		{
			// TODO: Trick ingredients as a potion, and mimic the same 4 effect labels with a description?
			if (a_item->Is(RE::FormType::Ingredient)) {
			}
		}

		static void handleSoulGems(RE::ItemCard* itemCard, RE::TESBoundObject* a_item)
		{
			// TODO: Trick soul gems as a potion to get HTML text benefit?
			if (a_item->Is(RE::FormType::SoulGem)) {
			}
		}

		static void handleBooks(RE::ItemCard* itemCard, RE::TESBoundObject* a_item)
		{
			if (a_item->Is(RE::FormType::Book)) {
				auto desc = std::string(getDescription(a_item));
				if (desc.size() > 50 || std::regex_match(desc, htmlRegex)) {
					logger::info("Using html on book {}", desc);
					auto typeValue = RE::GFxValue(ICT_POTION);
					itemCard->obj.SetMember("type", typeValue);
				}
			}
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

	namespace MenuHooks {
		
		struct InventoryMenuHook
		{
			static void thunk(RE::IMenu* a_menu, float a_interval, std::uint32_t a_currentTime)
			{
				func(a_menu, a_interval, a_currentTime);
				//logger::info("Fixing book label");
				//const RE::GFxValue htmlTextType = "true";

				//const RE::GFxValue shrink = "shrink";
				//a_menu->uiMovie->SetVariable("_root.Menu_mc.itemCard.BookDescriptionLabel.textAutoSize", shrink, RE::GFxMovie::SetVarType::kPermanent);
				//a_menu->uiMovie->SetVariable("_root.Menu_mc.itemCard.BookDescriptionLabel.htmlText", htmlTextType, RE::GFxMovie::SetVarType::kPermanent);
			};

			static inline std::uint32_t idx = 0x5;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				// TODO: Future update to fix book description labels not being HTML supported
				//stl::write_vfunc<RE::InventoryMenu, InventoryMenuHook>();
			}
		};
	}
	static inline void InstallHooks()
	{
		ItemCardPopulateHook::Install();
		//MenuHooks::InventoryMenuHook::Install();
	}
}

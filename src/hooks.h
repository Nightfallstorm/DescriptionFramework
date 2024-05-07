#pragma once
#include "Configuration.h"
#include "Settings.h"
#include "ItemCardFixer.h"
#include "Utils.h"

namespace hooks
{
	static inline std::string debugDescription = 
		"<font color='#AAFF33'> My Custom <font color=\"#FFFFFF\">White</font> Text Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam risus massa, tincidunt nec pulvinar non, porttitor quis augue. Sed lacinia risus justo, eu pulvinar nulla dignissim nec.";

	namespace ItemCardHooks {
		static const char* getDescription(RE::TESForm* a_item)
		{
			if (Settings::IsDebug()) {
				return debugDescription.c_str();
			}
			auto settings = Settings::GetSingleton();
			auto database = ConfigurationDatabase::GetSingleton();
			if (database->GetDescriptionForObject(a_item).empty()) {
				return "";
			}
			auto desc = database->GetDescriptionForObject(a_item).c_str();
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

		// Embed description into the itemcard, our item card fixer will pull this out and apply it to items as needed
		static void addDescription(RE::ItemCard* itemCard, RE::TESForm* a_item)
		{
			auto descriptionValue = RE::GFxValue(getDescription(a_item));
			itemCard->obj.SetMember(ItemCardFixer::descriptionVar, descriptionValue);
		}

		struct ItemCardPopulateHook
		{
			// Let the item card populate, then embed our description afterwards
			static void thunk(RE::ItemCard* itemCard, RE::TESBoundObject** a_item, char a3)
			{
				func(itemCard, a_item, a3);
				if (itemCard && a_item && *a_item && std::strcmp(getDescription(*a_item), "")) {
					addDescription(itemCard, *a_item);
				}
			};

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

		struct ItemCardPopulateHook2
		{
			// Let the item card populate, then embed our description afterwards
			static void thunk(RE::ItemCard* itemCard, RE::TESForm* a_item)
			{
				func(itemCard, a_item);
				if (itemCard && a_item && std::strcmp(getDescription(a_item), "")) {
					addDescription(itemCard, a_item);
				}				
			};

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(50298, 51219), REL::VariantOffset(0x32, 0x32, 0x32) };
				stl::write_thunk_call<ItemCardPopulateHook2>(target.address());

				logger::info("ItemCardPopulateHook2 hooked at address {:x}", target.address());
				logger::info("ItemCardPopulateHook2 hooked at offset {:x}", target.offset());

				REL::Relocation<std::uintptr_t> target2{ RELOCATION_ID(51152, 52032), REL::VariantOffset(0x8F, 0x8F, 0x8B) };
				stl::write_thunk_call<ItemCardPopulateHook2>(target2.address());

				logger::info("ItemCardPopulateHook2 hooked at address {:x}", target2.address());
				logger::info("ItemCardPopulateHook2 hooked at offset {:x}", target2.offset());
			}
		};
	}

	namespace MenuHooks
	{
		static inline void applyDescription(RE::IMenu* a_menu, const char* itemCard = "_root.Menu_mc.itemCard")
		{
			RE::GFxValue a_itemCard;

			bool hasItemCard = a_menu->uiMovie->GetVariable(&a_itemCard, itemCard);
			if (!hasItemCard) {
				return;
			}

			ItemCardFixer::applyDescription(a_itemCard);
		}
		struct InventoryMenuHook
		{
			static RE::UI_MESSAGE_RESULTS thunk(RE::IMenu* a_menu, RE::UIMessage& a_message)
			{
				auto result = func(a_menu, a_message);
				applyDescription(a_menu);
				return result;
			};

			static inline std::uint32_t idx = 0x4;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				stl::write_vfunc<RE::InventoryMenu, InventoryMenuHook>();
			}
		};
		struct InventoryMenuHookTwo
		{
			static RE::UI_MESSAGE_RESULTS thunk(RE::IMenu* a_menu, float a_interval, std::uint32_t a_currentTime)
			{
				auto result = func(a_menu, a_interval, a_currentTime);
				applyDescription(a_menu);
				return result;
			};

			static inline std::uint32_t idx = 0x5;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				stl::write_vfunc<RE::InventoryMenu, InventoryMenuHookTwo>();
			}
		};
		struct BarterMenuHook
		{
			static RE::UI_MESSAGE_RESULTS thunk(RE::IMenu* a_menu, RE::UIMessage& a_message)
			{
				auto result = func(a_menu, a_message);
				applyDescription(a_menu);
				return result;
			};

			static inline std::uint32_t idx = 0x4;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				stl::write_vfunc<RE::BarterMenu, BarterMenuHook>();
			}
		};
		struct BarterMenuHookTwo
		{
			static RE::UI_MESSAGE_RESULTS thunk(RE::IMenu* a_menu, float a_interval, std::uint32_t a_currentTime)
			{
				auto result = func(a_menu, a_interval, a_currentTime);
				applyDescription(a_menu);
				return result;
			};

			static inline std::uint32_t idx = 0x5;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				stl::write_vfunc<RE::BarterMenu, BarterMenuHookTwo>();
			}
		};
		struct ContainerMenuHook
		{
			static RE::UI_MESSAGE_RESULTS thunk(RE::IMenu* a_menu, RE::UIMessage& a_message)
			{
				auto result = func(a_menu, a_message);
				applyDescription(a_menu);
				return result;
			};

			static inline std::uint32_t idx = 0x4;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				stl::write_vfunc<RE::ContainerMenu, ContainerMenuHook>();
			}
		};
		struct ContainerMenuHookTwo
		{
			static RE::UI_MESSAGE_RESULTS thunk(RE::IMenu* a_menu, float a_interval, std::uint32_t a_currentTime)
			{
				auto result = func(a_menu, a_interval, a_currentTime);
				applyDescription(a_menu);
				return result;
			};

			static inline std::uint32_t idx = 0x5;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				stl::write_vfunc<RE::ContainerMenu, ContainerMenuHookTwo>();
			}
		};
		struct GiftMenuHook
		{
			static RE::UI_MESSAGE_RESULTS thunk(RE::IMenu* a_menu, RE::UIMessage& a_message)
			{
				auto result = func(a_menu, a_message);
				applyDescription(a_menu);
				return result;
			};

			static inline std::uint32_t idx = 0x4;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				stl::write_vfunc<RE::GiftMenu, GiftMenuHook>();
			}
		};
		struct GiftMenuHookTwo
		{
			static RE::UI_MESSAGE_RESULTS thunk(RE::IMenu* a_menu, float a_interval, std::uint32_t a_currentTime)
			{
				auto result = func(a_menu, a_interval, a_currentTime);
				applyDescription(a_menu);
				return result;
			};

			static inline std::uint32_t idx = 0x5;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				stl::write_vfunc<RE::GiftMenu, GiftMenuHookTwo>();
			}
		};
		struct MagicMenuHook
		{
			static RE::UI_MESSAGE_RESULTS thunk(RE::IMenu* a_menu, RE::UIMessage& a_message)
			{
				auto result = func(a_menu, a_message);
				applyDescription(a_menu);
				return result;
			};

			static inline std::uint32_t idx = 0x4;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				stl::write_vfunc<RE::MagicMenu, MagicMenuHook>();
			}
		};
		struct MagicMenuHookTwo
		{
			static RE::UI_MESSAGE_RESULTS thunk(RE::IMenu* a_menu, float a_interval, std::uint32_t a_currentTime)
			{
				auto result = func(a_menu, a_interval, a_currentTime);
				applyDescription(a_menu);
				return result;
			};

			static inline std::uint32_t idx = 0x5;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				stl::write_vfunc<RE::MagicMenu, MagicMenuHookTwo>();
			}
		};
		struct CraftingMenuHook
		{
			static RE::UI_MESSAGE_RESULTS thunk(RE::CraftingMenu* a_menu, RE::UIMessage& a_message)
			{ 
				auto result = func(a_menu, a_message);
				applyDescription(a_menu, "_root.Menu.ItemInfo");
				return result;
			};

			static inline std::uint32_t idx = 0x4;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				stl::write_vfunc<RE::CraftingMenu, CraftingMenuHook>();
			}
		};
		struct CraftingMenuHookTwo
		{
			static RE::UI_MESSAGE_RESULTS thunk(RE::IMenu* a_menu, float a_interval, std::uint32_t a_currentTime)
			{
				auto result = func(a_menu, a_interval, a_currentTime);
				applyDescription(a_menu, "_root.Menu.ItemInfo");
				return result;
			};

			static inline std::uint32_t idx = 0x5;

			static inline REL::Relocation<decltype(thunk)> func;

			// Install our hook at the specified address
			static inline void Install()
			{
				stl::write_vfunc<RE::CraftingMenu, CraftingMenuHookTwo>();
			}
		};

		static void Install() {
			InventoryMenuHook::Install();
			InventoryMenuHookTwo::Install();
			BarterMenuHook::Install();
			BarterMenuHookTwo::Install();
			ContainerMenuHookTwo::Install();
			ContainerMenuHook::Install();
			GiftMenuHook::Install();
			GiftMenuHookTwo::Install();
			MagicMenuHook::Install();
			MagicMenuHookTwo::Install();
			CraftingMenuHook::Install();
			CraftingMenuHookTwo::Install();
		}
	}

	static inline void InstallHooks()
	{
		ItemCardHooks::ItemCardPopulateHook::Install();
		ItemCardHooks::ItemCardPopulateHook2::Install();
		MenuHooks::Install();
	}
}

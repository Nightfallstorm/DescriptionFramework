#pragma once

enum ItemCardType : int
{
	ICT_NONE,
	ICT_ARMOR,
	ICT_WEAPON,
	ICT_MISC,
	ICT_BOOK,
	ICT_FOOD,
	ICT_POTION,
	ICT_SPELL,
	ICT_INGREDIENT,
	ICT_KEY,
	ICT_SHOUT,
	ICT_ACTIVE_EFFECT,
	ICT_SOUL_GEMS,
	ICT_SPELL_DEFAULT,
	ICT_LIST,
	ICT_CRAFT_ENCHANTING,
	ICT_HOUSE_PART,
};

class ItemCardFixer
{
public:
	static inline constexpr auto descriptionVar = "DF_description";
	static inline constexpr auto appliedDescriptionVar = "DF_fixed";
	static inline constexpr auto itemInfoVar = "itemInfo";
	static inline constexpr auto typeVar = "type";
	

	void handleArmor(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleWeapon(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleMisc(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleBook(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleFood(RE::GFxValue& a_itemInfo, const char* a_description);
	void handlePotion(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleSpell(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleIngredient(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleKey(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleShout(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleActiveEffect(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleSoulGems(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleSpellDefault(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleList(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleCraftEnchanting(RE::GFxValue& a_itemInfo, const char* a_description);
	void handleHousePart(RE::GFxValue& a_itemInfo, const char* a_description);
	void fixArmor(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixWeapon(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixMisc(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixBook(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixFood(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixPotion(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixSpell(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixIngredient(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixKey(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixShout(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixActiveEffect(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixSoulGems(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixSpellDefault(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixList(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixCraftEnchanting(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);
	void fixHousePart(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo);


	void applyDescriptionToItemInfo(RE::GFxValue& a_itemInfo, const char* a_description, ItemCardType a_type);
	void fixItemCardForDescription(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo, ItemCardType a_type);

	static ItemCardFixer GetSingleton() {
		static ItemCardFixer _this_fixer;
		return _this_fixer;
	}
	static void applyDescription(RE::GFxValue& a_itemCard)
	{
		// Guard checks
		if (!a_itemCard.IsObject()) {
			logger::debug("Not itemcard object, returning");
			return;
		}

		RE::GFxValue a_itemInfo;
		a_itemCard.GetMember(itemInfoVar, &a_itemInfo);

		if (!a_itemInfo.IsObject()) {
			logger::debug("No iteminfo object, returning");
			return;
		}

		RE::GFxValue a_description;
		a_itemInfo.GetMember(descriptionVar, &a_description);
		if (!a_description.IsString()) {
			logger::debug("No description present, returning");
			return;
		}

		RE::GFxValue a_type;
		a_itemInfo.GetMember(typeVar, &a_type);
		if (!a_type.IsNumber()) {
			logger::debug("Not type number, returning");
			return;
		}

		RE::GFxValue appliedDescription;
		a_itemInfo.GetMember(appliedDescriptionVar, &appliedDescription);

		if (appliedDescription.IsBool() && appliedDescription.GetBool()) {
			// Description already applied to item info. This will prevent us from adding description again, causing duplicate descriptions
			return;
		}
		// end guard checks

		RE::GFxValue itemName;
		a_itemInfo.GetMember("name", &itemName);
		// First, fix up item info to apply the description where needed
		logger::debug("Fixing up item {} with item type {} using description {}", itemName.GetString(), a_type.GetUInt(), a_description.GetString());
		GetSingleton().applyDescriptionToItemInfo(a_itemInfo, a_description.GetString(), (ItemCardType)a_type.GetUInt());

		// Second, re-apply item info to get item card on the right frame for an object
		a_itemCard.SetMember(itemInfoVar, a_itemInfo);

		// Third, fix up item card itself as needed
		GetSingleton().fixItemCardForDescription(a_itemCard, a_itemInfo, (ItemCardType)a_type.GetUInt());

		
		appliedDescription.SetBoolean(true);
		a_itemInfo.SetMember(appliedDescriptionVar, appliedDescription);
	}
};

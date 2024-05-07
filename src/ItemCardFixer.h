#pragma once
#include "Settings.h"

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

	struct CollectDFVariables : RE::GFxValue::ObjectVisitor
	{
	public:
		// Collect DF variables so we can erase them when reverting description
		virtual void Visit(const char* a_name, const RE::GFxValue& a_val) override {
			if (std::string(a_name).find("DF_") != std::string::npos) {
				collectedVariableNames.emplace_back(a_name);
			}
		}

		std::list<const char*> collectedVariableNames;
	};

	struct AddDisplayBorder : RE::GFxValue::ObjectVisitor
	{
	public:
		// Collect DF variables so we can erase them when reverting description
		virtual void Visit(const char* a_name, const RE::GFxValue& a_val) override
		{
			RE::GFxValue border;

			if (!a_val.IsDisplayObject()) {
				return;
			}

			a_val.GetMember("border", &border);
			if (border.IsBool() && border.GetBool()) {
				return;
			}

			logger::debug("Applying debug border to {}", a_name);
			RE::GFxValue displayVal = a_val;
			displayVal.SetMember("border", true);
			displayVal.SetMember("borderColor", 0xFC0303);
		}
	};

	static void applyDescription(RE::GFxValue a_itemCard)
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

		// end guard checks

		if (appliedDescription.IsBool() && appliedDescription.GetBool()) {
			// Description already applied to item info. This will prevent us from adding description again, causing duplicate descriptions
			return;
		}

		RE::GFxValue itemName;
		a_itemInfo.GetMember("name", &itemName);

		logger::debug("Fixing up item {} with item type {} using description {}", itemName.GetString(), a_type.GetUInt(), a_description.GetString());

		// Setup data for fixing
		ItemCardFixer* fixer = new ItemCardFixer(a_itemCard, a_itemInfo, a_description.GetString(), (ItemCardType)a_type.GetUInt());

		// First, fix up item info to apply the description where needed
		fixer->applyDescription();

		// Second, re-apply item info to get item card on the right frame for an object
		fixer->itemCard.SetMember(itemInfoVar, a_itemInfo);

		// Third, fix up item card itself as needed
		fixer->fixItemCard();

		// Finally, apply debug features
		if (Settings::IsDebug()) {
			AddDisplayBorder borderMaker;
			a_itemCard.VisitMembers(&borderMaker);
		}

		// delete fixer;

		appliedDescription.SetBoolean(true);
		a_itemInfo.SetMember(appliedDescriptionVar, appliedDescription);


	}

	static void revertDescription(RE::GFxValue a_itemCard) {
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

		// First, remove all DF variables used except the one holding the current description
		CollectDFVariables collector;
		a_itemCard.VisitMembers(&collector);
		for (auto DFVariable : collector.collectedVariableNames) {
			if (std::strcmp(DFVariable, descriptionVar)) {
				a_itemCard.DeleteMember(DFVariable);
			}
		}

		collector.collectedVariableNames.clear();

		a_itemInfo.VisitMembers(&collector);
		for (auto DFVariable : collector.collectedVariableNames) {
			if (std::strcmp(DFVariable, descriptionVar)) {
				a_itemInfo.DeleteMember(DFVariable);
			}
		}

		collector.collectedVariableNames.clear();

		// Second, apply a blank itemInfo to reset SWF elements
		RE::GFxValue origType;
		a_itemInfo.GetMember("type", &origType);
		a_itemInfo.SetMember("type", ICT_NONE);
		
		a_itemCard.SetMember("itemInfo", a_itemInfo); // This will skip to the empty itemCard frame, clearing out all previous SWF elements we touched
		
		// Third, reapply original itemInfo to get the item card where it should be
		a_itemInfo.SetMember("type", origType);
		a_itemCard.SetMember("itemInfo", a_itemInfo);		
	}

private:

	ItemCardFixer(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo, const char* a_description, ItemCardType a_type) {
		assert(!a_itemCard.IsUndefined());
		assert(!a_itemInfo.IsUndefined());
		assert(strcmp(a_description, "") != 0);
		
		itemCard = a_itemCard;
		itemInfo = a_itemInfo;
		type = a_type;
		description = a_description;
	}
	void handleEffects();
	void handleSoulLVL();
	void fixHTML(const char* a_displayVariable, const char* a_descriptionVariable);
	void fixBackground(const char* a_displayLabel);

	void handleArmor();
	void handleWeapon();
	void handleMisc();
	void handleBook();
	void handleFood();
	void handlePotion();
	void handleSpell();
	void handleIngredient();
	void handleKey();
	void handleShout();
	void handleActiveEffect();
	void handleSoulGems();
	void handleSpellDefault();
	void handleList();
	void handleCraftEnchanting();
	void handleHousePart();

	void fixArmor();
	void fixWeapon();
	void fixMisc();
	void fixBook();
	void fixFood();
	void fixPotion();
	void fixSpell();
	void fixIngredient();
	void fixKey();
	void fixShout();
	void fixActiveEffect();
	void fixSoulGems();
	void fixSpellDefault();
	void fixList();
	void fixCraftEnchanting();
	void fixHousePart();

	void applyDescription();
	void fixItemCard();

	RE::GFxValue itemInfo;
	RE::GFxValue itemCard;
	ItemCardType type;
	const char* description;
};

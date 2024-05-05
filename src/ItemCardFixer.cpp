#pragma once
#include "ItemCardFixer.h"

static void handleEffects(RE::GFxValue& a_itemInfo, const char* a_description)
{
	// Insert description into effects
	auto effectsStr = std::string("");
	RE::GFxValue effects;
	a_itemInfo.GetMember("effects", &effects);
	if (effects.IsString() && std::strcmp(effects.GetString(), "undefined") && std::strcmp(effects.GetString(), "")) {
		effectsStr = std::string(effects.GetString());
		effectsStr.erase(remove(effectsStr.begin(), effectsStr.end(), ' '), effectsStr.end());
		if (!effectsStr.empty()) {
			effectsStr = std::string(effects.GetString()) + "\n";
		}
	}

	auto effectsDesc = effectsStr + a_description;
	a_itemInfo.SetMember("effects", effectsDesc.c_str());
}

static void handleSoulLVL(RE::GFxValue& a_itemInfo, const char* a_description)
{
	// Append description to soulLVL, while faking as a book
	auto soulStr = std::string("");
	RE::GFxValue soulLVL;
	a_itemInfo.GetMember("soulLVL", &soulLVL);
	if (soulLVL.IsString() && std::strcmp(soulLVL.GetString(), "undefined") && std::strcmp(soulLVL.GetString(), "")) {
		soulStr = std::string(soulLVL.GetString());
		soulStr.erase(remove(soulStr.begin(), soulStr.end(), ' '), soulStr.end());
		if (!soulStr.empty()) {
			soulStr = std::string(soulLVL.GetString()) + "\n";
		}
	}

	auto soulsDesc = soulStr + a_description;
	a_itemInfo.SetMember("description", soulsDesc.c_str());
	a_itemInfo.SetMember(ItemCardFixer::typeVar, ItemCardType::ICT_BOOK);
}

void ItemCardFixer::handleArmor(RE::GFxValue& a_itemInfo, const char* a_description)
{
	handleEffects(a_itemInfo, a_description);
}
void ItemCardFixer::handleWeapon(RE::GFxValue& a_itemInfo, const char* a_description)
{
	handleEffects(a_itemInfo, a_description);
}
void ItemCardFixer::handleMisc(RE::GFxValue& a_itemInfo, const char* a_description)
{
	// Fake MISC items as books
	a_itemInfo.SetMember("description", a_description);
	a_itemInfo.SetMember(typeVar, ItemCardType::ICT_BOOK);
}
void ItemCardFixer::handleBook(RE::GFxValue& a_itemInfo, const char* a_description)
{
	// Append DF's description to the books
	auto origDescStr = std::string("");
	RE::GFxValue description;
	a_itemInfo.GetMember("description", &description);
	if (description.IsString() && std::strcmp(description.GetString(), "undefined") && std::strcmp(description.GetString(), "")) {
		origDescStr = std::string(description.GetString());
		origDescStr.erase(remove(origDescStr.begin(), origDescStr.end(), ' '), origDescStr.end());
		if (!origDescStr.empty()) {
			origDescStr = std::string(description.GetString()) + "\n";
		}
	}

	auto newDesc = origDescStr + a_description;
	a_itemInfo.SetMember("description", newDesc.c_str());
}
void ItemCardFixer::handleFood(RE::GFxValue& a_itemInfo, const char* a_description)
{
	handleEffects(a_itemInfo, a_description);
}
void ItemCardFixer::handlePotion(RE::GFxValue& a_itemInfo, const char* a_description)
{
	handleEffects(a_itemInfo, a_description);
}
void ItemCardFixer::handleSpell(RE::GFxValue& a_itemInfo, const char* a_description)
{
	handleEffects(a_itemInfo, a_description);
}
void ItemCardFixer::handleIngredient(RE::GFxValue& a_itemInfo, const char* a_description) {
	/* unsupported*/
}
void ItemCardFixer::handleKey(RE::GFxValue& a_itemInfo, const char* a_description)
{
	// Fake KEY items as books
	a_itemInfo.SetMember("description", a_description);
	a_itemInfo.SetMember(typeVar, ItemCardType::ICT_BOOK);
}
void ItemCardFixer::handleShout(RE::GFxValue& a_itemInfo, const char* a_description)
{
	handleEffects(a_itemInfo, a_description);
}
void ItemCardFixer::handleActiveEffect(RE::GFxValue& a_itemInfo, const char* a_description)
{
	handleEffects(a_itemInfo, a_description);
}
void ItemCardFixer::handleSoulGems(RE::GFxValue& a_itemInfo, const char* a_description)
{
	handleSoulLVL(a_itemInfo, a_description);
}
void ItemCardFixer::handleSpellDefault(RE::GFxValue& a_itemInfo, const char* a_description)
{
	handleEffects(a_itemInfo, a_description);
}
void ItemCardFixer::handleList(RE::GFxValue& a_itemInfo, const char* a_description)
{
	/* unsupported*/
}
void ItemCardFixer::handleCraftEnchanting(RE::GFxValue& a_itemInfo, const char* a_description)
{
	handleEffects(a_itemInfo, a_description);
	handleSoulLVL(a_itemInfo, a_description);
}
void ItemCardFixer::handleHousePart(RE::GFxValue& a_itemInfo, const char* a_description)
{
	handleEffects(a_itemInfo, a_description);
}

void ItemCardFixer::fixArmor(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo) {
	/* no fixing necessary */
}
void ItemCardFixer::fixWeapon(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo)
{
	// Remove weapon charge if there's no actual effects
	RE::GFxValue weaponChargeLabel;
	RE::GFxValue DFdescription;
	RE::GFxValue effects;
	a_itemCard.GetMember("WeaponChargeMeter", &weaponChargeLabel);
	a_itemInfo.GetMember(descriptionVar, &DFdescription);
	a_itemInfo.GetMember("effects", &effects);
	logger::debug("types {} {} {}", weaponChargeLabel.GetType(), DFdescription.GetType(), effects.GetType());
	if (weaponChargeLabel.IsDisplayObject() && DFdescription.IsString() && effects.IsString()) {
		if (!strcmp(DFdescription.GetString(), effects.GetString())) {
			// effects is same as description, turn off weapon charge label
			logger::debug("Turning off charge label");
			RE::GFxValue::DisplayInfo info;
			weaponChargeLabel.GetDisplayInfo(&info);
			info.SetVisible(false);
			weaponChargeLabel.SetDisplayInfo(info);
			logger::debug("Weapon Charge Label disabled");
		}
	}
}
void ItemCardFixer::fixMisc(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo)
{
	// MISC items are now books, fix the same way
	fixBook(a_itemCard, a_itemInfo);
}
void ItemCardFixer::fixBook(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo)
{
	// As a book, make it HTML
	RE::GFxValue bookLabel;
	RE::GFxValue description;
	a_itemCard.GetMember("BookDescriptionLabel", &bookLabel);
	a_itemInfo.GetMember("description", &description);
	if (bookLabel.IsDisplayObject() && description.IsString()) {
		bookLabel.SetTextHTML(description.GetString());
	}
}
void ItemCardFixer::fixFood(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo) {
	/* no fixing necessary */
}
void ItemCardFixer::fixPotion(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo) {
	/* no fixing necessary */
}
void ItemCardFixer::fixSpell(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo) {
	/* no fixing necessary */
}
void ItemCardFixer::fixIngredient(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo) {
	/* unsupported */
}
void ItemCardFixer::fixKey(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo) {
	fixBook(a_itemCard, a_itemInfo);
}
void ItemCardFixer::fixShout(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo) {
	// As a shout, use text auto shrink
	RE::GFxValue shoutEffectsLabel;
	a_itemCard.GetMember("ShoutEffectsLabel", &shoutEffectsLabel);
	if (shoutEffectsLabel.IsDisplayObject()) {
		shoutEffectsLabel.SetMember("textAutoSize", "shrink");
	}
}
void ItemCardFixer::fixActiveEffect(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo) {
	/* no fixing necessary */
}
void ItemCardFixer::fixSoulGems(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo)
{
	// Soul gems are faked as books now, fix up as a book
	fixBook(a_itemCard, a_itemInfo);
}
void ItemCardFixer::fixSpellDefault(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo) {}
void ItemCardFixer::fixList(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo) {}
void ItemCardFixer::fixCraftEnchanting(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo) {
	fixSoulGems(a_itemCard, a_itemInfo);
}
void ItemCardFixer::fixHousePart(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo) {}

void ItemCardFixer::applyDescriptionToItemInfo(RE::GFxValue& a_itemInfo, const char* a_description, ItemCardType a_type)
{
	switch (a_type) {
	case ICT_ARMOR:
		{
			logger::debug("Handling armor");
			handleArmor(a_itemInfo, a_description);
			break;
		}
	case ICT_WEAPON:
		{
			logger::debug("Handling weapon");
			handleWeapon(a_itemInfo, a_description);
			break;
		}
	case ICT_MISC:
		{
			logger::debug("Handling misc");
			handleMisc(a_itemInfo, a_description);
			break;
		}
	case ICT_BOOK:
		{
			logger::debug("Handling book");
			handleBook(a_itemInfo, a_description);
			break;
		}
	case ICT_FOOD:
		{
			logger::debug("Handling food");
			handleFood(a_itemInfo, a_description);
			break;
		}
	case ICT_POTION:
		{
			logger::debug("Handling potion");
			handlePotion(a_itemInfo, a_description);
			break;
		}
	case ICT_SPELL:
		{
			logger::debug("Handling spell");
			handleSpell(a_itemInfo, a_description);
			break;
		}
	case ICT_INGREDIENT:
		{
			logger::debug("Handling ingredient");
			handleIngredient(a_itemInfo, a_description);
			break;
		}
	case ICT_KEY:
		{
			logger::debug("Handling key");
			handleKey(a_itemInfo, a_description);
			break;
		}
	case ICT_SHOUT:
		{
			logger::debug("Handling shout");
			handleShout(a_itemInfo, a_description);
			break;
		}
	case ICT_ACTIVE_EFFECT:
		{
			logger::debug("Handling active effect");
			handleActiveEffect(a_itemInfo, a_description);
			break;
		}
	case ICT_SOUL_GEMS:
		{
			logger::debug("Handling soul gem");
			handleSoulGems(a_itemInfo, a_description);
			break;
		}
	case ICT_SPELL_DEFAULT:
		{
			logger::debug("Handling spell default");
			handleSpellDefault(a_itemInfo, a_description);
			break;
		}
	case ICT_LIST:
		{
			logger::debug("Handling list");
			handleList(a_itemInfo, a_description);
			break;
		}
	case ICT_CRAFT_ENCHANTING:
		{
			logger::debug("Handling craft enchanting");
			handleCraftEnchanting(a_itemInfo, a_description);
			break;
		}
	case ICT_HOUSE_PART:
		{
			logger::debug("Handling house part");
			handleHousePart(a_itemInfo, a_description);
			break;
		}
	}
}

void ItemCardFixer::fixItemCardForDescription(RE::GFxValue& a_itemCard, RE::GFxValue& a_itemInfo, ItemCardType a_type)
{
	switch (a_type) {
	case ICT_ARMOR:
		{
			logger::debug("Fixing armor");
			fixArmor(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_WEAPON:
		{
			logger::debug("Fixing weapon");
			fixWeapon(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_MISC:
		{
			logger::debug("Fixing misc");
			fixMisc(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_BOOK:
		{
			logger::debug("Fixing book");
			fixBook(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_FOOD:
		{
			logger::debug("Fixing food");
			fixFood(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_POTION:
		{
			logger::debug("Fixing potion");
			fixPotion(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_SPELL:
		{
			logger::debug("Fixing spell");
			fixSpell(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_INGREDIENT:
		{
			logger::debug("Fixing ingredient");
			fixIngredient(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_KEY:
		{
			logger::debug("Fixing key");
			fixKey(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_SHOUT:
		{
			logger::debug("Fixing shout");
			fixShout(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_ACTIVE_EFFECT:
		{
			logger::debug("Fixing active effect");
			fixActiveEffect(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_SOUL_GEMS:
		{
			logger::debug("Fixing soul gem");
			fixSoulGems(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_SPELL_DEFAULT:
		{
			logger::debug("Fixing spell default");
			fixSpellDefault(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_LIST:
		{
			logger::debug("Fixing list");
			fixList(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_CRAFT_ENCHANTING:
		{
			logger::debug("Fixing craft enchanting");
			fixCraftEnchanting(a_itemCard, a_itemInfo);
			break;
		}
	case ICT_HOUSE_PART:
		{
			logger::debug("Fixing house part");
			fixHousePart(a_itemCard, a_itemInfo);
			break;
		}
	}
}

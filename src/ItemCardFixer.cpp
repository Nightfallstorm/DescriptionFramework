#pragma once
#include "ItemCardFixer.h"
#include "Settings.h"

void ItemCardFixer::handleEffects()
{
	// Insert description into effects
	auto effectsStr = std::string("");
	RE::GFxValue effects;
	itemInfo.GetMember("effects", &effects);
	if (effects.IsString() && std::strcmp(effects.GetString(), "undefined") && std::strcmp(effects.GetString(), "")) {
		effectsStr = std::string(effects.GetString());
		effectsStr.erase(remove(effectsStr.begin(), effectsStr.end(), ' '), effectsStr.end());
		if (!effectsStr.empty()) {
			effectsStr = std::string(effects.GetString()) + "\n";
		}
	}

	auto effectsDesc = effectsStr + this->description;
	itemInfo.SetMember("effects", effectsDesc.c_str());
}

void ItemCardFixer::handleSoulLVL()
{
	// Append description to soulLVL, while faking as a book
	auto soulStr = std::string("");
	RE::GFxValue soulLVL;
	itemInfo.GetMember("soulLVL", &soulLVL);
	if (soulLVL.IsString() && std::strcmp(soulLVL.GetString(), "undefined") && std::strcmp(soulLVL.GetString(), "")) {
		soulStr = std::string(soulLVL.GetString());
		soulStr.erase(remove(soulStr.begin(), soulStr.end(), ' '), soulStr.end());
		if (!soulStr.empty()) {
			soulStr = std::string(soulLVL.GetString()) + "\n";
		}
	}

	auto soulsDesc = soulStr + this->description;
	itemInfo.SetMember("description", soulsDesc.c_str());
	itemInfo.SetMember(ItemCardFixer::typeVar, ItemCardType::ICT_BOOK);
}

void ItemCardFixer::fixHTML(const char* a_displayVariable, const char* a_descriptionVariable)
{
	RE::GFxValue displayLabel;
	RE::GFxValue descriptionLabel;
	RE::GFxValue heightLabel;
	itemCard.GetMember(a_displayVariable, &displayLabel);
	itemInfo.GetMember(a_descriptionVariable, &descriptionLabel);
	if (displayLabel.IsDisplayObject() && descriptionLabel.IsString()) {
		displayLabel.SetMember("textAutoSize", "shrink");
		displayLabel.SetTextHTML(descriptionLabel.GetString());

		// Make text clearer for easier reading
		displayLabel.SetMember("antiAliasType", "advanced");
		displayLabel.SetMember("gridFitType", "pixel");
	}	
}

void ItemCardFixer::fixBackground(const char* a_displayLabel)
{
// Increase the background size, and the display label to match
	float heightScale = Settings::GetSingleton()->tweaks.heightScale;
	float widthScale = Settings::GetSingleton()->tweaks.widthScale;
	float heightOffset = Settings::GetSingleton()->tweaks.heightOffset;
	float widthOffset = Settings::GetSingleton()->tweaks.widthOffset;

	RE::GFxValue background;
	RE::GFxValue backgroundFixed;
	RE::GFxValue displayLabel;
	RE::GFxValue backgroundX;
	RE::GFxValue backgroundY;
	RE::GFxValue displayX;
	RE::GFxValue displayY;
	RE::GFxValue backgroundHeight;
	RE::GFxValue backgroundWidth;

	itemCard.GetMember("background", &background);
	itemCard.GetMember(a_displayLabel, &displayLabel);

	background.GetMember("DF_backgroundFixed", &backgroundFixed);
	background.GetMember("_height", &backgroundHeight);
	background.GetMember("_width", &backgroundWidth);
	background.GetMember("_y", &backgroundY);
	background.GetMember("_x", &backgroundX);
	displayLabel.GetMember("_x", &displayX);
	displayLabel.GetMember("_y", &displayY);

	// If background is already scaled, don't scale it again
	if (!backgroundFixed.IsBool() || !backgroundFixed.GetBool()) {
		backgroundHeight.SetNumber(backgroundHeight.GetNumber() * heightScale);
		background.SetMember("_height", backgroundHeight);
		backgroundY.SetNumber(backgroundY.GetNumber() + heightOffset);
		background.SetMember("_y", backgroundY);

		backgroundWidth.SetNumber(backgroundWidth.GetNumber() * widthScale);
		background.SetMember("_width", backgroundWidth);
		backgroundX.SetNumber(backgroundX.GetNumber() + widthOffset);
		background.SetMember("_x", backgroundX);


		background.SetMember("DF_backgroundFixed", true);
	}
	
	auto bottomY = backgroundY.GetNumber() + backgroundHeight.GetNumber();
	auto newDisplayHeight = bottomY - displayY.GetNumber();
	newDisplayHeight -= 15; // Padding
	displayLabel.SetMember("_height", newDisplayHeight);

	displayX.SetNumber(backgroundX.GetNumber() + 15);
	displayLabel.SetMember("_x", displayX);
	auto rightX = backgroundX.GetNumber() + backgroundWidth.GetNumber();
	auto newDisplayWidth = rightX - displayX.GetNumber();
	newDisplayWidth -= 15;  // Padding
	displayLabel.SetMember("_width", newDisplayWidth);
	
	if (Settings::IsDebug()) {
		displayLabel.SetMember("border", true);
		displayLabel.SetMember("borderColor", descriptionDebugBorder);
	}
}

void ItemCardFixer::handleArmor()
{
	handleEffects();
}
void ItemCardFixer::handleWeapon()
{
	handleEffects();
}
void ItemCardFixer::handleMisc()
{
	// Fake MISC items as books
	itemInfo.SetMember("description", this->description);
	itemInfo.SetMember(typeVar, ItemCardType::ICT_BOOK);
}
void ItemCardFixer::handleBook()
{
	// Append DF's description to the books
	auto origDescStr = std::string("");
	RE::GFxValue desc;
	itemInfo.GetMember("description", &desc);
	if (desc.IsString() && std::strcmp(desc.GetString(), "undefined") && std::strcmp(desc.GetString(), "")) {
		origDescStr = std::string(desc.GetString());
		origDescStr.erase(remove(origDescStr.begin(), origDescStr.end(), ' '), origDescStr.end());
		if (!origDescStr.empty()) {
			origDescStr = std::string(desc.GetString()) + "\n";
		}
	}

	auto newDesc = origDescStr + this->description;
	itemInfo.SetMember("description", newDesc.c_str());
}
void ItemCardFixer::handleFood()
{
	handleEffects();
}
void ItemCardFixer::handlePotion()
{
	handleEffects();
}
void ItemCardFixer::handleSpell()
{
	handleEffects();
}
void ItemCardFixer::handleIngredient()
{
	/* unsupported*/
}
void ItemCardFixer::handleKey()
{
	// Fake KEY items as books
	itemInfo.SetMember("description", this->description);
	itemInfo.SetMember(typeVar, ItemCardType::ICT_BOOK);
}
void ItemCardFixer::handleShout()
{
	handleEffects();
}
void ItemCardFixer::handleActiveEffect()
{
	handleEffects();
}
void ItemCardFixer::handleSoulGems()
{
	handleSoulLVL();
}
void ItemCardFixer::handleSpellDefault()
{
	handleEffects();
}
void ItemCardFixer::handleList()
{
	/* unsupported*/
}
void ItemCardFixer::handleCraftEnchanting()
{
	handleEffects();
	handleSoulLVL();
}
void ItemCardFixer::handleHousePart()
{
	handleEffects();
}

void ItemCardFixer::fixArmor()
{
	fixHTML("ApparelEnchantedLabel", "effects");
	fixBackground("ApparelEnchantedLabel");
}
void ItemCardFixer::fixWeapon()
{
	// Remove weapon charge if there's no actual effects
	RE::GFxValue weaponChargeLabel;
	RE::GFxValue DFdescription;
	RE::GFxValue effects;
	itemCard.GetMember("WeaponChargeMeter", &weaponChargeLabel);
	itemInfo.GetMember(descriptionVar, &DFdescription);
	itemInfo.GetMember("effects", &effects);
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
	fixHTML("WeaponEnchantedLabel", "effects");
	fixBackground("WeaponEnchantedLabel");
}
void ItemCardFixer::fixMisc()
{
	// MISC items are now books, fix the same way
	fixBook();
}
void ItemCardFixer::fixBook()
{
	// As a book, make it HTML
	fixHTML("BookDescriptionLabel", "description");
	fixBackground("BookDescriptionLabel");
}
void ItemCardFixer::fixFood()
{
	fixHTML("PotionsLabel", "effects");
	fixBackground("PotionsLabel");
}
void ItemCardFixer::fixPotion()
{
	fixHTML("PotionsLabel", "effects");
	fixBackground("PotionsLabel");
}
void ItemCardFixer::fixSpell()
{
	fixHTML("MagicEffectsLabel", "effects");
	//fixBackground("MagicEffectsLabel");
}
void ItemCardFixer::fixIngredient()
{
	/* unsupported */
}
void ItemCardFixer::fixKey()
{
	fixBook();
}
void ItemCardFixer::fixShout()
{
	// As a shout, use text auto shrink
	fixHTML("ShoutEffectsLabel", "effects");
	// TODO: Can't fix shouts since it will clobber over other elements
}
void ItemCardFixer::fixActiveEffect()
{
	fixSpell();
	fixBackground("MagicEffectsLabel");
}
void ItemCardFixer::fixSoulGems()
{
	// Soul gems are faked as books now, fix up as a book
	fixBook();
}
void ItemCardFixer::fixSpellDefault()
{
	fixSpell();
	fixBackground("MagicEffectsLabel");
}
void ItemCardFixer::fixList()
{
	/* unsupported */
}
void ItemCardFixer::fixCraftEnchanting()
{
	fixHTML("SoulLevel", "soulLVL");

	fixHTML("EnchantmentLabel", "effects");

	fixBackground("SoulLevel");

	fixBackground("EnchantmentLabel");
}
void ItemCardFixer::fixHousePart()
{
	fixSpell();
}

void ItemCardFixer::applyDescription()
{
	logger::info("Applying description {}", description);
	switch (type) {
	case ICT_ARMOR:
		{
			logger::debug("Handling armor");
			handleArmor();
			break;
		}
	case ICT_WEAPON:
		{
			logger::debug("Handling weapon");
			handleWeapon();
			break;
		}
	case ICT_MISC:
		{
			logger::debug("Handling misc");
			handleMisc();
			break;
		}
	case ICT_BOOK:
		{
			logger::debug("Handling book");
			handleBook();
			break;
		}
	case ICT_FOOD:
		{
			logger::debug("Handling food");
			handleFood();
			break;
		}
	case ICT_POTION:
		{
			logger::debug("Handling potion");
			handlePotion();
			break;
		}
	case ICT_SPELL:
		{
			logger::debug("Handling spell");
			handleSpell();
			break;
		}
	case ICT_INGREDIENT:
		{
			logger::debug("Handling ingredient");
			handleIngredient();
			break;
		}
	case ICT_KEY:
		{
			logger::debug("Handling key");
			handleKey();
			break;
		}
	case ICT_SHOUT:
		{
			logger::debug("Handling shout");
			handleShout();
			break;
		}
	case ICT_ACTIVE_EFFECT:
		{
			logger::debug("Handling active effect");
			handleActiveEffect();
			break;
		}
	case ICT_SOUL_GEMS:
		{
			logger::debug("Handling soul gem");
			handleSoulGems();
			break;
		}
	case ICT_SPELL_DEFAULT:
		{
			logger::debug("Handling spell default");
			handleSpellDefault();
			break;
		}
	case ICT_LIST:
		{
			logger::debug("Handling list");
			handleList();
			break;
		}
	case ICT_CRAFT_ENCHANTING:
		{
			logger::debug("Handling craft enchanting");
			handleCraftEnchanting();
			break;
		}
	case ICT_HOUSE_PART:
		{
			logger::debug("Handling house part");
			handleHousePart();
			break;
		}
	}
}

void ItemCardFixer::fixItemCard()
{
	switch (type) {
	case ICT_ARMOR:
		{
			logger::debug("Fixing armor");
			fixArmor();
			break;
		}
	case ICT_WEAPON:
		{
			logger::debug("Fixing weapon");
			fixWeapon();
			break;
		}
	case ICT_MISC:
		{
			logger::debug("Fixing misc");
			fixMisc();
			break;
		}
	case ICT_BOOK:
		{
			logger::debug("Fixing book");
			fixBook();
			break;
		}
	case ICT_FOOD:
		{
			logger::debug("Fixing food");
			fixFood();
			break;
		}
	case ICT_POTION:
		{
			logger::debug("Fixing potion");
			fixPotion();
			break;
		}
	case ICT_SPELL:
		{
			logger::debug("Fixing spell");
			fixSpell();
			break;
		}
	case ICT_INGREDIENT:
		{
			logger::debug("Fixing ingredient");
			fixIngredient();
			break;
		}
	case ICT_KEY:
		{
			logger::debug("Fixing key");
			fixKey();
			break;
		}
	case ICT_SHOUT:
		{
			logger::debug("Fixing shout");
			fixShout();
			break;
		}
	case ICT_ACTIVE_EFFECT:
		{
			logger::debug("Fixing active effect");
			fixActiveEffect();
			break;
		}
	case ICT_SOUL_GEMS:
		{
			logger::debug("Fixing soul gem");
			fixSoulGems();
			break;
		}
	case ICT_SPELL_DEFAULT:
		{
			logger::debug("Fixing spell default");
			fixSpellDefault();
			break;
		}
	case ICT_LIST:
		{
			logger::debug("Fixing list");
			fixList();
			break;
		}
	case ICT_CRAFT_ENCHANTING:
		{
			logger::debug("Fixing craft enchanting");
			fixCraftEnchanting();
			break;
		}
	case ICT_HOUSE_PART:
		{
			logger::debug("Fixing house part");
			fixHousePart();
			break;
		}
	}
}

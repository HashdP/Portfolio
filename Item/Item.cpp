#include "Item/Item.hpp"
#include "Factory/ItemFactory.hpp"

USING_NS_CC;

Item::Item() :
	id(ItemID::None),
	num(0),
	maxNum(0),
	sprite(nullptr)
{
}


Item::Item(ItemID id, int num, int maxNum) :
	id(id),
	num(num),
	maxNum(maxNum),
	sprite(nullptr)
{
	if (this->maxNum < this->num)
		this->num = this->maxNum;
}

void Item::SetNum(int num)
{
	this->num = num;
	if (this->maxNum < this->num)
		this->num = this->maxNum;

	if (sprite != nullptr)
	{
		//‰æ‘œ‚Ì”š‚ğ•ÏX‚·‚é
		Label* label = sprite->getChildByTag<Label*>(0);
		label->setString(std::to_string(num));
	}
}

Sprite* Item::GetItemImage(bool numLabel) 
{
	if (sprite == nullptr)
	{
		//‰æ‘œ‚ª‘¶İ‚µ‚È‚¢ê‡‚Íì¬‚·‚é
		sprite = myutil::make_cocos_shared_ref<Sprite>(Sprite::createWithTexture(ItemFactory::getInstance()->GetItemTexture(id)));

		if (numLabel)
		{
			Label* label = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 20), std::to_string(num));
			label->setPositionX(5.0f);
			label->setTag(0);
			sprite->addChild(label, INT_MAX);
		}
	}

	return sprite.get();
}
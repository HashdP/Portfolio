#pragma execution_character_set("utf-8")

#include "GameObject/Character/Character.hpp"
#include "Scene_GamePlay/Layer/ObjectLayer.hpp"
#include "Scene_GamePlay/Floor.hpp"
#include "Field/Area/Area.hpp"
#include "myutil.hpp"
#include "StateAnimationSprite.hpp"
#include "Manager/SoundManager.hpp"
#include "Scene_GamePlay/CharaData.hpp"

USING_NS_CC;

bool Character::init(ObjectLayer* objectLayer, CharacterID charaID, float shadowScale)
{
	if (!GameObject::init(objectLayer))
		return false;

	this->charaID = charaID;

	sprite = StateAnimationSprite::create();
	this->addChild(sprite);

	shadow = Sprite::create("Images/Character/chara_shadow.png");
	shadow->setScale(shadowScale);
	Size shadowSize = shadow->getContentSize();
	hitBox = Rect(-shadowScale * shadowSize / 2, static_cast<Size>(shadowScale * shadowSize));
	this->addChild(shadow, -1);

	color = Color3B::WHITE;

	return true;
}

bool Character::init(ObjectLayer* objectLayer, CharacterID charaID, const CharaData& charaData)
{
	if (!Character::init(objectLayer, charaID, charaData["ShadowScale"]))
		return false;

	InitHP(charaData["HP"]);

	return true;
}

void Character::update(float delta)
{
	if (0.0f < freezeTime)
	{
		//フリーズしている時
		freezeTime -= delta;
		if (freezeTime < 0.0f)
			freezeTime = 0.0f;

		SetState("Stay");
	}
	else
	{
		CharacterUpdate(delta);
	}
}

void Character::Freeze(float freezeTime)
{
	if (this->freezeTime <= 0) 
		this->freezeTime = freezeTime;
}

void Character::Damage(int damage, float freeze, const Vec2& knockBack)
{
	Freeze(freeze * freezeMul);

	sprite->runAction(
		Sequence::create(
			TintTo::create(0.1f, Color3B::RED),
			TintTo::create(0.1f, color),
			TintTo::create(0.1f, Color3B::RED),
			TintTo::create(0.1f, color),
			NULL
		)
	);

	SetHP(HP - damage);

	if(0 < HP)
		MoveCharacter(knockBack * knockBackMul);
}

void Character::MoveCharacter(Vec2& difPosition)
{
	if (difPosition.x != 0.0f && difPosition.y != 0.0f)
	{
		//もし斜め移動の場合、本来移動できない斜め方向につながっているフロアを移動できるようにする（特別な場合）
		if (GetObjectLayer()->GetArea().IsMoveablePlace(this->getPosition() + difPosition))
		{
			this->setPosition(this->getPosition() + difPosition);
			return;
		}
	}

	Vec2 moveAmount = Vec2(std::abs(difPosition.x), std::abs(difPosition.y));

	while (0.0f < moveAmount.x || 0.0f < moveAmount.y)
	{
		Vec2 toPosition = Vec2::ZERO;

		if (0.0f < moveAmount.x)
		{
			float difX = 32 < moveAmount.x ? 32 : moveAmount.x;
			if (difPosition.x < 0.0f)
				difX *= -1;

			moveAmount.x -= 32;

			if (GetObjectLayer()->GetArea().IsMoveablePlace(this->getPosition() + Vec2(difX, 0)))
			{
				toPosition.x += difX;
			}
			else
			{
				//ｘ方向が移動不可の場合
				moveAmount.x = 0.0f;

				float corPosX = this->getPositionX() + 320.0f;
				int numX = corPosX / 32;

				if (difPosition.x < 0.0f)
					toPosition.x += (numX * 32.0f - corPosX) + 0.001f;
				else
					toPosition.x += ((numX + 1) * 32.0f - corPosX) - 0.001f;
			}
		}

		if (0.0f < moveAmount.y)
		{
			float difY = 32 < moveAmount.y ? 32 : moveAmount.y;
			if (difPosition.y < 0.0f)
				difY *= -1;

			moveAmount.y -= 32;

			if (GetObjectLayer()->GetArea().IsMoveablePlace(this->getPosition() + Vec2(0, difY)))
			{
				toPosition.y += difY;
			}
			else
			{
				//ｙ方向が移動不可の場合
				moveAmount.y = 0.0f;

				float corPosY = this->getPositionY() + 80.0f;
				int numY = corPosY / 32;

				if (difPosition.y < 0.0f)
					toPosition.y += (numY * 32.0f - corPosY) + 0.001f;
				else
					toPosition.y += ((numY + 1) * 32.0f - corPosY) - 0.001f;
			}
		}

		if (GetObjectLayer()->GetArea().IsMoveablePlace(this->getPosition() + toPosition))
		{
			this->setPosition(this->getPosition() + toPosition);
		}
		else
		{
			break;
		}
	}
}

void Character::SetKnockBack(float knockBack)
{
	this->knockBackMul = knockBack;
	if (1.0f < this->knockBackMul)
		this->knockBackMul = 1.0f;
	else if (this->knockBackMul < 0.0f)
		this->knockBackMul = 0.0f;
}

void Character::SetFreezeMul(float freezeMul)
{
	this->freezeMul = freezeMul;
	if (1.0f < this->freezeMul)
		this->freezeMul = 1.0f;
	else if (this->freezeMul < 0.0f)
		this->freezeMul = 0.0f;
}

void Character::SetState(std::string state, bool changeAnimation)
{
	this->state = state;

	if (changeAnimation)
	{
		std::string dirAnimStr = state + "_" + myutil::GetDirectionStr(GetDirection());

		if (sprite->IsExistAnimation(dirAnimStr))
		{
			//方向別アニメーションがある場合
			sprite->SetStateAnimation(dirAnimStr);
		}
		else
		{
			//ない場合
			sprite->SetStateAnimation(state);
		}
	}
}

Node* Character::Speak(std::string text, float time, int fontSize)
{
	Sprite* hukidashi = Sprite::create("Images/hukidashi.png");
	hukidashi->setScale(0);
	hukidashi->setPositionY(50);
	this->addChild(hukidashi, INT_MAX);

	Label* speakText = Label::createWithTTF(TTFConfig("Fonts/mplus-1p-medium.ttf", fontSize), text);
	speakText->setColor(Color3B::BLACK);
	speakText->setPosition(Vec2(hukidashi->getContentSize() / 2) + Vec2(0, 25));
	speakText->setWidth(500);
	speakText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	speakText->setName("text");
	speakText->setLineHeight(45.0f);
	hukidashi->addChild(speakText, INT_MAX);

	Sequence* hukidashiScaleAnim = Sequence::create(
		ScaleTo::create(0.1f, 0.15f),
		CallFuncN::create([](Node*) { SoundManager::getInstance()->Play2DSound(SoundID::Hukidashi_Appear); }),
		NULL
	);

	if (0.0f < time)
	{
		hukidashi->setName("hukidashi");
		hukidashi->runAction(
			Sequence::create(
				hukidashiScaleAnim,
				DelayTime::create(time),
				CallFuncN::create([this](Node* node) {removeChildByName("hukidashi"); }),
				NULL
			)
		);
	}
	else
	{
		hukidashi->runAction(hukidashiScaleAnim);
	}

	return hukidashi;
}

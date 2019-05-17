#include "Attack_Bullet.hpp"
#include "ObjectLayer.hpp"
#include "Character.hpp"
#include "Light.hpp"
#include "EffectFactory.hpp"
#include "myutil.hpp"
#include "SoundManager.hpp"

USING_NS_CC;

bool Attack_Bullet::init(CharacterID parentID, ObjectLayer* objectLayer, const cocos2d::Vec2& originPosition, Direction direction, float dmg, float frz, float kb, SoundID hitSEID)
{
	if (!Attack::init(parentID, objectLayer, originPosition, direction, dmg, frz, kb, hitSEID))
		return false;

	Texture2D* gunTexture = Director::getInstance()->getTextureCache()->addImage("Images/gunShot.png");
	float bulletScale = 0.25f;

	Node* bullet = Node::create();

	Light* gunNode1 = Light::create("Images/gunShot.png", 1);
	bullet->addChild(gunNode1);

	Sprite* gunNode2 = Sprite::createWithTexture(gunTexture);
	gunNode2->setScaleY(0.05f);
	gunNode2->setScaleX(0.8f);
	bullet->addChild(gunNode2);

	bullet->setScaleY(0.3f);
	bullet->setScaleX(bulletScale);
	bullet->setCascadeColorEnabled(true);
	this->addChild(bullet);

	switch (GetDirection())
	{
	case Direction::Left:
	case Direction::Right:
		bullet->setPositionY(8);
		break;

	case Direction::Front:
	case Direction::Back:
		bulletScale *= 0.5f;
		bullet->setScaleX(bulletScale);

		setRotation(90.0f);
		break;
	}

	this->setCascadeColorEnabled(true);
	this->setColor(Color3B(255, 200, 100));

	bulletLength = gunTexture->getContentSize().width * bulletScale;
	basehit = true;

	return true;
}

void Attack_Bullet::update(float delta)
{
	//ƒqƒbƒg”»’è
	float speed = bulletLength;

	if (basehit)
	{
		//‹ß‚·‚¬‚éê‡‚Ì‚·‚è”²‚¯–h~

		Vec2 basePos = getPosition();
		Rect baseRect;
		switch (GetDirection())
		{
		case Direction::Left:  baseRect.setRect(basePos.x - bulletLength / 2, basePos.y - 1, bulletLength / 2, 2); break;
		case Direction::Right: baseRect.setRect(basePos.x, basePos.y - 1, bulletLength / 2, 2); break;
		case Direction::Front: baseRect.setRect(basePos.x - 1, basePos.y - bulletLength / 2, 2, bulletLength);   break;
		case Direction::Back:  baseRect.setRect(basePos.x - 1, basePos.y, 2, bulletLength);   break;
		}

		if (HitAttack(baseRect))
		{
			this->setVisible(false);
			return;
		}

		basehit = false;
	}

	setPosition(getPosition() + myutil::GetDirectionVec(GetDirection()) * speed);

	Vec2 pos = getPosition();

	switch (GetDirection())
	{
	case Direction::Left:
	case Direction::Right:
		bulletRect.setRect(pos.x - bulletLength / 2, pos.y - 1, bulletLength, 2);
		break;

	case Direction::Front:
	case Direction::Back:
		bulletRect.setRect(pos.x - 1, pos.y - bulletLength / 2, 2, bulletLength);
		break;
	}

	if (HitAttack(bulletRect))
	{
		this->setVisible(false);
		return;
	}

	//•Ç•ûŒü‚ÉŒ‚‚Âê‡‚Íæ‚ª•Ç‚É“–‚½‚Á‚½‚çíœ‚·‚é
	if (GetDirection() == Direction::Back)
		pos.y += bulletLength;

	if (pos.x < -320.0f || 320.0f < pos.x || pos.y < -80.0f || 80.0f < pos.y)
	{
		if (GetDirection() == Direction::Back)
		{
			//•Ç‚É“–‚½‚Á‚½ê‡
			HitSparkle(Vec2(pos.x, 80.0f + bulletLength / 2));
		}

		GetObjectLayer()->RemoveObject(this);
		return;
	}
}

void Attack_Bullet::HitSparkle(const Vec2& position)
{
	Effect* ef = EffectFactory::getInstance()->CreateEffect(GetObjectLayer(), EffectID::BulletHit);
	ef->setCascadeColorEnabled(true);
	ef->setPosition(position);
	ef->setColor(getColor());
}

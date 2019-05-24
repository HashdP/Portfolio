#pragma execution_character_set("utf-8")

#include "TutorialLayer.hpp"
#include "SoundManager.hpp"

USING_NS_CC;

bool TutorialLayer::init()
{
	if (!OtherWindowLayer::init(SoundID::Activate_Map))
		return false;

	Size size = Director::getInstance()->getVisibleSize();

	//�w�i
	Sprite* background = Sprite::create("Images/GUI/GUIBackground.png");
	background->setColor(Color3B::BLUE);
	this->addChild(background);

	//�������
	Label* mainTitle = Label::createWithTTF(TTFConfig("Fonts/PixelMplus10-Regular.ttf", 60), "�������");
	mainTitle->setPosition(0, size.height / 2 - 80);
	this->addChild(mainTitle);

	std::vector<std::pair<std::string, std::string>> howto_texts;
	howto_texts.emplace_back("WASD", "����");
	howto_texts.emplace_back("WASD+SHIFT", "����");
	howto_texts.emplace_back("���N���b�N", "�U���^�A�C�e���̑I��");
	howto_texts.emplace_back("E", "�I�𒆂̃A�C�e�����g�p�^�C���^���N�g");
	howto_texts.emplace_back("C", "�I�𒆂̃A�C�e�����̂Ă�");
	howto_texts.emplace_back("Q", "�񕜂���i�񕜖���������Ă���ꍇ�j");
	howto_texts.emplace_back("�}�E�X�z�C�[��", "�����ύX");
	howto_texts.emplace_back("M", "�}�b�v���J��");

	for (unsigned i = 0; i < howto_texts.size(); ++i)
	{
		float y = size.height / 2 - 180 - 65 * i;

		Label* howto_key = Label::createWithTTF(TTFConfig("Fonts/PixelMplus10-Regular.ttf", 40), howto_texts[i].first);
		howto_key->setPosition(-320, y);
		this->addChild(howto_key);

		Label* howto_sec = Label::createWithTTF(TTFConfig("Fonts/PixelMplus10-Regular.ttf", 40), "�F");
		howto_sec->setPosition(-160, y);
		this->addChild(howto_sec);

		Label* howto_des = Label::createWithTTF(TTFConfig("Fonts/PixelMplus10-Regular.ttf", 40), howto_texts[i].second);
		howto_des->setAnchorPoint(Vec2(0.0f, 0.5f));
		howto_des->setPosition(-140, y);
		this->addChild(howto_des);
	}

	return true;
}

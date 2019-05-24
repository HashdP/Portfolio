#pragma once
#include "cocos2d.h"

enum InputCode
{
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_M,
	KEY_E,
	KEY_T,
	KEY_C,
	KEY_Q,
	KEY_SHIFT,
	KEY_ESCAPE,
	MOUSE_LEFT_FIELD,
	MOUSE_LEFT_GUI,
	MOUSE_RIGHT,

	NUM
};

class InputManager
{
private:
	enum InputState
	{
		None,
		Push,
		Release
	};

private:
	InputManager() = default;
	~InputManager() = default;

	static InputManager* instance;

	InputState states[(int)InputCode::NUM] = {};
	int counts[(int)InputCode::NUM] = {};

	InputState scrollState = InputState::None;
	int mouseScroll = 0;

	inline void SetMouseScroll(int scroll) { this->scrollState = InputState::Push; this->mouseScroll = scroll; }

public:
	InputManager(const InputManager& obj) = delete;
	InputManager& operator=(const InputManager& obj) = delete;
	InputManager(InputManager&& obj) = delete;
	InputManager& operator=(InputManager&& obj) = delete;

	static InputManager* getInstance();

	void Init(cocos2d::Scene* scene);
	void Update();

	//�L�[�������^����
	void OnKeyEvent(cocos2d::EventKeyboard::KeyCode keyCode, InputState state);
	//�}�E�X�{�^���������^����
	void OnMouseEvent(const cocos2d::Vec2& mousePosition, int mouseButton, InputState state);

	//�L�[�^�}�E�X�������Ă���Œ�
	bool GetInput(InputCode InputCode);
	//�L�[�^�}�E�X���������u��
	bool GetInputDown(InputCode InputCode);
	//�L�[�^�}�E�X�𗣂����u��
	bool GetInputUp(InputCode InputCode);
	//���炩�̓��͂����������ǂ���
	bool GetAnyInputDown();

	//�}�E�X�z�C�[���̒l���擾
	inline int GetMouseScroll() { return mouseScroll; }
};

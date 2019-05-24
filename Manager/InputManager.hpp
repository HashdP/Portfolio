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

	//キーを押す／離す
	void OnKeyEvent(cocos2d::EventKeyboard::KeyCode keyCode, InputState state);
	//マウスボタンを押す／離す
	void OnMouseEvent(const cocos2d::Vec2& mousePosition, int mouseButton, InputState state);

	//キー／マウスを押している最中
	bool GetInput(InputCode InputCode);
	//キー／マウスを押した瞬間
	bool GetInputDown(InputCode InputCode);
	//キー／マウスを離した瞬間
	bool GetInputUp(InputCode InputCode);
	//何らかの入力があったかどうか
	bool GetAnyInputDown();

	//マウスホイールの値を取得
	inline int GetMouseScroll() { return mouseScroll; }
};

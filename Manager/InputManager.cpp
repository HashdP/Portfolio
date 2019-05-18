#include "Manager/InputManager.hpp"

USING_NS_CC;

InputManager* InputManager::instance = nullptr;

InputManager* InputManager::getInstance()
{
	if (instance == nullptr)
		instance = new InputManager();

	return instance;
}

void InputManager::Init(Scene* scene)
{
	//リスナーを登録する
	InputManager* instance = InputManager::getInstance();

	//キーボード
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = [instance](EventKeyboard::KeyCode keyCode, Event* event)
	{
		instance->OnKeyEvent(keyCode, InputState::Push);
	};
	keyboardListener->onKeyReleased = [instance](EventKeyboard::KeyCode keyCode, Event* event)
	{
		instance->OnKeyEvent(keyCode, InputState::Release);
	};
	scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, scene);

	//マウス
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = [instance](Event* event)
	{
		EventMouse* mouse = (EventMouse*)event;
		instance->OnMouseEvent(mouse->getLocation(), mouse->getMouseButton(), InputState::Push);
	};
	mouseListener->onMouseUp = [instance](Event* event)
	{
		EventMouse* mouse = (EventMouse*)event;
		instance->OnMouseEvent(mouse->getLocation(), mouse->getMouseButton(), InputState::Release);
	};
	mouseListener->onMouseScroll = [instance](Event* event)
	{
		EventMouse* mouse = (EventMouse*)event;
		instance->SetMouseScroll(mouse->getScrollY());
	};
	scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, scene);

	for (int i = 0; i < InputCode::NUM; ++i)
	{
		states[i] = InputState::None;
		counts[i] = 0;
	}
}

void InputManager::Update()
{
	//ステイトによってカウントを変更する
	for (int i = 0; i < (int)InputCode::NUM; ++i)
	{
		InputState state = states[i];

		if (state == InputState::None)
		{
			counts[i] = 0;
		}
		else if (state == InputState::Push)
		{
			counts[i]++;
		}
		else if (state == InputState::Release)
		{
			counts[i] = -1;
			states[i] = InputState::None;
		}
	}

	if (scrollState == InputState::Push)
		scrollState = InputState::None;
	else
		mouseScroll = 0;
}

void InputManager::OnKeyEvent(EventKeyboard::KeyCode keyCode, InputState state)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_W)
		states[(int)InputCode::KEY_W] = state;
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
		states[(int)InputCode::KEY_A] = state;
	if (keyCode == EventKeyboard::KeyCode::KEY_S)
		states[(int)InputCode::KEY_S] = state;
	if (keyCode == EventKeyboard::KeyCode::KEY_D)
		states[(int)InputCode::KEY_D] = state;
	if (keyCode == EventKeyboard::KeyCode::KEY_M)
		states[(int)InputCode::KEY_M] = state;
	if (keyCode == EventKeyboard::KeyCode::KEY_E)
		states[(int)InputCode::KEY_E] = state;
	if (keyCode == EventKeyboard::KeyCode::KEY_T)
		states[(int)InputCode::KEY_T] = state;
	if (keyCode == EventKeyboard::KeyCode::KEY_C)
		states[(int)InputCode::KEY_C] = state;
	if (keyCode == EventKeyboard::KeyCode::KEY_SHIFT)
		states[(int)InputCode::KEY_SHIFT] = state;
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
		states[(int)InputCode::KEY_ESCAPE] = state;
}

void InputManager::OnMouseEvent(const Vec2& mousePosition, int mouseCode, InputState state)
{
	if (mouseCode == MOUSE_BUTTON_LEFT)
	{
		if (mousePosition.y <= 440)
			states[(int)InputCode::MOUSE_LEFT_FIELD] = state;
		else
			states[(int)InputCode::MOUSE_LEFT_GUI] = state;
	}
	if (mouseCode == MOUSE_BUTTON_RIGHT)
		states[(int)InputCode::MOUSE_RIGHT] = state;
}

bool InputManager::GetInput(InputCode InputCode)
{
	return 0 < counts[(int)InputCode];
}

bool InputManager::GetInputDown(InputCode InputCode)
{
	return counts[(int)InputCode] == 1;
}

bool InputManager::GetInputUp(InputCode InputCode)
{
	return counts[(int)InputCode] == -1;
}

bool InputManager::GetAnyInputDown()
{
	for (int i = 0; i < InputCode::NUM; ++i)
	{
		if (counts[i] == 1)
			return true;
	}

	return false;
}

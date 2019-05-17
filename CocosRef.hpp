#pragma once
#include "cocos2d.h"

namespace myutil
{
	//shared_ptr���g�p����
	template<class T>
	using cocos_shared_ref = std::shared_ptr<T>;

	template<class T>
	cocos_shared_ref<T> make_cocos_shared_ref(T* ptr)
	{
		ptr->retain();
		return cocos_shared_ref<T>(ptr, [](T *qtr) { qtr->release(); });
	}

	//���N���X��addChild����Node���󂯎��Ǘ�����
	//make_cocos_unique_ref��retain
	//�f���[�^��release
	struct cocos_deleter
	{
		void operator()(cocos2d::Ref *ptr)
		{
			ptr->release();
		}
	};

	template<class T>
	using cocos_unique_ref = std::unique_ptr<T, cocos_deleter>;

	template<class T>
	cocos_unique_ref<T> make_cocos_unique_ref(T* ptr)
	{
		ptr->retain();
		return cocos_unique_ref<T>(ptr);
	}
}

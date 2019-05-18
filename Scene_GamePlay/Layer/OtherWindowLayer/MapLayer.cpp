#include "Scene_GamePlay/Layer/OtherWindowLayer/MapLayer.hpp"
#include "Scene_GamePlay/MapNode.hpp"
#include "Scene_GamePlay/Route.hpp"
#include "GameObject/Character/Character.hpp"
#include "myutil.hpp"
#include "Scene_GamePlay/Layer/ObjectLayer.hpp"
#include "Manager/SoundManager.hpp"
#include "SaveData.hpp"

USING_NS_CC;

bool MapLayer::init()
{
	if (!OtherWindowLayer::init(SoundID::Activate_Map))
		return false;

	if (SaveData::getInstance()->LoadContinueFlag())
	{
		//�}�b�v�����[�h
		map = SaveData::getInstance()->LoadMap();
	}
	else
	{
		//�}�b�v���쐬����
		std::shared_ptr<MapNode> baseMap(new MapNode());
		map.push_back(baseMap);

		//�����̏��
		std::array<RouteState, 4> enterState = { RouteState::Dirt,RouteState::HeavyDirt,RouteState::BadDirt,RouteState::DangerousDirt };
		std::mt19937_64 getRandMt(random<int>(0, INT_MAX));
		std::shuffle(enterState.begin(), enterState.end(), getRandMt);

		//���[�g�̒��������߂�
		std::array<int, 4> routeLength;
		for (unsigned i = 0; i < enterState.size(); ++i)
		{
			switch (enterState[i])
			{
			case RouteState::Dirt:          routeLength[i] = 2; break;
			case RouteState::HeavyDirt:     routeLength[i] = 3; break;
			case RouteState::BadDirt:       routeLength[i] = 4; break;
			case RouteState::DangerousDirt: routeLength[i] = 5; break;
			}
		}

		std::array<std::vector<std::shared_ptr<MapNode>>, 4> nodeTree;
		std::array<std::shared_ptr<MapNode>, 4> bossNode;
		while (0 < routeLength[0] || 0 < routeLength[1] || 0 < routeLength[2] || 0 < routeLength[3])
		{
			for (int i = 0; i < static_cast<int>(RouteDirection::RouteNum); ++i)
			{
				if (routeLength[i]-- <= 0)
					continue;

				std::shared_ptr<MapNode> newMap(new MapNode());

				//�G�̐��ƃ��[�g���x����ݒ�
				switch (enterState[i])
				{
				case RouteState::Dirt:          newMap->SetRouteLevel(1); newMap->SetEnemyNum(2); break;
				case RouteState::HeavyDirt:     newMap->SetRouteLevel(2); newMap->SetEnemyNum(4); break;
				case RouteState::BadDirt:       newMap->SetRouteLevel(3); newMap->SetEnemyNum(5); break;
				case RouteState::DangerousDirt: newMap->SetRouteLevel(4); newMap->SetEnemyNum(6); break;
				}

				RouteState thisTreeState = baseMap->GetRoute(static_cast<RouteDirection>(i)).GetState();

				if (thisTreeState == RouteState::None)
				{
					//��ԍŏ��̃m�[�h
					CreateRoute(baseMap, static_cast<RouteDirection>(i), newMap, enterState[i]);
				}
				else
				{
					//������RouteState�����肷��
					RouteState routeState = RouteState::Normal;
					switch (thisTreeState)
					{
					case RouteState::Dirt:          if (random<float>(0.0f, 1.0f) <= 1.0f) routeState = RouteState::Dirt; break;
					case RouteState::HeavyDirt:     if (random<float>(0.0f, 1.0f) < 0.2f) routeState = RouteState::Dirt; break;
					case RouteState::BadDirt:       if (random<float>(0.0f, 1.0f) < 0.4f) routeState = RouteState::Dirt; break;
					case RouteState::DangerousDirt: if (random<float>(0.0f, 1.0f) < 0.6f) routeState = RouteState::Dirt; break;
					}

					while (true)
					{
						std::shared_ptr<MapNode> addMap = nodeTree[i][random<int>(0, nodeTree[i].size() - 1)];
						if (CreateRandomRoute(addMap, newMap, routeState))
						{
							//�����_���ȃm�[�h�ɒǉ������������ꍇ
							if (!bossNode[i] || bossNode[i]->GetLengthFromBase() < newMap->GetLengthFromBase())
							{
								if (bossNode[i])
									bossNode[i]->SetBossID(CharacterID::None);

								switch (thisTreeState)
								{
								case RouteState::Dirt:          newMap->SetBossID(CharacterID::ShotBound); break;
								case RouteState::HeavyDirt:     newMap->SetBossID(CharacterID::BadBat); break;
								case RouteState::BadDirt:       newMap->SetBossID(CharacterID::PunchBound3); break;
								case RouteState::DangerousDirt: newMap->SetBossID(CharacterID::MossRat); break;
								}
								bossNode[i] = newMap;
							}
							break;
						}
						else
						{
							//���肪�����ς��Œǉ�����̂��s�\�ȃm�[�h�̏ꍇ
							myutil::EraseVectorValue<std::shared_ptr<MapNode>>(nodeTree[i], addMap);
						}
					}
				}

				nodeTree[i].emplace_back(newMap);
				map.push_back(newMap);
			}
		}
	}

	//�}�b�v�f�B�X�v���C
	Sprite* background = Sprite::create("Images/GUI/GUIBackground.png");
	background->setColor(Color3B::GREEN);
	this->addChild(background);

	for (auto m : map)
	{
		Vec2 position = Vec2(m->GetPosition().x * 100, m->GetPosition().y * 50);

		//�}�b�v�m�[�h�̃X�v���C�g

		Sprite* square = Sprite::create("Images/map_display_square.png");
		square->setPosition(position);
		mapSquare[m->GetPosition()] = square;
		this->addChild(square, 1);

		for (int i = 0; i < (int)RouteDirection::RouteNum; ++i)
		{
			RouteDirection dir = (RouteDirection)i;
			const Route route = m->GetRoute(dir);

			Point routePosition = m->GetPosition() + myutil::GetPointFromRouteDirection(dir) * 0.5f;

			if (route.GetState() != RouteState::None && routeLine.find(routePosition) == routeLine.cend())
			{
				//���[�g�̃X�v���C�g

				Vec2 dirPos = route.GetDestination(m)->GetPosition() - m->GetPosition();

				Sprite* line = Sprite::create("Images/map_display_line.png");
				line->setPosition(position + Vec2(50 * dirPos.x, 25 * dirPos.y));
				if (dir == RouteDirection::LowerLeft || dir == RouteDirection::UpperRight)
					line->setRotation(-35);
				else
					line->setRotation(35);
				this->addChild(line);

				routeLine[routePosition] = line;
			}
		}
	}

	return true;
}

void MapLayer::OnVisible(ObjectLayer& objectLayer)
{
	for (auto m : map)
	{
		//�\����ύX����
		bool isInit = true;
		//bool isInit = m->IsInitFloorMap();

		mapSquare[m->GetPosition()]->setVisible(isInit);

		if (isInit)
		{
			//�m�[�h�̐F
			if (m->GetPosition() == objectLayer.GetCurrentMap().GetPosition())
				mapSquare[m->GetPosition()]->setColor(Color3B::YELLOW);
			else if (m->GetBossID() != CharacterID::None)
				mapSquare[m->GetPosition()]->setColor(Color3B::RED);
			else
				mapSquare[m->GetPosition()]->setColor(Color3B::GRAY);

			for (int i = 0; i < (int)RouteDirection::RouteNum; ++i)
			{
				//���̐F
				const Route route = m->GetRoute((RouteDirection)i);
				Point routePosition = m->GetPosition() + myutil::GetPointFromRouteDirection((RouteDirection)i)* 0.5f;

				if (route.GetState() != RouteState::None)
					routeLine[routePosition]->setVisible(isInit);

				switch (route.GetState())
				{
				case RouteState::Normal:        routeLine[routePosition]->setColor(Color3B::WHITE); break;
				case RouteState::Dirt:          routeLine[routePosition]->setColor(Color3B::GREEN); break;
				case RouteState::HeavyDirt:     routeLine[routePosition]->setColor(Color3B::MAGENTA); break;
				case RouteState::BadDirt:       routeLine[routePosition]->setColor(Color3B::ORANGE); break;
				case RouteState::DangerousDirt: routeLine[routePosition]->setColor(Color3B::RED); break;
				}
			}
		}
		else
		{
			//��\���̏ꍇ�́A���[�g�̕\������\���ɂ���
			//�������A�ړ��悪�\������Ă���ꍇ�̓��[�g���\������
			for (int i = 0; i < (int)RouteDirection::RouteNum; ++i)
			{
				const Route route = m->GetRoute((RouteDirection)i);
				Point routePosition = m->GetPosition() + myutil::GetPointFromRouteDirection((RouteDirection)i)* 0.5f;

				if (route.GetState() != RouteState::None && !route.GetDestination(m)->IsInitFloorMap())
					routeLine[routePosition]->setVisible(isInit);
			}
		}
	}
}

const std::shared_ptr<MapNode>& MapLayer::GetMap(const Point& position)
{
	for (const auto& m : map)
	{
		if (m->GetPosition() == position)
		{
			return m;
		}
	}

	return map[0];
}

void MapLayer::CreateRoute(std::shared_ptr<MapNode> from, RouteDirection routeDir, std::shared_ptr<MapNode> to, RouteState state, bool setToProperty)
{
	std::shared_ptr<Route> route = std::make_shared<Route>(state, from, to);

	from->SetRoute(routeDir, route);
	if (setToProperty)
		to->SetLengthFromBase(from->GetLengthFromBase() + 1);

	switch (routeDir)
	{
	case RouteDirection::UpperLeft:  
		if (setToProperty)
			to->SetPosition(from->GetPosition() + Point(-1, 1));
		to->SetRoute(RouteDirection::LowerRight, route);
		break;

	case RouteDirection::LowerLeft: 
		if (setToProperty)
			to->SetPosition(from->GetPosition() + Point(-1, -1));
		to->SetRoute(RouteDirection::UpperRight, route);
		break;

	case RouteDirection::UpperRight: 
		if (setToProperty)
			to->SetPosition(from->GetPosition() + Point(1, 1));
		to->SetRoute(RouteDirection::LowerLeft, route);
		break;

	case RouteDirection::LowerRight: 
		if (setToProperty)
			to->SetPosition(from->GetPosition() + Point(1, -1));
		to->SetRoute(RouteDirection::UpperLeft, route);
		break;
	}
}

bool MapLayer::CreateRandomRoute(std::shared_ptr<MapNode> from, std::shared_ptr<MapNode> to, RouteState state)
{
	//State��None�̃��[�g��������
	std::vector<RouteDirection> rootDir{ RouteDirection::UpperLeft, RouteDirection::LowerLeft, RouteDirection::UpperRight, RouteDirection::LowerRight };

	for (const auto m : map)
	{
		Point difPoint = m->GetPosition() - from->GetPosition();
		RouteDirection existDir = RouteDirection::RouteNum;
		if      (difPoint == Point(-1, 1))  existDir = RouteDirection::UpperLeft;
		else if (difPoint == Point(-1, -1)) existDir = RouteDirection::LowerLeft;
		else if (difPoint == Point(1, 1))   existDir = RouteDirection::UpperRight;
		else if (difPoint == Point(1, -1))  existDir = RouteDirection::LowerRight;

		if (existDir != RouteDirection::RouteNum)
			myutil::EraseVectorValue(rootDir, existDir);
	}

	//�ݒu�\�ȃ��[�g���Ȃ������ꍇ
	if (rootDir.size() == 0)
		return false;

	CreateRoute(from, rootDir[random<int>(0, rootDir.size() - 1)], to, state);

	return true;
}

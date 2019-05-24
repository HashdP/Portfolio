#include "myutil.hpp"
#include "MapNode.hpp"
#include "GameObject.hpp"

USING_NS_CC;

namespace myutil
{
	std::string GetDirectionStr(Direction direction)
	{
		switch (direction)
		{
		case Direction::Back: return "Back";
		case Direction::Left: return "Left";
		case Direction::Front: return "Front";
		case Direction::Right: return "Right";
		}

		return "";
	}

	Vec2 GetDirectionVec(Direction direction)
	{
		Vec2 dirVec = Vec2::ZERO;
		switch (direction)
		{
		case Direction::Front: dirVec.y -= 1.0f; break;
		case Direction::Back:  dirVec.y += 1.0f; break;
		case Direction::Left:  dirVec.x -= 1.0f; break;
		case Direction::Right: dirVec.x += 1.0f; break;
		}
		return dirVec;
	}

	Vec2 GetDirectionVec8(const Vec2& fromPosition, const Vec2& toPosition)
	{
		Vec2 difVec = toPosition - fromPosition;
		difVec.normalize();

		float test = std::cos(M_PI * 1 / 8);

		Vec2 retVec = Vec2::ZERO;
		if (difVec.x < std::cos(M_PI * 7 / 8))
		{
			retVec = Vec2(-1, 0);
		}
		else if (std::cos(M_PI * 7 / 8) <= difVec.x && difVec.x < std::cos(M_PI * 5 / 8))
		{
			retVec = Vec2(-1, difVec.y < 0 ? -1 : 1);
		}
		else if (std::cos(M_PI * 5 / 8) <= difVec.x && difVec.x < std::cos(M_PI * 3 / 8))
		{
			retVec = Vec2(0, difVec.y < 0 ? -1 : 1);
		}
		else if (std::cos(M_PI * 5 / 8) <= difVec.x && difVec.x < std::cos(M_PI * 3 / 8))
		{
			retVec = Vec2(1, difVec.y < 0 ? -1 : 1);
		}
		else
		{
			retVec = Vec2(1, 0);
		}

		//if (difVec.x < std::cos(M_PI * 1 / 8))
		//{
		//	retVec = Vec2(-1, 0);
		//}
		//else if (std::cos(M_PI * 1 / 8) <= difVec.x && difVec.x < std::cos(M_PI * 3 / 8))
		//{
		//	retVec = Vec2(-1, difVec.y < 0 ? -1 : 1);
		//}
		//else if (std::cos(M_PI * 3 / 8) <= difVec.x && difVec.x < std::cos(M_PI * 5 / 8))
		//{
		//	retVec = Vec2(0, difVec.y < 0 ? -1 : 1);
		//}
		//else if (std::cos(M_PI * 5 / 8) <= difVec.x && difVec.x < std::cos(M_PI * 7 / 8))
		//{
		//	retVec = Vec2(1, difVec.y < 0 ? -1 : 1);
		//}
		//else if (std::cos(M_PI * 7 / 8) <= difVec.x)
		//{
		//	retVec = Vec2(1, 0);
		//}

		return retVec.getNormalized();
	}

	Direction GetDirectionFromVec(const Vec2& dirVec)
	{
		if (dirVec.x < 0.0f)
			return Direction::Left;
		else if (0.0f < dirVec.x)
			return Direction::Right;
		else if (dirVec.y < 0.0f)
			return Direction::Front;
		else if (0.0f < dirVec.y)
			return Direction::Back;

		return Direction::Num;
	}

	Vec2 ConvertPosToFloorPos(const Vec2& position)
	{
		int squareX = (position.x + 320.0f) / FIELD_SQUARE_SIZE;
		int squareY = (position.y + 80.0f) / FIELD_SQUARE_SIZE;

		return Vec2(squareX, squareY);
	}

	Vec2 ConvertFloorPosToPos(int x, int y)
	{
		return Vec2(x * FIELD_SQUARE_SIZE - 320.0f, y * FIELD_SQUARE_SIZE - 80.0f) + Vec2(FIELD_SQUARE_SIZE / 2, FIELD_SQUARE_SIZE / 2);
	}

	RouteDirection GetRouteDirectionFromFloorPos(const Vec2& floorPos)
	{
		if (floorPos.x == 0 && floorPos.y == 0)
		{
			return RouteDirection::LowerLeft;
		}
		else if (floorPos.x == 0 && floorPos.y == FIELD_Y_NUM - 1)
		{
			return RouteDirection::UpperLeft;
		}
		else if (floorPos.x == FIELD_X_NUM - 1 && floorPos.y == 0)
		{
			return RouteDirection::LowerRight;
		}
		else if (floorPos.x == FIELD_X_NUM - 1 && floorPos.y == FIELD_Y_NUM - 1)
		{
			return RouteDirection::UpperRight;
		}

		return RouteDirection::RouteNum;
	}

	Point GetPointFromRouteDirection(RouteDirection routeDirection)
	{
		switch(routeDirection)
		{
		case RouteDirection::UpperLeft: return Point(-1, 1);
		case RouteDirection::LowerLeft: return Point(-1, -1);
		case RouteDirection::UpperRight: return Point(1, 1);
		case RouteDirection::LowerRight: return Point(1, -1);
		}

		return Point(0, 0);
	}

	Animation* CreateStateAnimation(std::string file, float delay, int beginIndex, int totalNum, int sizeX, int sizeY)
	{
		Animation* animation = Animation::create();
		animation->setDelayPerUnit(delay);
		myutil::AddStateAnimation(animation, file, beginIndex, totalNum, sizeX, sizeY);
		return animation;
	}

	void AddStateAnimation(Animation* animation, std::string file, int beginIndex, int totalNum, int sizeX, int sizeY)
	{
		Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(file);

		//テクスチャのサイズからインデックスの最大と最小を取得する
		Size textureSize = texture->getContentSize();
		int maxX = textureSize.width / sizeX;
		int maxy = textureSize.height / sizeY;

		for (int i = 0; i < totalNum; ++i)
		{
			int index = i + beginIndex;

			//画像を切り取る基点
			int baseRectX = (index % maxX) * sizeX;
			int baseRectY = (index / maxX) * sizeY;

			animation->addSpriteFrameWithTexture(texture, Rect(baseRectX, baseRectY, sizeX, sizeY));
		}
	}

	Direction GetTargetDir(const Vec2& base, const Vec2& target)
	{
		Vec2 dif = target - base;

		float angle = 180 * dif.getAngle() / M_PI;

		if (-135 <= angle && angle < -45)
			return Direction::Front;
		else if (-45 <= angle && angle < 45)
			return Direction::Right;
		else if (45 <= angle && angle < 135)
			return Direction::Back;

		return Direction::Left;
	}

	void SetGlobalZOrderRecursive(Node *parent, float z)
	{
		if (parent == nullptr) return;
		parent->setGlobalZOrder(z);

		// adjust every child menu item
		Vector<Node*> &children = parent->getChildren();

		for (int i = 0; i < children.size(); ++i)
		{
			Node* child = children.at(i);
			SetGlobalZOrderRecursive(child, z);
		}
	}
}
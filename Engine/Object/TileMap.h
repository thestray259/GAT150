#pragma once
#include "Object.h"
#include "Core/Serializable.h"
#include <vector>

namespace nc
{
	class TileMap : public Object, public ISerializable
	{
	public: 
		void Create() override; 

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public: 
		class Scene* scene = nullptr; 

	private:
		int numTilesX = 0; 
		int numTilesY = 0; 
		Vector2 size; 
		Vector2 start; 
		std::vector<std::string> tileNames; 
		std::vector<int> tiles; 
	};
}
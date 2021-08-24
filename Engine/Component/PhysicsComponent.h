#pragma once
#include "Component.h"
#include "Math/Vector2.h"

namespace nc
{
	class PhysicsComponent : public Component
	{
	public: 
		void Update() override; 
		void ApplyForce(const Vector2& force) { acceleration += force; }

	public: 
		Vector2 velocity; 
		Vector2 acceleration; 

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
	};
}
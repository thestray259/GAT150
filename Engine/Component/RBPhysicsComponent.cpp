#include "RBPhysicsComponent.h" 
#include "Engine.h"

namespace nc
{
	void RBPhysicsComponent::Update()
	{
		if (!body)
		{
			body = owner->scene->engine->Get<PhysicsSystem>()->CreateBody(owner->transform.position, owner->transform.rotation, data, owner); 
		}

		owner->transform.position = PhysicsSystem::WorldToScreen(body->GetPosition()); 
		owner->transform.rotation = body->GetAngle(); 
		velocity = body->GetLinearVelocity(); 
	}

	void RBPhysicsComponent::ApplyForce(const Vector2& force)
	{
		if (body)
		{
			body->ApplyForceToCenter(force, true); 
		}
	}

	bool RBPhysicsComponent::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool RBPhysicsComponent::Read(const rapidjson::Value& value)
	{
		JSON_READ(value, data.isDynamic);
		JSON_READ(value, data.isSensor);
		JSON_READ(value, data.lockAngle);
		JSON_READ(value, data.size);
		JSON_READ(value, data.density);
		JSON_READ(value, data.friction);
		JSON_READ(value, data.restitution);
		JSON_READ(value, data.gravityScale);

		return true;
	}
}
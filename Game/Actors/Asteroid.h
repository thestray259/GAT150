#pragma once
#include "Object/Actor.h"

class Asteroid : public nc::Actor 
{
public:
	Asteroid(const nc::Transform transform, std::shared_ptr<nc::Texture> texture, float speed) : nc::Actor{ transform, texture }, speed{ speed } {}

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float fireTimer{ 0 };
	float fireRate{ 1.0f };
	float speed = 300;
};
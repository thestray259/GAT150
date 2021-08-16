#pragma once
#include "Object/Actor.h"

class Projectile : public nc::Actor
{
public:
	Projectile(const nc::Transform transform, std::shared_ptr<nc::Texture> texture, float speed) : nc::Actor{ transform, texture }, speed{ speed } {}

	void Update(float dt);

private:
	float lifetime{ 1 }; 
	float speed = 300;
};

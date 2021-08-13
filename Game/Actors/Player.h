#pragma once
#include "Object/Actor.h"
#include "Math/Vector2.h"

class Player : public nc::Actor
{
public:
	//Player(const nc::Transform transform, std::shared_ptr<nc::Shape> shape, float speed); 
	Player(const nc::Transform transform, std::shared_ptr<nc::Texture> texture, float speed);

	void Initialize() override; 

	void Update(float dt);
	void OnCollision(Actor* actor) override;

private:
	float fireTimer{ 0 }; 
	float fireRate{ 0.2f }; 
	float fireRate2{ 1.0f }; 
	float speed = 1000; 
	nc::Vector2 velocity; 
};
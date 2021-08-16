#include "Projectile.h"
#include "Engine.h"

void Projectile::Update(float dt)
{
	Actor::Update(dt);

	lifetime -= dt; 
	if (lifetime <= 0) destroy = true; // or destroy = (lifetime <= 0); 

	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);

	//std::vector<nc::Color> colors = { nc::Color::white, nc::Color::red, nc::Color::green, nc::Color::blue, nc::Color::purple, nc::Color::cyan, nc::Color::orange, nc::Color::yellow };
	std::shared_ptr<nc::Texture> particle = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("../Resources/Art/start_gold.png", scene->engine->Get<nc::Renderer>()); // needs to fixed maybe (?)
	scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 10, 0.5f, particle, 150);
}

#include "Asteroid.h"
#include "Engine.h"
#include "Player.h"
#include "Projectile.h" 

void Asteroid::Update(float dt)
{
	Actor::Update(dt); 

	if (scene->GetActor<Player>())
	{
		transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
		transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
		transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);
	}
}

void Asteroid::OnCollision(Actor* actor)
{
	if (dynamic_cast<Projectile*>(actor) && actor->tag == "Player")
	{
		destroy = true;

		std::shared_ptr<nc::Texture> particle = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("../Resources/Art/start_gold.png", scene->engine->Get<nc::Renderer>()); // needs to be fixed 
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, particle, 50);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");

		nc::Event event;
		event.name = "AddPoints";
		event.data = 150;
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}
}

#include "Player.h"
//#include "Enemy.h"
#include "Engine.h"
//#include "Projectile.h"
#include <memory>

Player::Player(const nc::Transform transform, std::shared_ptr<nc::Texture> texture, float speed) : nc::Actor{ transform, texture }, speed{ speed } {}

void Player::Initialize()
{
	// child 0 
	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ -2, 0 };
	locator->transform.localRotation = nc::DegToRad(180);
	AddChild(std::move(locator));

	// child 1
	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0, 5 };
	AddChild(std::move(locator));

	// child 2
	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0, -5 };
	AddChild(std::move(locator));

	// child 3
	//std::unique_ptr engine = std::make_unique<Actor>(nc::Transform{}, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("engine.txt"));
	//engine->transform.localPosition = nc::Vector2{ 1, 0 };
	//AddChild(std::move(engine));
}

void Player::Update(float dt)
{
	Actor::Update(dt); 

	//children[3]->transform.localRotation += 5 * dt; 

	// movement
	float thrust = 0;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == nc::InputSystem::eKeyState::Held) transform.rotation -= 5 * dt;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == nc::InputSystem::eKeyState::Held) transform.rotation += 5 * dt;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == nc::InputSystem::eKeyState::Held) thrust = speed;

	// acceleration
	nc::Vector2 acceleration; 
	acceleration = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * thrust;
	//nc::Vector2 gravity = nc::Vector2::down * 50; 
	//acceleration += gravity; 
	
	velocity += acceleration * dt; 
	transform.position += velocity * dt; 

	velocity *= 0.985f; // drag or damping

	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);

	// fire
	fireTimer -= dt; 
	if (fireTimer <=0 && scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
	{
		fireTimer = fireRate; 

		{
			std::vector<nc::Vector2> points = { {-5, -5}, {5, -5}, {0, 10}, {-5, -5} };

			nc::Transform t = children[1]->transform; 
			t.scale = 0.5f; 

			//std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("projectile.txt"), 800.0f);
			//projectile->tag = "Player"; 
			//scene->AddActor(std::move(projectile));
		}

		{
			std::vector<nc::Vector2> points = { {-5, -5}, {5, -5}, {0, 10}, {-5, -5} };

			nc::Transform t = children[2]->transform;
			t.scale = 0.5f;

			//std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("projectile.txt"), 800.0f);
			//projectile->tag = "Player";
			//scene->AddActor(std::move(projectile));
		}

		scene->engine->Get<nc::AudioSystem>()->PlayAudio("playershoot");
	}

	if (fireTimer <= 0 && scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_TAB) == nc::InputSystem::eKeyState::Pressed)
	{
		fireTimer = fireRate2;

			std::vector<nc::Vector2> points = { {-5, -5}, {5, -5}, {0, 10}, {-5, -5} };

			nc::Transform t = transform;
			t.scale = 0.5f;

			//std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("bigProj.txt"), 400.0f);
			//projectile->tag = "Player";
			//scene->AddActor(std::move(projectile));

		scene->engine->Get<nc::AudioSystem>()->PlayAudio("playershoot");
	}

	std::shared_ptr<nc::Texture> particle = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("../Resources/Art/start_gold.png", scene->engine->Get<nc::Renderer>()); 
	scene->engine->Get<nc::ParticleSystem>()->Create(children[0]->transform.position, 3, 2, particle, 50, children[0]->transform.rotation, nc::DegToRad(45)); 
}

void Player::OnCollision(Actor* actor)
{
	//if (dynamic_cast<Enemy*>(actor) || (dynamic_cast<Projectile*>(actor) && actor->tag == "Enemy"))
	//{
	//	destroy = true;
	//	scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, nc::Color::red, 50);
	//	scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");

	//	nc::Event event;
	//	event.name = "PlayerDead";
	//	event.data = std::string("yes I'm dead"); 
	//	scene->engine->Get<nc::EventSystem>()->Notify(event);
	//}
}

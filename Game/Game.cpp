#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/AudioComponent.h"
#include "GameComponent/TextComponent.h"
#include "GameComponent/PickupComponent.h"

void Game::Initialize()
{
	// create engine
	engine = std::make_unique<nc::Engine>(); 
	engine->Startup(); 
	engine->Get<nc::Renderer>()->Create("GAT150", 800, 600); 

	// register classes 
	REGISTER_CLASS(PlayerComponent); 
	REGISTER_CLASS(EnemyComponent); 
	REGISTER_CLASS(AudioComponent); 
	REGISTER_CLASS(TextComponent); 
	REGISTER_CLASS(PickupComponent); 

	// create scene
	scene = std::make_unique<nc::Scene>(); 
	scene->engine = engine.get(); 

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr))); 
	nc::SetFilePath("../Resources");

	// events 
	engine->Get<nc::EventSystem>()->Subscribe("add_score", std::bind(&Game::OnAddScore, this, std::placeholders::_1)); 
	engine->Get<nc::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1)); 

	// audio 
	scene->engine->Get<nc::AudioSystem>()->AddAudio("music", "Audio/music.mp3");
	music = scene->engine->Get<nc::AudioSystem>()->PlayAudio("music", 1, 1, true);

}

void Game::Shutdown()
{
	scene->RemoveAllActors(); 
	engine->Shutdown(); 
}

void Game::Update()
{
	engine->Update();

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed)
	{
		quit = true;
	}

	switch (state)
	{
	case Game::eState::Reset:
		Reset(); 
		break;
	case Game::eState::Title:
		Title(); 
		break;
	case Game::eState::Controls:
		Controls();
		break;
	case Game::eState::StartGame:
		StartGame(); 
		break;
	case Game::eState::StartLevel:
		StartLevel(); 
		break;
	case Game::eState::Level:
		Level(); 
		break;
	case Game::eState::PlayerDead:
		PlayerDead(); 
		break;
	case Game::eState::GameOver:
		GameOver(); 
		break;
	default:
		break;
	}

	// update score 
	auto scoreActor = scene->FindActor("Score"); 
	if (scoreActor)
	{
		scoreActor->GetComponent<TextComponent>()->SetText(std::to_string(score)); 
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<nc::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());

	engine->Get<nc::Renderer>()->EndFrame(); 
}

void Game::Reset()
{
	scene->RemoveAllActors(); 

	score = 0; 

	rapidjson::Document document;
	bool success = nc::json::Load("title.txt", document);
	assert(success);
	scene->Read(document);

	state = eState::Title; 
}

void Game::Title()
{
	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
	{
		auto title = scene->FindActor("Title");
		title->active = false;
		assert(title);

		title = scene->FindActor("TitleB");
		title->active = false;
		assert(title);

		title = scene->FindActor("TitleC");
		title->active = false;
		assert(title);

		state = eState::StartGame;
	}

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_TAB) == nc::InputSystem::eKeyState::Pressed)
	{
		auto title = scene->FindActor("Title");
		title->active = false;
		assert(title);

		state = eState::Controls;
	}
}

void Game::Controls()
{
	rapidjson::Document document;
	bool success = nc::json::Load("controls.txt", document);
	assert(success);
	scene->Read(document);

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_TAB) == nc::InputSystem::eKeyState::Pressed)
	{
		auto title = scene->FindActor("Controls");
		title->active = false;
		assert(title);

		state = eState::Reset;
	}
}

void Game::StartGame()
{
	rapidjson::Document document;
	bool success = nc::json::Load("scene.txt", document);
	assert(success);
	scene->Read(document);

	nc::TileMap tilemap;
	tilemap.scene = scene.get();
	success = nc::json::Load("map.txt", document);
	assert(success);
	tilemap.Read(document);
	tilemap.Create();
	
	state = eState::StartLevel; 
	stateTimer = 0; 
}

void Game::StartLevel()
{
	stateTimer += engine->time.deltaTime; 
	if (stateTimer >= 1)
	{
		auto player = nc::ObjectFactory::Instance().Create<nc::Actor>("Player"); 
		player->transform.position = nc::Vector2{ 400, 450 }; 
		scene->AddActor(std::move(player)); 

		spawnTimer = 2; 
		state = eState::Level; 
	}
}

void Game::Level()
{
	spawnTimer -= engine->time.deltaTime; 
	if (spawnTimer <= 0)
	{
		spawnTimer = nc::RandomRange(2, 4); 

		auto coin = nc::ObjectFactory::Instance().Create<nc::Actor>("Coin");
		coin->transform.position = nc::Vector2{ nc::RandomRange(100, 700), 100.0f };
		scene->AddActor(std::move(coin));
	}

	if (score >= 100)
	{
		state = eState::GameOver; 
	}
}

void Game::PlayerDead()
{
	rapidjson::Document document;
	bool success = nc::json::Load("dead.txt", document);
	assert(success);
	scene->Read(document);

	if (scene->GetActors<nc::Actor>().size() > 0)
	{
		scene->RemoveAllActors(); 
	}

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
	{
		state = eState::Reset;
	}
}

void Game::GameOver()
{
	scene->RemoveAllActors();

	rapidjson::Document document;
	bool success = nc::json::Load("gamewin.txt", document);
	assert(success);
	scene->Read(document);

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
	{
		state = eState::Reset;
	}
}

void Game::OnAddScore(const nc::Event& event)
{
	score += std::get<int>(event.data); 
}

void Game::OnPlayerDead(const nc::Event& event)
{
	std::cout << std::get<std::string>(event.data) << std::endl;
	state = eState::PlayerDead; 
}

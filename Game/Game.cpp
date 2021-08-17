#include "Game.h"

void Game::Initialize()
{
	// create engine
	engine = std::make_unique<nc::Engine>(); 
	engine->Startup(); 
	engine->Get<nc::Renderer>()->Create("GAT150", 800, 600); 

	// create scene
	scene = std::make_unique<nc::Scene>(); 
	scene->engine = engine.get(); 

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr))); 
	nc::SetFilePath("../Resources");

	// get font from resource system
	int size = 16;
	std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/Techtro.ttf", &size);
	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("hello world", nc::Color{ 1, 1, 1 }));
	engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);

	// score text
	//std::shared_ptr<nc::Texture> scoreTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	//scoreTexture->Create(font->CreateSurface("0000", nc::Color{ 1, 1, 1 }));
	//engine->Get<nc::ResourceSystem>()->Add("scoreTexture", scoreTexture);
	
	// game
	//nc::SetFilePath("Audio");
	engine->Get<nc::AudioSystem>()->AddAudio("playershoot", "Audio/playershoot.wav"); 
	engine->Get<nc::AudioSystem>()->AddAudio("enemyshoot", "Audio/enemyshoot.wav"); 
	engine->Get<nc::AudioSystem>()->AddAudio("gamemusic", "Audio/gameMusic.mp3"); 
	engine->Get<nc::AudioSystem>()->AddAudio("gamelose", "Audio/musicLose.mp3"); 
	engine->Get<nc::AudioSystem>()->AddAudio("gamewin", "Audio/musicWin.mp3"); 
	engine->Get<nc::AudioSystem>()->AddAudio("explosion", "Audio/explosion.wav");
	musicChannel = engine->Get<nc::AudioSystem>()->PlayAudio("gameMusic", 1, 1, true);

	engine->Get<nc::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1)); 
	//engine->Get<nc::EventSystem>()->Subscribe("PlayerHurt", std::bind(&Game::OnPlayerHurt, this, std::placeholders::_1)); 
	engine->Get<nc::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1)); 
}

void Game::Shutdown()
{
	scene->RemoveAllActors(); 
	engine->Shutdown(); 
}

void Game::Update()
{
	engine->Update();
	stateTimer += engine->time.deltaTime; 

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed)
	{
		quit = true;
	}

	switch (state)
	{
	case Game::eState::Title:
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
		{
			state = eState::StartGame; 
		}
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_TAB) == nc::InputSystem::eKeyState::Pressed)
		{
			state = eState::Controls;
		}
		break;
	case Game::eState::Controls:
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_TAB) == nc::InputSystem::eKeyState::Pressed)
		{
			state = eState::Title; 
		}
		break; 
	case Game::eState::StartGame:
		score = 0; 
		lives = 1; 
		state = eState::StartLevelOne; 
		break;
	case Game::eState::StartLevelOne:
		UpdateLevelOneStart(engine->time.deltaTime);
		break;
	case Game::eState::LevelOne:
		//scene->engine->Get<nc::AudioSystem>()->PlayAudio("gamemusic");

		if ((scene->GetActors<Enemy>().size() == 0) && (scene->GetActors<Asteroid>().size() == 0))
		{
			state = eState::StartLevelTwo;
		}
		break; 
	case Game::eState::StartLevelTwo:
		UpdateLevelTwoStart(engine->time.deltaTime);
		break;
	case Game::eState::LevelTwo:
		if ((scene->GetActors<Enemy>().size() == 0) && (scene->GetActors<Asteroid>().size() == 0))
		{
			state = eState::StartLevelThree;
		}
		break;
	case Game::eState::StartLevelThree:
		UpdateLevelThreeStart(engine->time.deltaTime);
		break;
	case Game::eState::LevelThree:
		if ((scene->GetActors<Enemy>().size() == 0) && (scene->GetActors<Asteroid>().size() == 0))
		{
			state = eState::GameWin;
		}
		break;
	case Game::eState::Game:
		if (scene->GetActors<Enemy>().size() == 0)
		{
			state = eState::GameWin; 
		}
		break;
	case Game::eState::GameWin:
		//musicChannel = engine->Get<nc::AudioSystem>()->PlayAudio("gameWin", 1, 1, true);
		break;
	case Game::eState::GameLose:
		//musicChannel = engine->Get<nc::AudioSystem>()->PlayAudio("gameLose", 1, 1, true);

		if (scene->GetActors<Enemy>().size() > 0)
		{
			scene->RemoveAllActors(); 
		}
		break;
	default:
		break;
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<nc::Renderer>()->BeginFrame();

	int size = 16;
	std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/Techtro.ttf", &size);
	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	nc::Transform t;

	switch (state)
	{
	case Game::eState::Title:
		textTexture->Create(font->CreateSurface("VECTREX 9999", nc::Color{ 0, 1, 1 })); 
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture); 
		t.position = { 400, 200 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		textTexture->Create(font->CreateSurface("Press Space To Start", nc::Color{ 0, 0, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		t.position = { 405, 400 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		textTexture->Create(font->CreateSurface("Press Tab For Controls", nc::Color{ 0, 0, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		t.position = { 407, 450 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);
		break;
	case Game::eState::Controls:
		// Controls 
		textTexture->Create(font->CreateSurface("W = Foward", nc::Color{ 1, 1, 0 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		t.position = { 400, 250 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		textTexture->Create(font->CreateSurface("A = Left Turn", nc::Color{ 1, 1, 0 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		t.position = { 400, 270 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		textTexture->Create(font->CreateSurface("D = Right Turn", nc::Color{ 1, 1, 0 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		t.position = { 400, 290 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		textTexture->Create(font->CreateSurface("Spacebar = Weapon 1", nc::Color{ 1, 1, 0 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		t.position = { 400, 330 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		textTexture->Create(font->CreateSurface("Tab = Weapon 2", nc::Color{ 1, 1, 0 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		t.position = { 400, 350 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		// Go back to title screen 
		textTexture->Create(font->CreateSurface("Press Tab To Return", nc::Color{ 0, 0, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		t.position = { 400, 500 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);
		break; 
	case Game::eState::StartGame:
		// score text
		//std::shared_ptr<nc::Texture> texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("scoreTexture");
		//std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/Mintello.ttf");
		//texture->Create(font->CreateSurface(std::to_string(score), nc::Color{ 1, 1, 1 }));
		//t.position = { 50, 20 };
		//engine->Get<nc::Renderer>()->Draw(texture, t);
		textTexture->Create(font->CreateSurface(std::to_string(score), nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		t.position = { 50, 20 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);
		break;
	case Game::eState::StartLevelOne:
		break;
	case Game::eState::LevelOne:
		break;
	case Game::eState::StartLevelTwo:
		break;
	case Game::eState::LevelTwo:
		break;
	case Game::eState::StartLevelThree:
		break;
	case Game::eState::LevelThree:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::GameWin:
		textTexture->Create(font->CreateSurface("You Win!", nc::Color{ 0, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		t.position = { 400, 380 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		break; 
	case Game::eState::GameLose:
		textTexture->Create(font->CreateSurface("Game Over", nc::Color{ 1, 0, 0 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		t.position = { 400, 380 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		textTexture->Create(font->CreateSurface(":'(", nc::Color{ 1, 0, 0 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
		t.position = { 400, 500 };
		engine->Get<nc::Renderer>()->Draw(textTexture, t);

		break;
	default:
		break;
	}

	// draw 
	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());

	engine->Get<nc::Renderer>()->EndFrame(); 
}

void Game::UpdateLevelOneStart(float dt)
{
	nc::SetFilePath("Art");
	scene->AddActor(std::make_unique<Player>(nc::Transform{ nc::Vector2{400, 300}, 0, 3 }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("playerShip.png", engine->Get<nc::Renderer>()), 300.0f));
	
	for (size_t i = 0; i < 2; i++)
	{
		scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("enemyShip1.png", engine->Get<nc::Renderer>()), 100.0f));
	}

	scene->AddActor(std::make_unique<Asteroid>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("asteroid1.png", engine->Get<nc::Renderer>()), 25.0f));
	//scene->engine->Get<nc::AudioSystem>()->PlayAudio("gamemusic");

	state = eState::LevelOne;
}

void Game::UpdateLevelTwoStart(float dt)
{
	for (size_t i = 0; i < 2; i++)
	{
		scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("enemyShip1.png", engine->Get<nc::Renderer>()), 100.0f));
	}
	scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("enemyShip2.png", engine->Get<nc::Renderer>()), 100.0f));

	for (size_t i = 0; i < 2; i++)
	{
		scene->AddActor(std::make_unique<Asteroid>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("asteroid1.png", engine->Get<nc::Renderer>()), 25.0f));
	}

	state = eState::LevelTwo; 
}

void Game::UpdateLevelThreeStart(float dt)
{
	for (size_t i = 0; i < 3; i++)
	{
		scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("enemyShip1.png", engine->Get<nc::Renderer>()), 100.0f));
	}

	for (size_t i = 0; i < 2; i++)
	{
		scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("enemyShip2.png", engine->Get<nc::Renderer>()), 100.0f));
	}

	for (size_t i = 0; i < 3; i++)
	{
		scene->AddActor(std::make_unique<Asteroid>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("asteroid1.png", engine->Get<nc::Renderer>()), 25.0f));
	}

	state = eState::LevelThree; 
}

void Game::OnAddPoints(const nc::Event& event)
{
	score += std::get<int>(event.data); 
}

//void Game::OnPlayerHurt(const nc::Event& event)
//{
//	lives--; 
//
//	if (lives == 0)
//	{
//		state = eState::GameOver; 
//	}
//}

void Game::OnPlayerDead(const nc::Event& event)
{
	lives = 0;
	std::cout << std::get<std::string>(event.data) << std::endl;
	state = eState::GameLose;
}

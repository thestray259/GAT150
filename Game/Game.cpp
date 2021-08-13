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

	nc::SetFilePath("../Resources");

	//std::shared_ptr<nc::Texture> texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("sf2.png", engine->Get<nc::Renderer>()); // first guy
	//std::shared_ptr<nc::Texture> player = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Art/playerShip.png", engine->Get<nc::Renderer>()); // player
	particleTexture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", engine->Get<nc::Renderer>());

	engine->Get<nc::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("music", "audio/gameMusic.mp3");
	musicChannel = engine->Get<nc::AudioSystem>()->PlayAudio("music", 1, 1, true);

	//for (size_t i = 0; i < 10; i++)
	//{
	//	nc::Transform transform{ {nc::RandomRangeInt(0, 800), nc::RandomRangeInt(0, 600)}, static_cast<float>(nc::RandomRangeInt(0, 360)), 1.0f };
	//	std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, player);
	//	scene->AddActor(std::move(actor));
	//}

	// get font from resource system
	int size = 16;
	std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/Techtro.ttf", &size);

	// create font texture
	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());

	// set font texture with font surface
	textTexture->Create(font->CreateSurface("hello world", nc::Color{ 1, 1, 1 }));

	// add font texture to resource system
	engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);
	
	// game
	engine->Get<nc::AudioSystem>()->AddAudio("playershoot", "playershoot.wav"); 
	engine->Get<nc::AudioSystem>()->AddAudio("enemyshoot", "enemyshoot.wav"); 
	engine->Get<nc::AudioSystem>()->AddAudio("gamemusic", "gameMusic.mp3"); 
	engine->Get<nc::AudioSystem>()->AddAudio("gamelose", "musicLose.mp3"); 
	engine->Get<nc::AudioSystem>()->AddAudio("gamewin", "musicWin.mp3"); 

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

		//if ((scene->GetActors<Enemy>().size() == 0) && (scene->GetActors<Asteroid>().size() == 0))
		//{
		//	state = eState::StartLevelTwo;
		//}
		break; 
	case Game::eState::StartLevelTwo:
		UpdateLevelTwoStart(engine->time.deltaTime);
		break;
	case Game::eState::LevelTwo:
		//if ((scene->GetActors<Enemy>().size() == 0) && (scene->GetActors<Asteroid>().size() == 0))
		//{
		//	state = eState::StartLevelThree;
		//}
		break;
	case Game::eState::StartLevelThree:
		UpdateLevelThreeStart(engine->time.deltaTime);
		break;
	case Game::eState::LevelThree:
		//if ((scene->GetActors<Enemy>().size() == 0) && (scene->GetActors<Asteroid>().size() == 0))
		//{
		//	state = eState::GameWin;
		//}
		break;
	case Game::eState::Game:
		//if (scene->GetActors<Enemy>().size() == 0)
		//{
		//	state = eState::GameWin; 
		//}
		break;
	case Game::eState::GameWin:
		//scene->engine->Get<nc::AudioSystem>()->PlayAudio("gamewin");
		break; 
	case Game::eState::GameLose:
		//scene->engine->Get<nc::AudioSystem>()->PlayAudio("gamelose");

		//if (scene->GetActors<Enemy>().size() > 0)
		//{
		//	scene->RemoveAllActors(); 
		//}
		break;
	default:
		break;
	}
	
	// update
	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed)
	{
		quit = true;
	}

	//if (engine->Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Pressed)
	//{
	//	nc::Vector2 position = engine->Get<nc::InputSystem>()->GetMousePosition();
	//	engine->Get<nc::ParticleSystem>()->Create(position, 10, 2.0f, particleTexture, 50.0f, nc::DegToRad(45), 10.0f);
	//	engine->Get<nc::AudioSystem>()->PlayAudio("explosion", 1, nc::RandomRange(0.2f, 2.0f));
	//	musicChannel.SetPitch(nc::RandomRange(0.2f, 2.0f));
	//}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<nc::Renderer>()->BeginFrame();

	switch (state)
	{
	case Game::eState::Title:
		//graphics.SetColor(nc::Color::green); 
		//graphics.DrawString(370, 300 + static_cast<int>((std::sin(stateTimer * 3.0f)) * 5.0f), "VECTREX 9999"); 

		//graphics.SetColor(nc::Color::cyan);
		//graphics.DrawString(340, 360, "Press Space To Start");

		//graphics.SetColor(nc::Color::cyan);
		//graphics.DrawString(320, 380, "Press Tab For Controls");
		break;
	case Game::eState::Controls:
		//graphics.SetColor(nc::Color::orange);
		//graphics.DrawString(378, 250, "W = Foward");
		//graphics.DrawString(371, 270, "A = Left Turn");
		//graphics.DrawString(368, 290, "D = Right Turn");
		//graphics.DrawString(355, 330, "Spacebar = Weapon 1");
		//graphics.DrawString(360, 350, "Tab = Weapon 2");

		//graphics.SetColor(nc::Color::cyan);
		//graphics.DrawString(340, 380, "Press Tab To Return");
		break; 
	case Game::eState::StartGame:
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
		//graphics.SetColor(nc::Color::blue);
		//graphics.DrawString(370, 380, "You Win!");
		break; 
	case Game::eState::GameLose:
		//graphics.SetColor(nc::Color::red);
		//graphics.DrawString(370, 380, "Game Over");
		break;
	default:
		break;
	}

	// draw 
	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());

	nc::Transform t;
	t.position = { 30, 30 };
	engine->Get<nc::Renderer>()->Draw(textTexture, t);

	engine->Get<nc::Renderer>()->EndFrame(); 

	//graphics.SetColor(nc::Color::white); 
	//graphics.DrawString(30, 20, std::to_string(score).c_str()); 
	//graphics.DrawString(750, 20, std::to_string(lives).c_str()); 

}

void Game::UpdateTitle(float dt)
{
	//if (Core::Input::IsPressed(VK_SPACE))
	//{
	//	stateFunction = &Game::UpdateLevelOneStart;
	//	//state = eState::StartGame;
	//}
}

void Game::UpdateLevelOneStart(float dt)
{
	nc::SetFilePath("../Resources/Art");
	scene->AddActor(std::make_unique<Player>(nc::Transform{ nc::Vector2{400, 300}, 0, 3 }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("playerShip.png", engine->Get<nc::Renderer>()), 300.0f));
	
	//for (size_t i = 0; i < 2; i++)
	//{
	//	scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("enemy.txt"), 100.0f));
	//}

	//scene->AddActor(std::make_unique<Asteroid>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("asteroid.txt"), 25.0f));
	scene->engine->Get<nc::AudioSystem>()->PlayAudio("gamemusic");

	state = eState::LevelOne;
}

void Game::UpdateLevelTwoStart(float dt)
{
	//for (size_t i = 0; i < 2; i++)
	//{
	//	scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("enemy.txt"), 100.0f));
	//}
	//scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("enemy2.txt"), 150.0f));

	//for (size_t i = 0; i < 2; i++)
	//{
	//	scene->AddActor(std::make_unique<Asteroid>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("asteroid.txt"), 25.0f));
	//}

	//state = eState::LevelTwo; 
}

void Game::UpdateLevelThreeStart(float dt)
{
	//for (size_t i = 0; i < 3; i++)
	//{
	//	scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("enemy.txt"), 100.0f));
	//}

	//for (size_t i = 0; i < 2; i++)
	//{
	//	scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("enemy2.txt"), 150.0f));
	//}

	//for (size_t i = 0; i < 3; i++)
	//{
	//	scene->AddActor(std::make_unique<Asteroid>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("asteroid.txt"), 25.0f));

	//}

	//state = eState::LevelThree; 
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

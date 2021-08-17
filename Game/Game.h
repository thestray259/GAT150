#pragma once
#include "Engine.h"

class Game
{
public:
	enum class eState
	{
		Title, 
		Controls, 
		StartGame, 
		StartLevelOne,
		LevelOne, 
		StartLevelTwo,
		LevelTwo, 
		StartLevelThree,
		LevelThree, 
		Game,
		GameWin,
		GameLose
	};

public: 
	void Initialize(); 
	void Shutdown(); 

	void Update(); 
	void Draw(); 

	bool IsQuit() { return quit; }

private: 
	void UpdateLevelOneStart(float dt); 
	void UpdateLevelTwoStart(float dt); 
	void UpdateLevelThreeStart(float dt); 

	void OnAddPoints(const nc::Event& event);
	//void OnPlayerHurt(const nc::Event& event);
	void OnPlayerDead(const nc::Event& event);

public: 
	std::unique_ptr<nc::Engine> engine; 
	std::unique_ptr<nc::Scene> scene; 

private: 
	bool quit = false; 
	eState state = eState::Title; 
	float stateTimer = 0.0f; 

	size_t score = 0; 
	size_t lives = 0; 

	nc::AudioChannel musicChannel; 
	std::shared_ptr<nc::Texture> particleTexture; 
	std::shared_ptr<nc::Texture> textTexture; 
};
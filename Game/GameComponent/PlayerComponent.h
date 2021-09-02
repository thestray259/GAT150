#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"
//#include "ProjectileComponent.h"

class PlayerComponent : public nc::Component
{
public: 
	virtual ~PlayerComponent();

	std::unique_ptr<Object> Clone() const { return std::make_unique<PlayerComponent>(*this); }

	void Create() override; 
	virtual void Update() override;

	virtual void OnCollisionEnter(const nc::Event& event);
	virtual void OnCollisionExit(const nc::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public: 
	float speed = 0; 
	float jump = 0; 
	float fireTimer{ 0 };
	float fireRate{ 0.2f };

private: 
	std::list<nc::Actor*> contacts; 
};
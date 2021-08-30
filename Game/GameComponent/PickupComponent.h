#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"

class PickupComponent : public nc::Component
{
public: 
	void Create() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(const nc::Event& event);
	virtual void OnCollisionExit(const nc::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

private:
	bool destroy{ false };
	std::list<nc::Actor*> contacts;
};
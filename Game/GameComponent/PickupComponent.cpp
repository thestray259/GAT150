#include "PickupComponent.h"
#include "Engine.h"

using namespace nc;

PickupComponent::~PickupComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
}

void PickupComponent::Create()
{
	owner->scene->engine->Get<nc::EventSystem>()->Subscribe("collision_enter", std::bind(&PickupComponent::OnCollisionEnter, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("coin", "Audio/coin.wav");
}

void PickupComponent::Update()
{
	//
}

void PickupComponent::OnCollisionEnter(const nc::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "Player"))
	{
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("coin");
		owner->destroy = true; 

		Event event; 
		event.name = "add_score"; 
		event.data = 10; 

		owner->scene->engine->Get<EventSystem>()->Notify(event); 
	}
}

bool PickupComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PickupComponent::Read(const rapidjson::Value& value)
{
	return true;
}
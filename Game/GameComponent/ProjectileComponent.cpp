#include "ProjectileComponent.h" 
#include "Engine.h" 

using namespace nc; 

void ProjectileComponent::Update()
{
}

bool ProjectileComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool ProjectileComponent::Read(const rapidjson::Value& value)
{
	return true;
}

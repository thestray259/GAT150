#include "TextComponent.h"

using namespace nc;

void TextComponent::Update()
{
}

void TextComponent::Draw(Renderer* renderer)
{
	renderer->Draw(texture, owner->transform);
}

void TextComponent::SetText(const std::string& text)
{
	texture->Create(font->CreateSurface(text, color));
}

bool TextComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool TextComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, fontName); 
	JSON_READ(value, fontSize); 
	JSON_READ(value, color); 
	JSON_READ(value, text); 

	font = owner->scene->engine->Get<ResourceSystem>()->Get<nc::Font>(fontName, &fontSize); 
	texture = owner->scene->engine->Get<ResourceSystem>()->Get<nc::Texture>("texture", owner->scene->engine->Get<Renderer>()); 

	SetText(text); 

	return true;
}

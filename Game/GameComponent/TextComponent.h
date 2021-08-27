#pragma once
#include "Component/GraphicsComponent.h" 
#include "Engine.h"

class Texture;
class Font;

class TextComponent : public nc::GraphicsComponent
{
public: 
	virtual void Update() override;
	virtual void Draw(nc::Renderer* renderer) override;

	void SetText(const std::string& text);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override; 

public: 
	std::string fontName; 
	int fontSize = 1; 
	nc::Color color; 
	std::string text; 

	std::shared_ptr<nc::Font>(font); 
	std::shared_ptr<nc::Texture>(texture);
};
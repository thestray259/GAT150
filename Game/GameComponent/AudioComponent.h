#pragma once
#include "Component/Component.h"
#include "Audio/AudioChannel.h"

class AudioComponent : public nc::Component
{
public:
	virtual void Update() override;

	void Play();
	void Stop();

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public: 
	std::string soundName; 
	float volume = 1; 
	float pitch = 1; 
	bool loop = false; 
	bool playOnAwake = false; 

private: 
	nc::AudioChannel channel;
	bool played{ false };
};
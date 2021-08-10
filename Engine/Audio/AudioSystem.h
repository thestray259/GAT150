#pragma once
#include "AudioChannel.h"
#include "Framework/System.h"
#include <fmod.hpp>
#include <string>
#include <map>

namespace nc
{
	class AudioSystem : public System
	{
	public:
		void Startup();
		void Shutdown();

		void Update(float dt);

		void AddAudio(const std::string& name, const std::string& filename);
		AudioChannel PlayAudio(const std::string& name, float volume = 1, float pitch = 1, bool loop = false);

	private:
		FMOD::System* fmodSystem;
		std::map<std::string, FMOD::Sound*> sounds;
	};
}

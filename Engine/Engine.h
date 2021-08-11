#pragma once

// systems 

// core 
#include "Core/Utilities.h"
#include "Core/FileSystem.h"
#include "Core/Timer.h"

// math
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"
#include "Math/Matrix22.h"
#include "Math/Matrix33.h"
#include "Math/Random.h"

// graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Graphics/ParticleSystem.h"
#include "Graphics/Font.h"

// input 
#include "Input/InputSystem.h"

// framework
#include "Framework/EventSystem.h"

// resource
#include "Resource/ResourceSystem.h"

// objects 
#include "Object/Actor.h"
#include "Object/Scene.h"

// audio 
#include "Audio/AudioSystem.h" 
#include "Audio/AudioChannel.h" 

#include <vector>
#include <memory>
#include <algorithm>

namespace nc
{
	class Engine
	{
	public:
		void Startup(); 
		void Shutdown(); 

		void Update();
		void Draw(Renderer* renderer); 

		template<typename T>
		T* Get(); 

	public: 
		FrameTimer time; 

	private:
		std::vector<std::unique_ptr<System>> systems; 
	};

	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems)
		{
			if (dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get()); 
		}

		return nullptr;
	}
}
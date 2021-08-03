#pragma once

// systems 

// core 
#include "Core/FileSystem.h"

// math
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"
#include "Math/Matrix22.h"
#include "Math/Matrix33.h"
#include "Math/Random.h"

// graphics

// framework
#include "Framework/EventSystem.h"

// resource
#include "Resource/ResourceSystem.h"

// objects 
#include "Object/Actor.h"
#include "Object/Scene.h"

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

		void Update(float dt);
		void Draw(); 

		template<typename T>
		T* Get(); 

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
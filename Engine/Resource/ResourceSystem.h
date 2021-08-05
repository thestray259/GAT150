#pragma once
#include "Framework/System.h"
#include "Resource.h"
#include <map>
#include <memory>
#include <algorithm>

namespace nc
{
	class ResourceSystem : public System
	{
	public: 
		void Startup() override {}
		void Shutdown() override {}
		void Update(float dt) override {}

		template <typename T> 
		std::shared_ptr<T> Get(const std::string& name, void* data = nullptr);

	private: 
		std::map<std::string, std::shared_ptr<Resource>> resources; 
	};

	template<typename T>
	inline std::shared_ptr<T> ResourceSystem::Get(const std::string& name, void* data)
	{
		if (resources.find(name) != resources.end())
		{
			return std::dynamic_pointer_cast<T>(resources[name]);
		}
		else
		{
			std::shared_ptr resource = std::make_shared<T>(); // new Shape 
			resource->Load(name, data); 
			resources[name] = resource; 

			return resource; 
		}
	}
}
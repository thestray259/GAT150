#include "Random.h"
#include <stdlib.h>

namespace nc
{
	void SeedRandom(unsigned int seed)
	{
		srand(seed); 
	}

	float Random() // 0 - 1
	{
		return rand() / static_cast<float>(RAND_MAX);
	}

	float RandomRange(float min, float max)
	{
		return min + (max - min) * Random(); // 3 - 10 = 3 + (7)
	}

	int RandomInt()
	{
		return rand();
	}

	int RandomRangeInt(int min, int max)
	{
		return min + rand() % (max - min);
	}
}
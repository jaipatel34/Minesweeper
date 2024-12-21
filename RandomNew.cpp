


#include "RandomNew.h"
#include <ctime>

std::mt19937 Random::random(time(0));

int Random::RandNum(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(random);
}





#ifndef MINESWEEPER_RANDOMNEW_H
#define MINESWEEPER_RANDOMNEW_H

#include <random>
using namespace std;

class Random
{
public:
    static int RandNum(int min, int max);
private:
    static std::mt19937 random;
};
#endif //MINESWEEPER_RANDOMNEW_H

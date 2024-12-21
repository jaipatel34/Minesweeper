

#ifndef MINESWEEPER_THREENEW_H
#define MINESWEEPER_THREENEW_H
#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>
using namespace std;


class Buttons {
private:
    sf::Texture texture;
    int x_;
    int y_;

public:
    Buttons(sf::Texture  textureName, int x, int y);
    void SetTexture(sf::Texture &textureName);
    sf::Sprite open();


};

#endif MINESWEEPER_THREENEW_H
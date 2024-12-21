
#include "ThreeNew.h"


Buttons::Buttons(sf::Texture textureName, int x, int y) {
    this->texture = textureName;
    this->x_ = x;
    this->y_= y;
}

sf::Sprite Buttons::open() {
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(x_, y_);
    return sprite;
}

void Buttons::SetTexture(sf::Texture &textureName) {
    texture = textureName;
}
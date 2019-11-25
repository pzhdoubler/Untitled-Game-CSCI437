#include <SFML/Graphics.hpp>
#include <PlayerChar.h>


PlayerChar::PlayerChar(bool player){
  this->player = player;
  this->height = 18;
  this->width = 15;
  this->in_air = true;
  this->interaction = false;
  this->powerUp = 0;
  if (player){
    fast_man.setSize(sf::Vector2f(width, height));
    fast_man.setFillColor(sf::Color::Red);
  }
  else{
    jump_man.setSize(sf::Vector2f(width, height));
    jump_man.setFillColor(sf::Color::Blue);
  }
}

void PlayerChar::draw(sf::RenderWindow &window){
    jump_man.setPosition(pos);
    window.draw(jump_man);
    fast_man.setPosition(pos);
    window.draw(fast_man);

}

sf::Vector2f PlayerChar::getPos(){
  return pos;
}

void PlayerChar::setPos(sf::Vector2f& pos){
  this->pos.x = pos.x;
  this->pos.y = pos.y;
}

void PlayerChar::setVelocity(sf::Vector2f& vel){
  this->vel.x = vel.x;
  this->vel.y = vel.y;
}

sf::Vector2f PlayerChar::getVelocity(){
  return vel;
}

int PlayerChar::getHeight(){
  return height;
}

int PlayerChar::getWidth(){
  return width;
}

bool PlayerChar::getType(){
  return player;
}

bool PlayerChar::isInAir() {
  return in_air;
}

void PlayerChar::setInAir(bool in_air) {
  this->in_air = in_air;
}

void PlayerChar::printPlayerChar() {
	if (player)
		printf("Type: Fast\n");
	else
		printf("Type: Jump\n");

	printf("Position: { %f, %f }\n", pos.x, pos.y);
	printf("Velocity: { %f, %f }\n", vel.x, vel.y);
	printf("Height: %d\n", height);
	printf("Width: %d\n", width);

	printf("\n");
}

void PlayerChar::setSpawnPt(sf::Vector2f& pos){
  this->spawn.x = pos.x;
  this->spawn.y = pos.y;
}

void PlayerChar::respawn(){
  this->vel = sf::Vector2f(0, 0);
  this->powerUp = 0;
  setPos(spawn);
}

void PlayerChar::interact(bool interact){
  this->interaction = interact;
}

bool PlayerChar::interacted(){
  return interaction;
}

void PlayerChar::setItem(int id){
    this->powerUp = id;

}

void PlayerChar::useItem(){
  if(player && this->powerUp == 1){
    this->vel.y = -300;
    this->powerUp = 0;
  }
  if(player == false && this->powerUp == 1){
  this->vel.y = -500;
  this->powerUp = 0;
}
}

int PlayerChar::getPowerUp(){
  return this->powerUp;
}

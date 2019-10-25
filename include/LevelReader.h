#ifndef _LEVELREADER_H_
#define _LEVELREADER_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <LevelState.h>

//LevelReader takes input from a text file and interprets it into arguments to pass
//to the LevelState constructor

class LevelReader {
public:
	LevelReader();

	//reads a text file with the argument given of a path to map file
	void loadMap(const std::string& level);

	LevelState createLevelState();

	void setFastSpawnPt(sf::Vector2f pt);
	void setJumpSpawnPt(sf::Vector2f pt);

	void setExitPt(sf::Vector2f pt);

private:
	LevelState currentLevelState;
	sf::Vector2f fastSpawnPt;
	sf::Vector2f jumpSpawnPt;

	sf::Vector2f exitPt;

	std::vector<std::vector<int>> map; //stores values for each tile and their id, pass to LevelState

	int tileSize;
};

#endif //_LEVELREADER_H_
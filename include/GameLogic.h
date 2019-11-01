#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include <PlayerChar.h>
#include <GameElements.h>
#include <Interactables.h>
#include <LevelState.h>
#include <Barrier.h>
#include <Controller.h>

class GameLogic
{
	public:
		//Default constructor
		GameLogic();

		//Initiallizes the gamestate based on the given LevelState description
		bool init(LevelState &level);

		//Updates internal game state for the given time
		bool update(float deltaMs);

		//Returns a collection of all drawable objects in the game state to be updated
		std::vector<GameElements*> getDrawables();

		//Recives button input and updates gamestate based on given time and button
		void buttonPress(Controller::Controls button, float deltaMs);


	private:

		enum UnitDirection
		{
			Up,
			Down,
			Left,
			Right
		};

		std::vector<int> getUnitVector(UnitDirection direction);

		int dotProduct(int v1x, int v1y, int v2x, int v2y);

		//updates player's physics based on current postion and buttons press
		void updatePlayerPosition(PlayerChar& player, float deltaMs);

		//determines if the player is going to take damage or die based on current position
		void updatePlayerState(PlayerChar& player, float deltaMs);

		//updates any moving, non-player objects in the game
		void updateObjects(float deltaMs);

		//determines where a point should be after a collision. also communicates information about which axis was effected
		//last index is 0 for x-axis and 1 for y-axis
		std::vector<float> collisionCalculation(float x, float y, UnitDirection dir);

		std::vector<int> getClosestCollisionPoint(int tile_x, int tile_y, float pos_x, float pos_y);

		std::vector<UnitDirection> getCollisionVectors(int point_x, int point_y);

		LevelState* level;

		std::shared_ptr<PlayerChar> fast_man;
		std::shared_ptr<PlayerChar> jump_man;

		std::vector<Interactables> buttons;

		std::vector<Interactables> hazards;

		std::vector<Interactables> enemies;

		std::vector<Interactables> items;

		int GRAVITY;
		int FRICTION;
		int FAST_RUN, FAST_VERT, FAST_MAX_X, FAST_MAX_Y;
		int JUMP_RUN, JUMP_VERT, JUMP_MAX_X, JUMP_MAX_Y;

};

#endif // !_GAMELOGIC_H_
#include <GameLogic.h>

#include <Hazards.h>
#include <cmath>


GameLogic::GameLogic()
{
	fast_man = std::make_shared<PlayerChar>(true);
	jump_man = std::make_shared<PlayerChar>(false);
	//initialize all interactable lists here
}


bool GameLogic::init(LevelState &level)
{
	this->level = &level;
	//Read any important values from level
	//eg hazards, enemies, etc
	//sf::Vector2f grav_test_start(200, 200);
	//static Hazards spike;
	//spike.setPos(grav_test_start);
	//Interactables* button_ptr = &spike;
	//buttons.push_back(button_ptr);

	fast_man->setPos(level.getFastSpawnPt());
	fast_man->setSpawnPt(level.getFastSpawnPt());
	jump_man->setPos(level.getJumpSpawnPt());
	jump_man->setSpawnPt(level.getJumpSpawnPt());

	//sf::Vector2f grav_test_start(200, 200);
	//fast_man->setPos(grav_test_start);

	sf::Vector2f zeros(0, 0);
	fast_man->setVelocity(zeros);
	jump_man->setVelocity(zeros);
	//set starting positions and speeds
	//stored in level state

	int tileSize = level.getTileSize();
	//read in physics constants
	GRAVITY = 1000;
	FRICTION = 1500;

	FAST_MAX_X = 2500;
	FAST_MAX_Y = 2500;
	FAST_RUN = 1000;
	FAST_HEIGHT = 4;
	FAST_VERT = std::sqrt(2*GRAVITY*FAST_HEIGHT*tileSize);
	FAST_AIR_MULT = 0.3;

	JUMP_MAX_X = 1000;
	JUMP_MAX_Y = 2000;
	JUMP_RUN = 500;
	JUMP_HEIGHT = 10;
	JUMP_VERT = std::sqrt(2*GRAVITY*JUMP_HEIGHT*tileSize);
	JUMP_AIR_MULT = 0.7;

	MIN_VERT = std::sqrt(GRAVITY*tileSize);

	return true;
}


bool GameLogic::update(float deltaMs)
{
	//level->printLevelState();

	//update player state

	updatePlayerPosition(*fast_man, deltaMs);
	updatePlayerPosition(*jump_man, deltaMs);

	//update other level objects
	return true;
}


std::vector<GameElements*> GameLogic::getDrawables() //IMPLEMENT WITH GAMEELEMENTS NEXT
{
	std::vector<GameElements*> drawables;

	//for (int i = 0; i < buttons.size(); i++) {
	//	GameElements* game_ptr = buttons[i];
	//	drawables.push_back(game_ptr);
	//}

	GameElements* fast_ptr = fast_man.get();
	GameElements* jump_ptr = jump_man.get();

	drawables.push_back(fast_ptr);
	drawables.push_back(jump_ptr);

	//add all interactables

	return drawables;
}


bool GameLogic::buttonPress(Controller::Controls button, float deltaMs)
{
	float seconds = deltaMs;
	bool success = false;

	sf::Vector2f fast_vel = fast_man->getVelocity();
	sf::Vector2f jump_vel = jump_man->getVelocity();

	switch (button)
	{
		case Controller::FAST_LEFT:
			if (!fast_man->isInAir()) {
				fast_vel.x -= (FAST_RUN + FRICTION) * seconds;
			}
			else {
				fast_vel.x -= FAST_RUN * FAST_AIR_MULT * seconds;
			}
			success = true;
			break;
		case Controller::FAST_RIGHT:
			if (!fast_man->isInAir()) {
				fast_vel.x += (FAST_RUN + FRICTION) * seconds;
			}
			else {
				fast_vel.x += FAST_RUN * FAST_AIR_MULT * seconds;
			}
			success = true;
			break;
		case Controller::FAST_JUMP:
			if (!fast_man->isInAir()) {
				fast_vel.y = (-1)*FAST_VERT;
				fast_man->setInAir(true);
				success = true;
			}
			success = true;
			break;
		case Controller::FAST_JUMP_RELEASE:
			if (fast_man->isInAir() && fast_vel.y < (-1)*MIN_VERT) {
				fast_vel.y = (-1)*MIN_VERT;
				success = true;
			}
			break;
		case Controller::FAST_DOWN:
			break;
		case Controller::FAST_USE:
			break;
		case Controller::JUMP_LEFT:
			if (!jump_man->isInAir()) {
				jump_vel.x -= (JUMP_RUN + FRICTION) * seconds;
			}
			else {
				jump_vel.x -= JUMP_RUN * JUMP_AIR_MULT * seconds;
			}
			success = true;
			break;
		case Controller::JUMP_RIGHT:
			if (!jump_man->isInAir()) {
				jump_vel.x += (JUMP_RUN + FRICTION) * seconds;
			}
			else {
				jump_vel.x += JUMP_RUN * JUMP_AIR_MULT * seconds;
			}
			success = true;
			break;
		case Controller::JUMP_JUMP:
			if (!jump_man->isInAir()) {
				jump_vel.y = (-1)*JUMP_VERT;
				jump_man->setInAir(true);
				success = true;
			}
			break;
		case Controller::JUMP_JUMP_RELEASE:
			if (jump_man->isInAir() && jump_vel.y < (-1)*MIN_VERT) {
				jump_vel.y = (-1)*MIN_VERT;
				success = true;
			}
			break;
		case Controller::JUMP_DOWN:
			break;
		case Controller::JUMP_USE:
			break;

		default:
			break;
	}

	fast_man->setVelocity(fast_vel);
	jump_man->setVelocity(jump_vel);

	return success;
}


std::vector<float> GameLogic::collisionCalculation(int tileXCoord, int tileYCoord, int prevTileX, int prevTileY, float posx, float posy, PointType corner)
{
	std::vector<std::vector<int>> tileMap = level->getCollisionMap();
	int tile_size = level->getTileSize();

	int x_diff, y_diff;

	//get to closest x tile border
	if ((int(posx) % tile_size) < (tile_size / 2))
		x_diff = -1 * (int(posx) % tile_size); //left side close
	else
		x_diff = tile_size - (int(posx) % tile_size); //right side close
	//get to closest y tile border
	if ((int(posy) % tile_size) < (tile_size / 2))
		y_diff = -1 * (int(posy) % tile_size); //top close
	else
		y_diff = tile_size - (int(posy) % tile_size); //bot close

	//check surrounding tiles
	int up = 0;
	int down = 0;
	int left = 0;
	int right = 0;
	if (tileYCoord > 0)
		up = tileMap[tileXCoord][tileYCoord - 1];
	if (tileYCoord < tileMap[0].size()-1)
		down = tileMap[tileXCoord][tileYCoord + 1];
	if (tileXCoord > 0)
		left = tileMap[tileXCoord - 1][tileYCoord];
	if (tileXCoord < tileMap.size() - 1)
		right = tileMap[tileXCoord + 1][tileYCoord];

	std::vector<float> to_return;

	//flat ground
	if (left == 1 && right == 1) {
		to_return.push_back(posx);
		to_return.push_back(int(posy) + y_diff);
		to_return.push_back(1);
	}
	//flat wall
	else if (up == 1 && down == 1) {
		to_return.push_back(int(posx) + x_diff);
		to_return.push_back(posy);
		to_return.push_back(0);
	}
	//top exposed corner
	else if (up != 1 && (corner == bot_left || corner == bot_right) && (int(posy) % tile_size) < (tile_size / 4)) {
		to_return.push_back(posx);
		to_return.push_back(int(posy) + y_diff);
		to_return.push_back(1);
	}
	//bot exposed corner
	else if (down != 1 && (corner == top_left || corner == top_right) && tile_size - (int(posy) % tile_size) < (tile_size / 4)) {
		to_return.push_back(posx);
		to_return.push_back(int(posy) + y_diff);
		to_return.push_back(1);
	}
	//default to x collision
	else {
		to_return.push_back(int(posx) + x_diff);
		to_return.push_back(posy);
		to_return.push_back(0);	to_return.push_back(1);
	}

	return to_return;
}


void GameLogic::updatePlayerPosition(PlayerChar& player, float deltaMs)
{
	float seconds = deltaMs;

	sf::Vector2f pos = player.getPos();
	sf::Vector2f vel = player.getVelocity();

	float new_vx = vel.x;
	float new_vy = vel.y;

	//apply friction on ground
	if (!player.isInAir() && new_vx != 0.0) {
		int sign;
		if (std::isless(new_vx, 0)) {
			sign = 1;
			new_vx += FRICTION * seconds;
		}
		else if(std::isgreater(new_vx, 0)) {
			sign = 0;
			new_vx -= FRICTION * seconds;
		}
		//passed zero
		if (sign != std::signbit(new_vx)) {
			new_vx = 0;
		}
	}

	//apply gravity
	new_vy += GRAVITY * seconds;
	player.setInAir(true);

	//limit to max vert speed
	if (std::abs(int(new_vy)) > FAST_MAX_Y) {
		if (player.getType()) { //if fast char
			new_vy = FAST_MAX_Y;
		}
		else {
			if (std::abs(int(new_vy)) > JUMP_MAX_Y) {
				new_vy = JUMP_MAX_Y;
			}
		}
	}	

	int tile_size = level->getTileSize();
	int player_height = player.getHeight();
	int player_width = player.getWidth();
	std::vector<std::vector<int>> level_layout = level->getCollisionMap();

	float deltaX = vel.x * seconds;
	float deltaY = vel.y * seconds;

	float new_x = pos.x + deltaX;
	float new_y = pos.y + deltaY;

	int prev_x = int(pos.x);
	int prev_y = int(pos.y);

	//bottom left corner
	if (level_layout[int(new_x) / tile_size][(int(new_y) + player_height) / tile_size] == 1) {
		std::vector<float> shift = collisionCalculation(int(new_x) / tile_size, (int(new_y) + player_height) / tile_size, prev_x / tile_size, (prev_y + player_height) / tile_size, new_x, new_y + player_height, bot_left);
		new_x = shift[0];
		new_y = shift[1] - player_height;
		if (shift[2] == 0) {
			new_vx = 0;
		}
		else {
			new_vy = 0;
			player.setInAir(false);
		}
	}
	//bottom right corner
	if (level_layout[(int(new_x) + player_width) / tile_size][(int(new_y) + player_height) / tile_size] == 1) {
		std::vector<float> shift = collisionCalculation((int(new_x) + player_width) / tile_size, (int(new_y) + player_height) / tile_size, (prev_x + player_width) / tile_size, (prev_y + player_height) / tile_size, new_x + player_width, new_y + player_height, bot_right);
		new_x = shift[0] - player_width;
		new_y = shift[1] - player_height;
		if (shift[2] == 0) {
			new_vx = 0;
		}
		else {
			new_vy = 0;
			player.setInAir(false);
		}
	}
	//top left corner
	if (level_layout[int(new_x)/tile_size][int(new_y)/tile_size] == 1) {
		std::vector<float> shift = collisionCalculation(int(new_x) / tile_size, int(new_y) / tile_size, prev_x / tile_size, prev_y / tile_size, new_x, new_y, top_left);
		new_x = shift[0];
		new_y = shift[1];
		if (shift[2] == 0) {
			new_vx = 0;
		}
		else {
			new_vy = 0;
			//player.setInAir(false);
		}
	}
	//top right corner
	if (level_layout[(int(new_x) + player_width) / tile_size][int(new_y) / tile_size] == 1) {
		std::vector<float> shift = collisionCalculation((int(new_x) + player_width) / tile_size, int(new_y) / tile_size, (prev_x + player_width) / tile_size, prev_y / tile_size, new_x + player_width, new_y, top_right);
		new_x = shift[0] - player_width;
		new_y = shift[1];
		if (shift[2] == 0) {
			new_vx = 0;
		}
		else {
			new_vy = 0;
			//player.setInAir(false);
		}
	}


	sf::Vector2f new_pos = sf::Vector2f(new_x, new_y);
	sf::Vector2f new_vel = sf::Vector2f(new_vx, new_vy);

	player.setPos(new_pos);
	player.setVelocity(new_vel);

}
//if bad
//then git gud



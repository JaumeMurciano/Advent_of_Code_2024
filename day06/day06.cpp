#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <map>
#include <set>

struct Coordinate
{
	int32_t x{};
	int32_t y{};
	Coordinate operator+(const Coordinate& movDirection) const { return { x + movDirection.x, y + movDirection.y }; }
	bool operator==(const Coordinate& otherCoordinate) const { return x == otherCoordinate.x && y == otherCoordinate.y; }
	bool operator<(const Coordinate& otherCoordinate) const {
		if (x < otherCoordinate.x) { return true; }
		if (x == otherCoordinate.x && y < otherCoordinate.y) { return true; }
		return false;
	}
};

int32_t checkLoops(std::vector<std::string> inputMap, Coordinate currentPosition, const std::array<Coordinate, 4>& movementDirections, std::size_t currentDirection, std::set<Coordinate>& goodObstacles)
{
	Coordinate obstaclePosition{ currentPosition.x + movementDirections[currentDirection].x, currentPosition.y + movementDirections[currentDirection].y };	//calculate new obstacle position
	inputMap[obstaclePosition.y][obstaclePosition.x] = '#';							//set an obstacle to that position
	if (goodObstacles.find(obstaclePosition) != goodObstacles.end()) { return 0; }	//if we have already detected an obstacle there, stop calculating
	std::map<Coordinate, std::set<Coordinate>> alreadyTurned{};						//store where have we turned, and if a loop will be detected when a turn in the same direction is detected in one position

	while (true)
	{
		Coordinate newPosition = currentPosition + movementDirections[currentDirection];	//try to move
		if (newPosition.x < 0 || newPosition.y < 0 || newPosition.x >= inputMap[0].size() || newPosition.y >= inputMap.size()) { return 0; }	//if we go out of the map there is not a loop
		if (inputMap[newPosition.y][newPosition.x] == '#') {
			if (alreadyTurned[currentPosition].find(movementDirections[currentDirection]) != alreadyTurned[currentPosition].end()) {
				goodObstacles.emplace(obstaclePosition); //an obstacle has been detected, emplace it in result
				return 1; 
			}
			alreadyTurned[currentPosition].emplace(movementDirections[currentDirection]);	//if that turn was not performed, add it to the list and turn
			currentDirection++; 
			currentDirection = currentDirection % 4; 
		}
		else {
			currentPosition = newPosition;				//if there is not an obstacle, advance
		}
	}
	return 0;
}

int main()
{
	std::ifstream myReadFile("day06_input.txt");
	std::string inputLine{};

	int64_t part1{1};							//count starting position
	Coordinate currentPosition{};				//variable that will be used to move around the map
	std::vector<std::string> inputMap{};
	
	while (std::getline(myReadFile, inputLine))
	{
		std::size_t myPos = inputLine.find("^"); //search for initial position
		if (myPos != std::string::npos)
		{
			currentPosition.x = static_cast<int32_t>(myPos);
			currentPosition.y = static_cast<int32_t>(inputMap.size());
		}
		inputMap.emplace_back(inputLine);		//save input map
	}

	std::array<Coordinate, 4> movementDirections{ {{0, -1},{1, 0},{0, 1},{-1, 0}} }; //possible movements
	std::size_t currentDirection{};				//currrent direction in movementDirections
	const Coordinate initialPosition = currentPosition;		//save initial position so that an obstacle is not put there
	std::set<Coordinate> goodObstacles{};		//list of unique good obstacles that create loops
	while (true)
	{
		Coordinate newPosition = currentPosition + movementDirections[currentDirection];									//try to move to the current direction
		if (newPosition.x <0 || newPosition.y < 0 || newPosition.x >= inputMap[0].size() || newPosition.y >= inputMap.size()) { break; }	//if we get out of the map, stop simulating
		if (inputMap[newPosition.y][newPosition.x] == '#') { currentDirection++; currentDirection = currentDirection % 4; }	//if we encounter an obstacle, turn
		else {
			if (!(newPosition == initialPosition) && inputMap[newPosition.y][newPosition.x] == '.')							//if we have not visited the position and it is not the starting position,try to emplace an obstacle there
			{ 
				checkLoops(inputMap, currentPosition, movementDirections, currentDirection, goodObstacles);					//if a new obstacle is detected, it will be emplaced in goodObstacles
			}
			if (inputMap[newPosition.y][newPosition.x] == '.') { inputMap[newPosition.y][newPosition.x] = 'X'; part1++; }	//if the position has not been visited, mark it and add 1 for part 1
			currentPosition = newPosition;		//advance position
		}
	}
	
	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << goodObstacles.size();
	return 0;
}
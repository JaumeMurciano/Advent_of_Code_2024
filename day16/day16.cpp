#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>
#include <queue>
#include <map>

struct Point
{
	int32_t row{};
	int32_t column{};
	Point operator+(const Point& otherPoint) const { return Point{ row + otherPoint.row, column + otherPoint.column }; }	//operator used to add positions
	bool operator==(const Point& otherPoint) const { return row == otherPoint.row && column == otherPoint.column; }			//operator used to compare equal positions
	bool goingBackwards(const Point& otherPoint) const { return !(abs(row - otherPoint.row) < 2 && abs(column - otherPoint.column) < 2); }
	bool operator<(const Point& otherPoint) const		//operator used with std::set
	{
		if (row < otherPoint.row) { return true; }
		if (row == otherPoint.row && column < otherPoint.column) { return true; }
		return false;
	}
};

struct Simulation
{
	Point position{};					//current simulation position
	Point movementDirection{};			//current simulation movement direction
	int64_t currentPoints{};			//current points in current simulation 
	std::vector<Point> pressedTiles{};	//current simulation path for part 2

	bool operator>(const Simulation& otherSim) const { return currentPoints > otherSim.currentPoints; }	//operator used with std::prioriy_queue
};

int main()
{
	std::ifstream myReadFile("day16_input.txt");
	std::string inputLine{};

	int64_t part1{};
	std::vector<std::string> inputMap{};

	while (std::getline(myReadFile, inputLine))
	{
		inputMap.emplace_back(inputLine);														//store input map
	}

	Simulation startingSim{ Point{static_cast<int32_t>(inputMap.size()) - 2,1}, Point{0,1},0 };	//starting position going to the east
	startingSim.pressedTiles.emplace_back(startingSim.position);								//emplace starting position pressed tile
	std::priority_queue<Simulation, std::vector<Simulation>, std::greater<Simulation>> simulationQueue{};	//priority queue
	std::map<Point, int64_t> simulationCache{};													//cache used to discard inefficient paths
	std::array<Point, 4> possibleMovements{ {{0,1},{1,0},{0,-1},{-1,0}} };						//possible movements
	std::set<Point> bestTiles{};																//pressed tiles for part 2

	simulationQueue.emplace(startingSim);

	while (!simulationQueue.empty())
	{
		Simulation currentSim = simulationQueue.top();											//get the least cost simulation
		simulationQueue.pop();																	//erase it from the queue
		
		if (currentSim.position.column == inputMap[1].size() - 2 && currentSim.position.row == 1) 
		{ 
			part1 = currentSim.currentPoints;													//if the simulation has ended, save least points
			for (Point eachTile : currentSim.pressedTiles)										//for each with the minimum path, save all pressed tiles
			{
				bestTiles.emplace(eachTile);
			}
		}
		if (part1 != 0 && currentSim.currentPoints > part1) { break; }							//stop searching when current simulation has more points than minimum ended

		for (Point& eachMovement : possibleMovements)
		{
			Point newPosition = currentSim.position + eachMovement;								//try a new position for each movement
			if (currentSim.movementDirection.goingBackwards(eachMovement)) { continue; }		//going backwards is never a good option, as that tile has already been pressed
			if (inputMap[newPosition.row][newPosition.column] == '#') { continue; }				//path is not available
			if (eachMovement == currentSim.movementDirection)									//if going backwards, create a new simulation and add 1 point
			{
				Simulation newSimulation = currentSim;
				newSimulation.position = newPosition;
				newSimulation.currentPoints += 1;
				newSimulation.pressedTiles.emplace_back(newPosition);
				if (simulationCache[newPosition] == 0 || newSimulation.currentPoints <= simulationCache[newPosition])	//add it if that tile has been pressed with less points than minimum or it has not been pressed
				{
					simulationQueue.emplace(newSimulation);
					simulationCache[newPosition] = newSimulation.currentPoints + 1000;									//add 1000 points to the minimum to let extra turn for part 2
				}
			}
			else
			{																					//repeat when turning 90º but add 1000 points for turn and 1 for advancing
				Simulation newSimulation = currentSim;
				newSimulation.position = newPosition;
				newSimulation.movementDirection = eachMovement;
				newSimulation.currentPoints += 1001;
				newSimulation.pressedTiles.emplace_back(newPosition);
				if (simulationCache[newPosition] == 0 || newSimulation.currentPoints <= simulationCache[newPosition])
				{
					simulationQueue.emplace(newSimulation);
					simulationCache[newPosition] = newSimulation.currentPoints + 1000;
				}
			}
		}
	}

	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << bestTiles.size();
	return 0;
}
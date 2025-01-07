#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>
#include <queue>
#include <unordered_map>

struct Point			 //Coordinaate structure used to represent points and movements 
{
	int32_t row{};
	int32_t column{};
	Point operator+(const Point& otherPoint) const { return Point{ row + otherPoint.row, column + otherPoint.column }; }
	bool operator==(const Point& otherPoint) const { return row == otherPoint.row && column == otherPoint.column; }
	bool operator<(const Point& otherPoint) const		//used to sort visitedPositions set in Simulation structure
	{
		if (row < otherPoint.row) { return true; }
		else if (row == otherPoint.row && column < otherPoint.column) { return true; }
		return false;
	}
};

struct Simulation		//Used to store simulations in dijkstra's std::priority_queue
{
	Point currentPosition{ 0,0 };
	int64_t currentResult{};
	std::set<Point> visitedPositions{};
	bool operator>(const Simulation& otherSim) const { return currentResult > otherSim.currentResult; }
};

//function used to determine the path cost from beginning to the end at each tile
std::vector<std::vector<int64_t>> searchDijkstra(std::vector<std::string>& inputMap, const Point& startingPosition, const Point& endingPosition)
{
	std::priority_queue<Simulation, std::vector<Simulation>, std::greater<Simulation>> simulationQueue{};
	Simulation start{ startingPosition,0,{startingPosition} };
	std::array<Point, 4> possibleMovements{ {{0,1},{1,0},{0,-1},{-1,0}} };
	std::vector<std::vector<int64_t>> costMap = std::vector<std::vector<int64_t>>(inputMap.size(), std::vector<int64_t>(inputMap[0].size(), -1)); //cost map used to store each position cost. Walls are -1
	costMap[startingPosition.row][startingPosition.column] = 0;
	simulationQueue.emplace(start);

	while (!simulationQueue.empty())
	{
		Simulation currentSimulation = simulationQueue.top();
		simulationQueue.pop();
		if (currentSimulation.currentPosition == endingPosition)
		{
			return costMap;																			//when finished, return costMap
		}
		for (Point eachMovement : possibleMovements)
		{
			Point newPosition = currentSimulation.currentPosition + eachMovement;
			if (inputMap[newPosition.row][newPosition.column] == '#') { continue; }					//cannot go through walls
			if (currentSimulation.visitedPositions.find(newPosition) != currentSimulation.visitedPositions.end()) { continue; } //already visited position
			costMap[newPosition.row][newPosition.column] = currentSimulation.currentResult + 1;		//update cost map
			Simulation newSimulation{};																//create and fill new simulation at current point
			newSimulation.currentResult = currentSimulation.currentResult + 1;
			newSimulation.currentPosition = newPosition;
			newSimulation.visitedPositions = currentSimulation.visitedPositions;
			newSimulation.visitedPositions.emplace(newPosition);
			simulationQueue.emplace(newSimulation);
		}
	}
	return {};
}
//Function used to search all cheating paths for part 2
int64_t searchAllShorterPaths(const int32_t cheatTime, const std::vector<std::vector<int64_t>>& costMap, const Point& currentPosition, int32_t timeToSave)
{
	int64_t result{};
	for(int32_t i{-cheatTime}; i <= cheatTime; i++) //search all paths that save timeToSave in a 20 tile radius diamond
	{
		for (int32_t j{ -abs(abs(i) - cheatTime) }; j <= abs(abs(i) - cheatTime); j++)
		{
			int32_t currentRow{ currentPosition.row + i };
			int32_t currentColumn{ currentPosition.column + j };
			if (currentRow < 0 || currentRow >= costMap.size()) { break; }				//prevent from going out of the map
			if (currentColumn < 0 || currentColumn >= costMap[0].size()) { continue; }	//prevent from going out of the map
			if (costMap[currentRow][currentColumn] == -1) { continue; }					//wall cannot end a cheat
			if (costMap[currentRow][currentColumn] - costMap[currentPosition.row][currentPosition.column] - abs(i) - abs(j) >= timeToSave) {
				result++; //if time is saved, add 1 to result
			}
		}
	}
	return result;
}


int main()
{
	std::ifstream myReadFile("day20_input.txt");
	std::string inputLine{};

	int64_t part1{};
	int64_t part2{};
	std::vector<std::string> inputMap{};
	Point startingPosition{};
	Point endingPosition{};

	while (std::getline(myReadFile, inputLine)) //store map and search for starting and ending position
	{
		if (inputLine.find('S') != inputLine.npos) { startingPosition.row = inputMap.size(); startingPosition.column = inputLine.find('S'); }
		if (inputLine.find('E') != inputLine.npos) { endingPosition.row = inputMap.size(); endingPosition.column = inputLine.find('E'); }
		inputMap.emplace_back(inputLine);
	}

	std::vector<std::vector<int64_t>> costMap = searchDijkstra(inputMap, startingPosition, endingPosition); //fill cost map using avobe dijkstra

	for (int32_t i{ 1 }; i < inputMap.size() - 1; i++)
	{
		for (int32_t j{ 1 }; j < inputMap[0].size() - 1; j++)
		{
			if (inputMap[i][j] != '#') {
				part1 += searchAllShorterPaths(2, costMap, Point{ i,j }, 100);	//part 1 is a 2x2 diamond
				part2 += searchAllShorterPaths(20, costMap, Point{ i,j }, 100);	//part 2 is a 20x20 diamond
				
			}
		}
	}

	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
}

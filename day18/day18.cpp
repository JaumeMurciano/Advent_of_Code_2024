#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>
#include <queue>

struct Point //struct used to store positions, operators are used to generate new points and to compare
{
	int32_t row{};
	int32_t column{};
	bool operator==(const Point& otherPoint) const { return row == otherPoint.row && column == otherPoint.column; }
	Point operator+(const Point& otherPoint) const { return Point{ row + otherPoint.row, column + otherPoint.column }; }
};

struct Simulation //struct used to store simulation status for Dijkstra
{
	Point currentPosition{};
	int32_t currentPoints{};
	bool operator>(const Simulation& otherSim) const { return currentPoints > otherSim.currentPoints; }
};

int main()
{
	std::ifstream myReadFile("day18_input.txt");
	std::string inputLine{};

	Point startingPosition{ 0,0 };
	Point endingPosition{ 70,70 };
	std::array<Point, 4> possibleMovements{ {{0,1}, {1,0}, {0,-1}, {-1,0}} };

	int64_t part1{};
	int mapSize{ 71 };					//map size is 71x71 ([0,70][0,70])
	int bytesFallen{ 1024 };			//bytes to read for part 1
	std::vector<std::string> byteMap(mapSize, std::string(mapSize, '.'));	//map where the bytes will be dropped

	while (bytesFallen > 0)				//drop first 1024 bytes
	{
		std::getline(myReadFile, inputLine);
		std::stringstream line(inputLine);
		std::string eachWord{};
		int32_t row{ -1 };
		int32_t column{ -1 };
		while (getline(line, eachWord, ','))
		{
			if (column == -1) { column = std::stoi(eachWord); }
			else { row = std::stoi(eachWord); }
		}
		byteMap[row][column] = '#';
		bytesFallen--;
	}
	Point lastByte{ -1,-1 };				//store last fallen byte for part 2
	std::vector<std::vector<int32_t>> bestCosts;	//discard-position cache (really not needed, but fastens solution)
	while (true)
	{
		std::priority_queue<Simulation, std::vector<Simulation>, std::greater<Simulation>> simulationQueue{};	//priority queue for dijsktra. Could also be done with bfs/dfs, but this is faster
		simulationQueue.emplace(Simulation{ startingPosition,0 });							//add initial simulation
		bestCosts = std::vector<std::vector<int>>(mapSize, std::vector<int>(mapSize, -1));	//start cost map
		while (!simulationQueue.empty())
		{
			Simulation currentSim = simulationQueue.top();									//get lowest cost simulation
			if (currentSim.currentPosition == endingPosition)								//if arrived to last position, stop.
			{
				if (lastByte == Point{ -1,-1 }) { part1 = currentSim.currentPoints; }
				break;
			}
			simulationQueue.pop();
			for (const Point& eachMovement : possibleMovements)	//try every possible movement and discard if it goes out of map, tile has been pressed with less cost or it is a wall
			{
				Point newPosition = currentSim.currentPosition + eachMovement;
				if (newPosition.row < 0 || newPosition.column < 0 || newPosition.row >= byteMap.size() || newPosition.column >= byteMap[0].size()) { continue; }
				if (byteMap[newPosition.row][newPosition.column] == '#') { continue; }
				Simulation newSimulation{ newPosition,currentSim.currentPoints + 1 };
				if (bestCosts[newPosition.row][newPosition.column] == -1 || newSimulation.currentPoints < bestCosts[newPosition.row][newPosition.column])
				{
					simulationQueue.emplace(newSimulation);									//add new best simulation tile
					bestCosts[newPosition.row][newPosition.column] = newSimulation.currentPoints;	//update cache
				}
			}
		}
		if (bestCosts[endingPosition.row][endingPosition.column] == -1) { break; }			//if last tile has not been pressed, it last byte is part 2 solution
		std::getline(myReadFile, inputLine);												//read one more bit for part 2. Could have been done by binary search and adding and removing bytes
		std::stringstream line(inputLine);
		std::string eachWord{};
		for (int i{}; i < 2; i++)
		{
			getline(line, eachWord, ',');
			if (i == 0) { lastByte.column = std::stoi(eachWord); }
			else { lastByte.row = std::stoi(eachWord); }
		}
		byteMap[lastByte.row][lastByte.column] = '#';
	}

	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << lastByte.column << "," << lastByte.row;
}
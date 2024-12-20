#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_map>

struct Point
{
	int32_t row{};
	int32_t column{};

	Point operator+(const Point& otherPoint) const { return Point{ row + otherPoint.row, column + otherPoint.column }; }
	Point operator-(const Point& otherPoint) const { return Point{ row - otherPoint.row, column - otherPoint.column }; }
};

int main()
{
	std::ifstream myReadFile("day08_input.txt");
	std::string inputLine{};

	int64_t part1{};
	int64_t part2{};

	std::vector<std::string> inputMap{};
	std::unordered_map<char, std::vector<Point>> allBeacons{};
	while (std::getline(myReadFile, inputLine))			//search all beacons and store all its positions classified by letter
	{
		for (int i{}; i < inputLine.size(); i++)
		{
			if (inputLine[i] != '.') { allBeacons[inputLine[i]].emplace_back(Point{ static_cast<int>(inputMap.size()), i }); }
		}
		inputMap.emplace_back(std::string(inputLine.size(), '.'));
	}

	std::vector<std::string> part2Map = inputMap;		//get a copy of the map for part two
	for (auto& eachType : allBeacons)					//search for all beacon combinations and calculate its antinodes
	{
		for (int i{}; i < eachType.second.size(); i++)
		{
			for (int j{}; j < eachType.second.size(); j++)
			{
				if (i == j) { continue; }
				Point distanceVector = eachType.second[i] - eachType.second[j];		//calculate the distance vector from the two nodes
				Point antiNode = eachType.second[i] + distanceVector;				//set antinode
				if (antiNode.row < 0 || antiNode.column < 0 || antiNode.row >= inputMap.size() || antiNode.column >= inputMap[0].size()) { continue; }
				if (inputMap[antiNode.row][antiNode.column] == '.') { part1++; inputMap[antiNode.row][antiNode.column] = '#'; } //add 1 count if anti node is in the map and there was not another antiNode
			}
		}
	}
	//repeat part 1 but keep on adding distance vector until the anti node position goes out of the map, the map is not that big to require formulas
	for (auto& eachType : allBeacons)					
	{
		for (int i{}; i < eachType.second.size(); i++)
		{
			for (int j{}; j < eachType.second.size(); j++)
			{
				if (i == j) { continue; }
				Point distanceVector = eachType.second[i] - eachType.second[j];
				Point antiNode = eachType.second[i];
				while (antiNode.row >= 0 && antiNode.column >= 0 && antiNode.row < part2Map.size() && antiNode.column < part2Map[0].size())
				{
					if (part2Map[antiNode.row][antiNode.column] == '.') { part2++; part2Map[antiNode.row][antiNode.column] = '#'; }
					antiNode = antiNode + distanceVector;
				}
			}
		}
	}

	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
	return 0;
}
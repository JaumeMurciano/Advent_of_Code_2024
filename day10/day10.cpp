#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>

void searchPath(std::vector<std::string>& inputMap, int row, int column, int currentNum, std::set<std::pair<int, int>>& visitedEnds, int& part2)
{
	if (row < 0 || column < 0 || row >= inputMap.size() || column >= inputMap[0].size()) { return; }	//discard going out of the map
	if (inputMap[row][column] - '0' != currentNum) { return; }											//discard if current number does not match expected number
	if (currentNum == 9) { visitedEnds.emplace(std::pair<int, int>{row, column}); part2++; return; }	//if ended, add end to list and return
	currentNum++;
	searchPath(inputMap, row + 1, column, currentNum, visitedEnds, part2);								//try all neighbours
	searchPath(inputMap, row - 1, column, currentNum, visitedEnds, part2);
	searchPath(inputMap, row, column + 1, currentNum, visitedEnds, part2);
	searchPath(inputMap, row, column - 1, currentNum, visitedEnds, part2);
}

int main()
{
	std::ifstream myReadFile("day10_input.txt");
	std::string inputLine{};

	int part1{};
	int part2{};
	std::vector<std::string> inputMap{};

	while (std::getline(myReadFile, inputLine))
	{
		inputMap.emplace_back(inputLine);		//store input number map
	}

	for (int row{}; row < inputMap.size(); row++)
	{
		for (int column{}; column < inputMap[row].size(); column++)
		{
			std::set<std::pair<int, int>> visitedEnds{};				//used to store all the possible ends of the path
			searchPath(inputMap, row, column, 0, visitedEnds, part2);	//try to search a path from each number, non-0 starts will be discarded in the function
			part1 += visitedEnds.size();
		}
	}

	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
	return 0;
}
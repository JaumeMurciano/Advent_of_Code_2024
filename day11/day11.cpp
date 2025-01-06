#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_map>

int main()
{
	std::ifstream myReadFile("day11_input.txt");
	std::string inputLine{};

	int64_t part1{};
	int64_t part2{};
	std::unordered_map<uint64_t, uint64_t> stoneMap{}; //each pos is first = value, second = ntimes, done bc as binks rise number of rocks rises exponentially but lots of numbers are repeated

	while (std::getline(myReadFile, inputLine))
	{
		std::stringstream line(inputLine);
		std::string eachWord{};
		while (getline(line, eachWord, ' '))
		{
			stoneMap[stoull(eachWord)] = 1;				//add one count of each input rock. Assume the rock numbers are unique
		}
	}
	int32_t nBlinks{75};
	int32_t part1Blinks = nBlinks - 25;					//to keep track of part 1 result

	while (nBlinks > 0)
	{
		std::unordered_map<uint64_t, uint64_t> newStoneMap{};
		if (nBlinks == part1Blinks)						//if 25 blinks have been processed, store the number of rocks for part 1
		{
			for (auto eachStone : stoneMap) {
				part1 += eachStone.second;
			}
		}
		for (auto eachStone : stoneMap)					//process one blink at each rock for each type of rock that is in the input. Add the number of rocks that are already in that state at the result
		{
			if (eachStone.first == 0) { newStoneMap[1] += eachStone.second; }
			else if (std::to_string(eachStone.first).size() % 2 == 0) {
				std::string numStr = std::to_string(eachStone.first);
				uint64_t num1 = std::stoull(numStr.substr(0, numStr.size() / 2));
				uint64_t num2 = std::stoull(numStr.substr((numStr.size() / 2), numStr.size() / 2));
				newStoneMap[num1] += eachStone.second;
				newStoneMap[num2] += eachStone.second;
			}
			else { newStoneMap[eachStone.first * 2024] += eachStone.second; }
		}
		stoneMap = newStoneMap;
		nBlinks--;
	}

	for (auto eachStone : stoneMap)						//when all the blinks have been processed, compute part 2 result
	{
		part2 += eachStone.second;
	}
	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
	return 0;
}
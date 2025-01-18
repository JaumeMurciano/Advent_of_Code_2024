#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>

std::vector<int32_t> parseKey(std::vector<std::string> inputKey)	//this function iterates over each key column by column and stores the height of each column
{
	std::vector<int32_t> result = std::vector<int32_t>(inputKey[0].size(), 0);
	for (int32_t column{}; column < inputKey[0].size(); column++)
	{
		int32_t lengthCounter{};
		for (int32_t row{ static_cast<int32_t>(inputKey.size()) -1}; row >= 0; row--)
		{
			if (inputKey[row][column] != '#') { break; }
			lengthCounter++;
		}
		result[column] = lengthCounter - 1;
	}
	return result;
}

std::vector<int32_t> parseLock(std::vector<std::string> inputLock)	//this function iterates over each lock column by column and stores the height of each column
{
	std::vector<int32_t> result = std::vector<int32_t>(inputLock[0].size(), 0);
	for (int32_t column{}; column < inputLock[0].size(); column++)
	{
		int32_t lengthCounter{};
		for (int32_t row{}; row < inputLock.size(); row++)
		{
			if (inputLock[row][column] != '#') { break; }
			lengthCounter++;
		}
		result[column] = lengthCounter - 1;
	}
	return result;
}

int main()
{
	std::ifstream myReadFile("day25_input.txt");
	std::string inputLine{};

	int64_t part1{};
	std::vector<std::string> currentPiece{};
	std::vector<std::vector<int>> allLocks{};		//used to store all locks, each lock is represented with the number of # it has below the first line
	std::vector<std::vector<int>> allKeys{};		//used to store all keys, each key is represented with the number of # they have below the first line

	while (std::getline(myReadFile, inputLine))		//parse all the locks and keys
	{
		if (inputLine != "") { currentPiece.emplace_back(inputLine); }
		else
		{
			if (currentPiece[0][0] == '#') { allLocks.emplace_back(parseLock(currentPiece)); }
			else { allKeys.emplace_back(parseKey(currentPiece)); }
			currentPiece = std::vector<std::string>{};
		}
	}
	if (currentPiece[0][0] == '#') { allLocks.emplace_back(parseLock(currentPiece)); }
	else { allKeys.emplace_back(parseKey(currentPiece)); } //store the last key/lock
	int32_t maxHeight = static_cast<int>(currentPiece.size())-2;

	for (const std::vector<int32_t>& eachLock : allLocks)	//try all the possible combinations of all keys and locks
	{
		for (const std::vector<int32_t>& eachKey : allKeys)
		{
			bool matches{true};
			for (std::size_t i{}; i < eachKey.size(); i++)
			{
				if (eachKey[i] + eachLock[i] > maxHeight) { matches = false; break; }	//if the key+lock exceeds the maximum height, the combination does not match
			}
			if (matches) { part1++; }
		}
	}
	std::cout << "Part 1 result is: " << part1 << "\n";
	//std::cout << "Part 2 result is: " << part2;
}
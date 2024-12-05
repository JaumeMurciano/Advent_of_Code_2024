#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_map>

int orderInvalidSequence(std::vector<int>& alreadySeen, std::unordered_map<int32_t, std::vector<int32_t>>& invalidPairs)
{
	bool finished{};
	for (int i{}; i < alreadySeen.size(); i++)
	{
		for (int j{}; j < i; j++)
		{
			if (std::find(invalidPairs[alreadySeen[i]].begin(), invalidPairs[alreadySeen[i]].end(), alreadySeen[j]) != invalidPairs[alreadySeen[i]].end())
			{
				//if sequence is invalid, swap the numbers and re-check if the swap has created any invalid sequence
				int tempNum{alreadySeen[i]};
				alreadySeen[i] = alreadySeen[j];
				alreadySeen[j] = tempNum;
				i = j;
			}
		}
	}
	return alreadySeen[alreadySeen.size() / 2];
}

int main()
{
	std::ifstream myReadFile("day05_input.txt");
	std::string inputLine{};
	
	std::unordered_map<int32_t,std::vector<int32_t>> invalidPairs{}; //we only care if the pair is invalid
	bool fileSecondPart{false};					//bool used to store if the second part of the file is being parsed

	int part1{};								//integer used to store part1 result
	int part2{};								//integer used to store part2 result

	while (std::getline(myReadFile, inputLine))
	{
		std::stringstream line(inputLine);
		std::string eachWord{};

		if (inputLine == "") { fileSecondPart = true; continue; }	//flag to start parsing part2

		if (!fileSecondPart)
		{
			int num1{};
			int num2{};
			while (getline(line, eachWord, '|'))
			{
				if (num1 == 0) { num1 = std::stoi(eachWord); }
				else { num2 = std::stoi(eachWord); }
			}
			invalidPairs[num2].emplace_back(num1);					//we really only care if we see a pair and it is INVALID
		}
		if (fileSecondPart)
		{
			std::vector<int> alreadySeen{};
			bool invalidSequence{};
			while (getline(line, eachWord, ','))
			{
				int currentNumber = std::stoi(eachWord);
				for (int eachSeen : alreadySeen)
				{
					//std::find searches if the current number has any invalid pair before it. If it does, the sequence is invalid
					if (std::find(invalidPairs[eachSeen].begin(), invalidPairs[eachSeen].end(), currentNumber) != invalidPairs[eachSeen].end()) { invalidSequence = true; }
				}
				alreadySeen.emplace_back(currentNumber);			//used to store already seen numbers
			}
			if (!invalidSequence) { part1 += alreadySeen[alreadySeen.size()/2]; }	//if sequence is valid, add to part1
			if (invalidSequence) { part2 += orderInvalidSequence(alreadySeen, invalidPairs); } //if sequence is invalid,fix it and add to part2
		}
	}

	std::cout << "Part 1 solution is: " << part1 << "\n";
	std::cout << "Part 2 solution is: " << part2;

	return 0;
}
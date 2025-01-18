#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <deque>
#include <map>

//function used to calculate the next secret number from a secret number
uint64_t calculateSecretNumber(const int64_t secretNumber)
{
	int64_t newSecret = ((secretNumber * 64) ^ secretNumber) % 16777216;
	newSecret = ((newSecret / 32) ^ newSecret) % 16777216;
	newSecret = ((newSecret * 2048)^newSecret) % 16777216;
	return newSecret;
}
int main()
{
	std::ifstream myReadFile("day22_input.txt");
	std::string inputLine{};

	int64_t part1{};
	int64_t part2{};
	int32_t nIterations{2000};
	std::map<std::deque<int32_t>, int32_t> bestSequences{};
	while (std::getline(myReadFile, inputLine))
	{
		uint64_t currentNumber = std::stoull(inputLine);
		int64_t lastNumber = currentNumber%10;	//to calculate the differences between the last number and the current secret number
		std::map<std::deque<int32_t>, int32_t> bestLocalSequences{};	//deque is the last four differences sequence
		std::deque<int32_t> lastChanges{};								//last four differences sequence
		for (int i{}; i < nIterations; i++)
		{
			currentNumber = calculateSecretNumber(currentNumber);		//calculate the next secret number
			lastChanges.emplace_back((static_cast<int64_t>(currentNumber) % 10) - lastNumber);
			lastNumber = currentNumber % 10;
			if (lastChanges.size() == 5) { lastChanges.pop_front(); }	//move the moving window
			if (lastChanges.size() == 4)
			{
				if (bestLocalSequences.find(lastChanges) == bestLocalSequences.end())	//if sequence has not been found in that seller, store the bananas that will be earned
				{
					bestLocalSequences[lastChanges] = currentNumber % 10;
				}
			}
		}
		for (auto eachSequence : bestLocalSequences)					//store the bananas earned with each sequence
		{
			bestSequences[eachSequence.first] += eachSequence.second;
		}
		part1 += currentNumber;
	}

	for (auto eachSequence : bestSequences)								//search for the highest result sequence
	{
		if (part2 == 0 || eachSequence.second > part2)
		{
			part2 = eachSequence.second;
		}
	}
	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
}
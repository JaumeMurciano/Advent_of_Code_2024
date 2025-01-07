#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>
#include <unordered_map>

struct letterTree											//all the possible string combinations will be stored here as a tree
{
	std::unordered_map<char, letterTree*> availableConnections{};
	bool endWord{};											//boolean to store if this is the end of a word
};

// Function used to add the possibilities to go to other positions from current letter
void searchCombination(letterTree& originalTree, int currentLetter, std::string combination, std::vector<int64_t>& combinationCache)
{
	letterTree* currentTree = &originalTree;
	int originalLetter{ currentLetter };					//store the original letter position to add cumulative possibilities
	while (currentTree != nullptr)
	{
		currentTree = currentTree->availableConnections[combination[currentLetter]];	//search for an availalbe letter combination available
		if (currentTree == nullptr) { break; }				//If letter is not available, stop searching
		currentLetter++;									//if it is available go to next letter
		if (currentTree->endWord == true) { combinationCache[currentLetter] += combinationCache[originalLetter]; } //And add cumulative combinations to cache
	}
}

int main()
{
	std::ifstream myReadFile("day19_input.txt");
	std::string inputLine{};

	letterTree possibleCombinations{};						//possible letter combination tree
	int64_t part1{};
	int64_t part2{};

	bool secondPart{ false };
	while (std::getline(myReadFile, inputLine))
	{
		if (inputLine == "") {
			secondPart = true; continue;
		}
		if (!secondPart)
		{
			std::stringstream line(inputLine);
			std::string eachWord{};

			while (getline(line, eachWord, ' '))			//at part 1, create a pointer to the tip of the tree and add the new combination to it
			{
				letterTree* currentRun = &possibleCombinations;
				if (eachWord[eachWord.size() - 1] == ',') { eachWord.erase(eachWord.size() - 1); }
				for (char eachLetter : eachWord) {
					if (currentRun->availableConnections[eachLetter] == nullptr) { currentRun->availableConnections[eachLetter] = new letterTree(); }
					currentRun = currentRun->availableConnections[eachLetter];
				}
				currentRun->endWord = true;					//set the end of the word as a possible word end
			}
		}
		else
		{
			std::vector<int64_t> caseCache = std::vector<int64_t>(inputLine.size() + 1, 0);	//initialize case cache
			caseCache[0] = 1;								//the initial position does not represent any letter, just the beginning, and starts with one possibility
			for (int i{}; i < caseCache.size() - 1; i++)
			{
				if (caseCache[i] > 0) { searchCombination(possibleCombinations, i, inputLine, caseCache); }	//add the cumulative options for each case in the cache
			}
			part1 += caseCache[caseCache.size() - 1] > 0;	//just add and count if it is possible to arrive at the end
			part2 += caseCache[caseCache.size() - 1];			//add cumulative possibilities of arriving at the end
		}

	}

	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
}
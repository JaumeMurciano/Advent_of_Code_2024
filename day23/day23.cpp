#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>
#include <map>
//Disclaimer: lazy approach. I could have stored which nodes have already been visited and not neet to have a set to store the tries on part 1 and not repeat
//on part two the nodes that have been already been checked, which would make both problems  O(n)
int main()
{
	std::ifstream myReadFile("day23_input.txt");
	std::string inputLine{};

	int64_t part1{};
	std::map<std::string, std::vector<std::string>> allNodes{};	//graph the first is the name of the node and the second its detinations
	std::set<std::set<std::string>> part1Tries{};		//set to store the unique node tries (the inner set will be the ordered tries and the outer set all the tries)
	std::set<std::string> largestTrie{};				//set to store the largest trie for part 2
	
	while (std::getline(myReadFile, inputLine))			//build the grapth
	{
		std::stringstream line(inputLine);
		std::string eachWord{};
		std::array<std::string, 2> twoComps{};
		while (getline(line, eachWord, '-'))
		{
			if (twoComps[0] == "") { twoComps[0] = eachWord; }
			else { twoComps[1] = eachWord; }
		}
		allNodes[twoComps[0]].emplace_back(twoComps[1]);	//add the non-directed edges to the graph
		allNodes[twoComps[1]].emplace_back(twoComps[0]);
	}

	for (auto firstComputer : allNodes)
	{
		for (int32_t i{}; i < firstComputer.second.size();i++)
		{
			std::set<std::string> currentGroup{ firstComputer.first,firstComputer.second[i] };
			for (int32_t j{ i + 1 }; j < firstComputer.second.size(); j++)
			{
				//for each pair, search each connection of the second node that is in the first
				if (std::find(allNodes[firstComputer.second[i]].begin(), allNodes[firstComputer.second[i]].end(), firstComputer.second[j]) != allNodes[firstComputer.second[i]].end())
				{	//check that at least one starts with t
					if (firstComputer.first[0] == 't' || firstComputer.second[i][0] == 't' || firstComputer.second[j][0] == 't')
					{	//if complies, add in the set for part 1
						part1Tries.emplace(std::set<std::string>{firstComputer.first,firstComputer.second[i],firstComputer.second[j]});
					}
				}
				//part 2
				bool addConnection{true};
				for (std::string eachConnectionInSet : currentGroup)
				{	//search if the current third node is connected to all nodes that are already in the set
					if (std::find(allNodes[firstComputer.second[j]].begin(), allNodes[firstComputer.second[j]].end(), eachConnectionInSet) == allNodes[firstComputer.second[j]].end())
					{
						addConnection = false;
						break;
					}
				}
				//if fully connected, add to the set
				if (addConnection) { currentGroup.emplace(firstComputer.second[j]); }
			}
			//store the biggest set size.
			if (currentGroup.size() > largestTrie.size()) { largestTrie = currentGroup; }
		}
	}

	std::cout << "Part 1 result is: " << part1Tries.size() << "\n";
	for (std::string eachString : largestTrie)
	{
		std::cout << eachString << ",";
	}
}
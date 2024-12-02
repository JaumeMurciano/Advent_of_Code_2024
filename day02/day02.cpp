#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

bool checkPart2(std::vector<int> numsInLine, int numToSkip)
{
	if(numToSkip != -1){ numsInLine.erase(numsInLine.begin() + numToSkip); }
	bool increasing{numsInLine[1] > numsInLine[0]};

	for (int i{1}; i < numsInLine.size(); i++)
	{
		if (increasing) {
			if (abs(numsInLine[i] - numsInLine[i - 1]) > 3 || numsInLine[i] <= numsInLine[i - 1])
			{
				return false;
			}
		}
		else
		{
			if (abs(numsInLine[i] - numsInLine[i - 1]) > 3 || numsInLine[i] >= numsInLine[i - 1])
			{
				return false;
			}
		}
	}

	return true;

}

int main()
{
	std::ifstream myReadFile{ "day02_input.txt" };
	std::string inputLine{};

	int part1{};
	int part2{};

	while (getline(myReadFile, inputLine))
	{
		std::stringstream line(inputLine);
		std::string eachNumber{};

		std::vector<int> numsInLine{};
		while (getline(line, eachNumber, ' '))
		{
			numsInLine.emplace_back(std::stoi(eachNumber));
		}

		if (checkPart2(numsInLine, -1))
		{
			part1++;
			part2++;
		}
		else
		{
			for (int i{}; i < numsInLine.size(); i++)
			{
				if (checkPart2(numsInLine, i)) { part2++; break; }
			}
		}
	}

	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
}
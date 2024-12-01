#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

int main()
{
	std::ifstream myReadFile{ "day01_input.txt" };
	std::string inputLine{};

	std::vector<int> leftColumn{}; //We will store all the numbers from the left column in this vector
	std::vector<int> rightColumn{}; //We will store all the numbers from the right column in this vector

	std::array<int, 100000> rightFrequency{}; //for part2 we have to count the frequency of appearance of each number in the right column

	int part1{};
	int part2{};

	// for each line, we store the right and left column numbers and count how many times that right number has appeared
	while (getline(myReadFile, inputLine))
	{
		std::stringstream line(inputLine);
		std::string eachNumber{};

		bool inLeftRow{ true };
		while (getline(line, eachNumber, ' '))
		{
			if (eachNumber == "") { continue; }
			if (inLeftRow) { leftColumn.emplace_back(std::stoi(eachNumber)); }
			else { rightColumn.emplace_back(std::stoi(eachNumber)); rightFrequency[std::stoi(eachNumber)]++; }
			inLeftRow = false;
		}
	}
	
	//we sort both vectors to calculate part1
	std::sort(leftColumn.begin(), leftColumn.end());
	std::sort(rightColumn.begin(), rightColumn.end());

	for (int i{}; i < leftColumn.size(); i++)
	{
		part1 += abs(rightColumn[i] - leftColumn[i]); //part1 subtract all right and left ordered columns
		part2 += leftColumn[i] * rightFrequency[leftColumn[i]]; //part2 multiply each left column by its frequency on the right column
	}

	std::cout << "Part 1 answer is: " << part1 << "\n";
	std::cout << "Part 2 answer is: " << part2 << "\n";
	return 0;
}
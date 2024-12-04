#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <regex>


struct Coordinate				//Used to store coordinates/movements
{
	int32_t i{};
	int32_t j{};
};

void xmasPart1Search(std::vector<std::string>& xmas, int i, int j, int& xmasFound)
{
	std::string xmasWord{ "XMAS" }; //used to search for XMAS word
	if (i < 0 || i >= xmas.size() || j < 0 || j >= xmas.size()) { return; } //letter position is invalid
	if (xmas[i][j] != 'X') { return; } //word has to start with W

	for (int dirx{ -1 }; dirx <= 1; dirx++)		//x vector component
	{
		for (int diry{ -1 }; diry <= 1; diry++) // y vector component
		{
			Coordinate currentPos{ i,j };		//save current position
			for (int currentLetter{}; currentLetter < 4; currentLetter++)
			{
				if (currentPos.i < 0 || currentPos.i >= xmas.size() || currentPos.j < 0 || currentPos.j >= xmas[0].size()) { break; } //movement goes out of bounds
				if (xmas[currentPos.i][currentPos.j] != xmasWord[currentLetter]) { break; } //if letter does not match XMAS sequence, break
				currentPos.i += dirx;			//continue moving
				currentPos.j += diry;			//continue moving
				if (currentLetter == 3) { xmasFound++; } //if we have arrived to the end, add a XMAS found
			}
		}
	}
}

void masSearch(std::vector<std::string>& xmas, int i, int j, int& xmasFound, std::string& xmasWord)
{
	if (i < 0 || i >= xmas.size() || j < 0 || j >= xmas.size()) { return; } //letter position is invalid
	if (xmas[i][j] != 'A') { return; }


}

int main()
{
	std::ifstream myReadFile("day04_input.txt");
	std::string inputLine{};

	int part1{};								//Used to store part1 result
	int part2{};								//Used to store part2 result

	std::vector<std::string> xmas{};			//Used to store the user input

	while (std::getline(myReadFile, inputLine))
	{
		xmas.emplace_back(inputLine);
	}

	for (int i{}; i < xmas.size(); i++)
	{
		for (int j{}; j < xmas[0].size(); j++)
		{
			xmasPart1Search(xmas, i, j, part1); //search if there is any xmas at each cell
			if (xmas[i][j] == 'A') //part2: if there is an A, check if there is are any MAS/SAM in the four diagonals
			{
				if (i == 0 || j == 0 || i == xmas.size() - 1 || j == xmas[0].size() - 1) { continue; } //check not to go out of bounds
				int masCount{};
				if ((xmas[i - 1][j - 1] == 'M' && xmas[i + 1][j + 1] == 'S') || (xmas[i + 1][j + 1] == 'M' && xmas[i - 1][j - 1] == 'S')) { masCount++; }
				if ((xmas[i + 1][j - 1] == 'M' && xmas[i - 1][j + 1] == 'S') || (xmas[i - 1][j + 1] == 'M' && xmas[i + 1][j - 1] == 'S')) { masCount++; }
				if (masCount == 2) { part2++; } //if there is a MAS/SAM in the two diagonals, 1 X-MAS is found 
			}
		}
	}

	std::cout << "Part 1 solution is: " << part1 << "\n";
	std::cout << "Part 2 solution is: " << part2 << "\n";
	return 0;
}
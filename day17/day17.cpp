#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>

//function that processes a number in accordance with advent instructions
int32_t processInstructions(std::vector<int32_t> allInstructions, std::array<uint64_t, 8> comboOperands, bool part2)
{
	std::size_t currentInstruction{};
	int debug1 = comboOperands[4];
	for (int32_t i{}; i < allInstructions.size() - 1; i += 2) //i = instruction pointer
	{
		if (allInstructions[i] == 0) //num = A, div = 2^combo
		{
			comboOperands[4] = comboOperands[4] / static_cast<uint64_t>(pow(2, comboOperands[allInstructions[i + 1]]));
		}
		else if (allInstructions[i] == 1)
		{
			comboOperands[5] = comboOperands[5] ^ allInstructions[i + 1];
		}
		else if (allInstructions[i] == 2)
		{
			comboOperands[5] = comboOperands[allInstructions[i + 1]] % 8;
		}
		else if (allInstructions[i] == 3)
		{
			if (comboOperands[4] == 0) { continue; }
			else {
				i = allInstructions[i + 1];
				i -= 2; //to prevent adding 2
			};
		}
		else if (allInstructions[i] == 4)
		{
			comboOperands[5] = comboOperands[5] ^ comboOperands[6];
		}
		else if (allInstructions[i] == 5)
		{
			if (!part2) { std::cout << comboOperands[allInstructions[i + 1]] % 8; if (comboOperands[4] > 0) { std::cout << ","; } continue;}
			else { return comboOperands[allInstructions[i + 1]] % 8; } //for part two, we only need to see if the current number matches the searched number
		}
		else if (allInstructions[i] == 6)
		{
			comboOperands[5] = comboOperands[4] / static_cast<uint64_t>(pow(2, comboOperands[allInstructions[i + 1]]));
		}
		else if (allInstructions[i] == 7)
		{
			comboOperands[6] = comboOperands[4] / static_cast<uint64_t>(pow(2, comboOperands[allInstructions[i + 1]]));
		}
	}
	return currentInstruction;
}

uint64_t findBitCombinations(const std::vector<int>& allInstructions, uint64_t currentNum, int32_t currentInstruction)
{
	std::array<uint64_t, 8> comboOperands{ 0,1,2,3,0,0,0,7 };
	for (int i{}; i < 8; i++)
	{
		comboOperands[4] = currentNum + i;							//try all the combinations of the last three bits
		if (processInstructions(allInstructions, comboOperands, true) == allInstructions[currentInstruction])
		{
			if (currentInstruction == 0) { return currentNum + i; }	//if the current instruction is the last, this is the first correct number
			uint64_t currentResult = findBitCombinations(allInstructions, (currentNum + i) * 8, currentInstruction - 1);	//if the last three bits create the desired sequence number, search for the next sequence number at the next three bits
			if (currentResult != 0) { return currentResult; }		//used to return the correct result recursively
		}
	}
	return 0;
}

int main()
{
	std::ifstream myReadFile("day17_input.txt");
	std::string inputLine{};

	uint64_t part2{};

	std::array<uint64_t, 8> comboOperands{ 0,1,2,3,4,5,6,7 };
	std::vector<int32_t> allInstructions{};
	std::size_t parseCombos{ 4 };

	bool secondPart{ false };

	while (std::getline(myReadFile, inputLine))	//parse the input instructions and store the initial value of the combo operands
	{
		std::stringstream line(inputLine);
		std::string eachWord{};
		if (inputLine == "") { secondPart = true; continue; }
		if (!secondPart)
		{
			getline(line, eachWord, ' ');
			getline(line, eachWord, ' ');
			getline(line, eachWord, ' ');
			comboOperands[parseCombos] = std::stoll(eachWord);
			parseCombos++;
		}
		else
		{
			std::stringstream line(inputLine);
			std::string eachWord{};
			getline(line, eachWord, ' ');
			while (getline(line, eachWord, ','))
			{
				allInstructions.emplace_back(std::stoi(eachWord));
			}
		}
	}
	std::cout << "Part 1 result is: ";
	processInstructions(allInstructions, comboOperands, false);		//process the instructions with the input combo operands for part 1
	std::cout << "\n";

	//part 2 consists on building the number backwards. The thing is that the machine operates over the lowest three bits and some bits on its left and outputs a number
	//so, we have to search which three bits create the last number and try to create the sequence backwards.
	part2 = findBitCombinations(allInstructions,0,allInstructions.size()-1);
	std::cout << "Part 2 result is: " << part2;
}
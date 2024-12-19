#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>
#include <unordered_map>

bool tryOperators(uint64_t targetNumber, std::vector<uint64_t>& allOperands, std::size_t currentOperand, uint64_t currentResult, bool part2)
{
	if (currentResult > targetNumber) { return false; }				//discard path if current result is greater than final result, each operand only increases the number
	if (currentOperand == allOperands.size())						//if the last operand has been consumed, check if the result is the target number or not
	{
		if (currentResult == targetNumber) { return true; }
		else { return false; }
	}
	//if there are operands available and the number is smaller, keep on trying to use all the operands
	if (tryOperators(targetNumber, allOperands, currentOperand + 1, currentResult * allOperands[currentOperand], part2)) { return true; }
	if (tryOperators(targetNumber, allOperands, currentOperand + 1, currentResult + allOperands[currentOperand], part2)) { return true; }
	if (part2)
	{
		std::string concatNumber = std::to_string(currentResult) + std::to_string(allOperands[currentOperand]);	//concatenate the two numbers
		std::string strTargetNumber = std::to_string(std::numeric_limits<uint64_t>::max());
		if (concatNumber.size() > strTargetNumber.size()) { return false; }		//check not to run into undefined behaviour (number should not be THAT big, just in case)
		if (tryOperators(targetNumber, allOperands, currentOperand + 1, std::stoull(concatNumber), part2)) { return true; }
	}
	return false;													//if none of the operators work, return false
}

int main()
{
	std::ifstream myReadFile("day07_input.txt");
	std::string inputLine{};
	uint64_t part1{};
	uint64_t part2{};

	while (std::getline(myReadFile, inputLine))
	{	//for each line parse the line and add the result to "targetNumber" and the operands to "allOperands"
		uint64_t targetNumber{};
		std::vector<uint64_t> allOperands{};

		std::stringstream line(inputLine);
		std::string eachWord{};
		getline(line, eachWord, ':');
		targetNumber = std::stoull(eachWord);
		getline(line, eachWord, ' ');
		while (getline(line, eachWord, ' '))
		{
			allOperands.emplace_back(std::stoull(eachWord));
		}
		//first, try only with + and * operators, and if it is not possible, try to concatenate
		if (tryOperators(targetNumber, allOperands, 1, allOperands[0], false)) { part1 += targetNumber; part2 += targetNumber; }
		else if (tryOperators(targetNumber, allOperands, 1, allOperands[0], true)) { part2 += targetNumber; }
	}

	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
}
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include <unordered_map>

//DISCLAIMER! This code only works for my input! Part 2 implemented here only helps to guess which cables are correct, but may not work for other inputs.
struct Operation 
{
	std::array<std::string,2> operands{};
	std::string result{};
	std::string operation{};
	bool contains(const std::string operand) { return operands[0] == operand || operands[1] == operand; }
	bool operator==(const Operation& otherOperation) const { return operands[0] == otherOperation.operands[0] && operands[0] == otherOperation.operands[0] && result == otherOperation.result && operation == otherOperation.operation; }
};

int64_t runOperations(std::map<std::string, int64_t> allNumbers, std::vector<std::array<std::string, 5>> allOperations)
{
	int64_t result{};
	std::vector<bool> operationsEnded = std::vector<bool>(allOperations.size(), false);	//store which operations have been already calculated

	while (true)
	{
		bool calcEnded{ true };
		std::size_t numbersSize{ allNumbers.size() };
		for (std::size_t i{}; i < allOperations.size(); i++)
		{
			//search if the two inputs have been already received
			if (allNumbers.find(allOperations[i][0]) == allNumbers.end() || allNumbers.find(allOperations[i][2]) == allNumbers.end())
			{
				calcEnded = false;
				continue;
			}
			//if already received, calculate the bit result.
			if (allOperations[i][1] == "AND") { allNumbers[allOperations[i][4]] = allNumbers[allOperations[i][0]] & allNumbers[allOperations[i][2]]; }
			else if (allOperations[i][1] == "OR") { allNumbers[allOperations[i][4]] = allNumbers[allOperations[i][0]] | allNumbers[allOperations[i][2]]; }
			else if (allOperations[i][1] == "XOR") { allNumbers[allOperations[i][4]] = allNumbers[allOperations[i][0]] ^ allNumbers[allOperations[i][2]]; }
		}
		if (calcEnded) { break; }
		if (numbersSize == allNumbers.size()) { return 0; }
	}

	//calculate part 1 result
	for (int i{}; i < 50; i++)
	{
		std::string currentBit;
		if (i < 10) { currentBit = "z0" + std::to_string(i); }
		else { currentBit = "z" + std::to_string(i); }
		result += pow(2, i) * allNumbers[currentBit];
	}
	return result;
}
//function to search an operation searching for the result
Operation findOperationByResult(const std::vector<Operation>& allOperations, const std::string& resultSearched)
{
	for (const Operation& eachOperation : allOperations)
	{
		if (eachOperation.result == resultSearched) { return eachOperation; }
	}
	return{};
}
//functio to search an operation with an operand and an operator
Operation findOperationByOperand(const std::vector<Operation>& allOperations, const std::string& operand, const std::string& operatorSearched)
{
	for (const Operation& eachOperation : allOperations)
	{
		if ((operand == eachOperation.operands[0] || operand == eachOperation.operands[1]) && operatorSearched == eachOperation.operation) { return eachOperation; }
	}
	return {};
}


int main()
{
	std::ifstream myReadFile("day24_input.txt");
	std::string inputLine{};

	uint64_t part1{};
	std::map<std::string, int64_t> allNumbers{};		//all outputted numbers are here
	std::vector<std::array<std::string, 5>> allOperations{};
	std::vector<Operation> allStructOperations{};
	bool part2{ false };
	
	//step 1: parse the input
	while (std::getline(myReadFile, inputLine))
	{
		std::stringstream line(inputLine);
		std::string eachWord{};
		if (inputLine == "") { part2 = true; continue; }
		if (!part2)
		{
			std::string varName{};
			int64_t varValue{};
			while (getline(line, eachWord, ' '))
			{
				if (varName == "") { eachWord.erase(eachWord.size() - 1); varName = eachWord; }
				else { varValue = std::stoll(eachWord); }
			}
			allNumbers.emplace(varName, varValue);
		}
		//for part 2, store all operations in the allOperations vector.
		if (part2)
		{
			std::array<std::string, 5> eachOperation{};
			std::size_t i{};
			while (getline(line, eachWord, ' '))
			{
				eachOperation[i] = eachWord;
				i++;
			}
			allOperations.emplace_back(eachOperation);
			allStructOperations.emplace_back(Operation{ {eachOperation[0],eachOperation[2]},eachOperation[4],eachOperation[1] });
		}
	}
	part1 = runOperations(allNumbers, allOperations);

	//part 2 is a binary adder. See binary adder structure, but essentially you have two binary numbers to add and a carry
	//job here is to search which two output cables are switched and to store the correct carry
	std::string currentCarry = findOperationByOperand(allStructOperations,"x00","AND").result;
	int32_t currentBit = 1;
	std::set<std::string> cablesToSwap{};
	
	//result goes by trying to swap cables looking at the diagram
	while (true)
	{
		if (currentBit == 45)
		{
			break;
		}
		std::string currentX = currentBit < 10 ? "x0" + std::to_string(currentBit) : "x" + std::to_string(currentBit);
		std::string currentY = currentBit < 10 ? "y0" + std::to_string(currentBit) : "y" + std::to_string(currentBit);
		std::string currentZ = currentBit < 10 ? "z0" + std::to_string(currentBit) : "z" + std::to_string(currentBit);
		Operation bitResult = findOperationByResult(allStructOperations, currentZ);
		Operation bitXOR = findOperationByOperand(allStructOperations, currentX, "XOR");
		Operation bitAND = findOperationByOperand(allStructOperations, currentX, "AND");
		Operation newCarry = findOperationByOperand(allStructOperations, bitAND.result, "OR");
		Operation pseudoCarry = findOperationByOperand(allStructOperations, bitXOR.result, "AND");
		Operation pseudoCarry2 = findOperationByOperand(allStructOperations, currentCarry, "AND");
		Operation pseudoResult = findOperationByOperand(allStructOperations, bitXOR.result, "XOR");
		Operation pseudoResult2 = findOperationByOperand(allStructOperations, currentCarry, "XOR");
		Operation goodPseudoCarry = pseudoCarry;
		Operation newCarry2 = findOperationByOperand(allStructOperations, goodPseudoCarry.result, "OR");
		if (newCarry == Operation{} && pseudoCarry == Operation{})
		{
			//these two cables are swapped
			currentCarry = findOperationByOperand(allStructOperations, pseudoCarry2.result, "OR").result;
			cablesToSwap.emplace(bitXOR.result);
			cablesToSwap.emplace(bitAND.result);
			currentBit++;
			continue;
		}
		if (pseudoResult == pseudoResult2 && !(pseudoResult == bitResult)) { 
			cablesToSwap.emplace(currentZ);
			cablesToSwap.emplace(pseudoResult.result);
			currentBit++;
			currentCarry = newCarry2.result;
			continue;
		}
		else if (pseudoResult == bitResult && !(pseudoResult2 == bitResult)) { cablesToSwap.emplace(currentCarry); }
		else if (pseudoResult2 == bitResult && !(pseudoResult == bitResult)) { cablesToSwap.emplace(bitXOR.result); goodPseudoCarry = pseudoCarry2; newCarry2 = findOperationByOperand(allStructOperations, goodPseudoCarry.result, "OR");
		}
		else if (!(newCarry == newCarry2))
		{
			Operation searchError = findOperationByOperand(allStructOperations, newCarry.result, "XOR");
			std::string nextResult = currentBit + 1 < 10 ? "z0" + std::to_string(currentBit + 1) : "z" + std::to_string(currentBit + 1);
			if (searchError.result == nextResult) { cablesToSwap.emplace(newCarry2.result); currentCarry = newCarry.result; currentBit++; continue; }
			else { cablesToSwap.emplace(newCarry.result); currentCarry = newCarry2.result; currentBit++; continue; }
		}
		currentCarry = newCarry.result;
		currentBit++;
		
	}
	std::cout << "Part 1 result is: " << part1 << "\n";
	for (std::string eachCable : cablesToSwap)
	{	//don't know why this happens, has to be investigated further
		if (eachCable != "") { std::cout << eachCable << ","; }
	}
}	//WARNING! Part 2 result has 1 extra comma
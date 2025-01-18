#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>
#include <unordered_map>
#include <map>
struct Point
{
	int32_t row{};
	int32_t column{};
	Point operator+(const Point& otherPoint) const { return Point{ row + otherPoint.row, column + otherPoint.column }; }
	Point operator-(const Point& otherPoint) const { return  Point{ row - otherPoint.row, column - otherPoint.column }; }
};

std::string moveDirectionalRobot(const std::string pathToPrint)
{
	std::unordered_map<char, Point> directionalKeypad{ {{'<',{0,0}},{'v',{0,1}},{'>',{0,2}},{'X',{1,0}},{'^',{1,1}},{'A',{1,2}}} };
	std::string robot1Movements{};
	char robot1Position{ 'A' };
	//the logic here is the same, the priorities are <v^>, except for the cases that the robot goes out of the map.
	for (char eachChar : pathToPrint)
	{
		Point requiredMovement = directionalKeypad[eachChar] - directionalKeypad[robot1Position];
		if (requiredMovement.column < 0) {
			if (directionalKeypad[robot1Position].row == 1 && directionalKeypad[robot1Position].column + requiredMovement.column == 0) 
			{ 
				while (requiredMovement.row < 0) { robot1Movements.push_back('v'); requiredMovement.row++;}; 
			}
			while (requiredMovement.column < 0) { robot1Movements.push_back('<'); requiredMovement.column++; }
		};
		if (requiredMovement.row < 0) {
			while (requiredMovement.row < 0) { robot1Movements.push_back('v'); requiredMovement.row++; }
		};
		if (requiredMovement.row > 0) {
			if (directionalKeypad[robot1Position].column == 0 && directionalKeypad[robot1Position].row + requiredMovement.row == 1)
			{
				while (requiredMovement.column > 0) { robot1Movements.push_back('>'); requiredMovement.column--; }
			}
			while (requiredMovement.row > 0) { robot1Movements.push_back('^'); requiredMovement.row--; }
		};
		if (requiredMovement.column > 0) {
			while (requiredMovement.column > 0) { robot1Movements.push_back('>'); requiredMovement.column--; }
		};
		robot1Movements.push_back('A');
		robot1Position = eachChar;
	}
	return robot1Movements;
}

uint64_t calculateRobotLength(const std::string& pathToPrint, bool numericType, int32_t numDirectional)
{
	std::unordered_map<char, Point> numericKeypad{ {{'X',{0,0}},{'0',{0,1}},{'A',{0,2}},{'1',{1,0}},{'2',{1,1}},{'3',{1,2}},{'4',{2,0}},{'5',{2,1}},{'6',{2,2}},{'7',{3,0}},{'8',{3,1}},{'9',{3,2}}} };
	std::unordered_map<char, Point> directionalKeypad{ {{'<',{0,0}},{'v',{0,1}},{'>',{0,2}},{'X',{1,0}},{'^',{1,1}},{'A',{1,2}}} };

	std::string numericMovements{};
	char currentPosition{ 'A' };
	//firstly, generate the shortest path for the numeric keypad
	//we have to prioritize to go to the left, then downwards, then up and then right because we have to try to go all the furthest movements first
	//if we run into a hole, invert the movements, but ALWAYS move only to one direction and then to the other, do NOT combine
	for (char eachChar : pathToPrint)
	{
		Point requiredMovement = numericKeypad[eachChar] - numericKeypad[currentPosition];

		if (requiredMovement.column < 0) {
			if (numericKeypad[currentPosition].row == 0 && numericKeypad[currentPosition].column + requiredMovement.column == 0)	//to prevent going into the hole
			{
				while (requiredMovement.row > 0) { numericMovements.push_back('^'); requiredMovement.row--; }
			}
			while (requiredMovement.column < 0) { numericMovements.push_back('<'); requiredMovement.column++; }
		};
		if (requiredMovement.row < 0) {
			if (numericKeypad[currentPosition].column == 0 && numericKeypad[currentPosition].row + requiredMovement.row == 0)		//to prevent goint into the hole
			{
				while (requiredMovement.column > 0) { numericMovements.push_back('>'); requiredMovement.column--; }
			}
			while (requiredMovement.row < 0) { numericMovements.push_back('v'); requiredMovement.row++; }
		};
		if (requiredMovement.row > 0) {
			while (requiredMovement.row > 0) { numericMovements.push_back('^'); requiredMovement.row--; }
		};
		if (requiredMovement.column > 0) {
			while (requiredMovement.column > 0) { numericMovements.push_back('>'); requiredMovement.column--; }
		};

		numericMovements.push_back('A');
		currentPosition = eachChar;
	}
	std::map<std::string, int64_t> movementSequences{};
	std::string eachSequence{};
	//in order to cache the results, store the number of time a sequence repeats, and each sequence will move and press a button, 
	//as the next robot will have to go to that tile, press the button and make it return to the A button
	for (int i{}; i < numericMovements.size(); i++)
	{
		eachSequence.push_back(numericMovements[i]);
		if (numericMovements[i] == 'A') { movementSequences[eachSequence]++; eachSequence = {}; }

	}
	//now, repeat the movement generation pattern and caging but with the directional robots
	for (int i{}; i < numDirectional; i++)
	{
		std::map<std::string, int64_t> newMovementSequences{};
		for (const auto& eachMovementSequence : movementSequences)
		{
			std::string newMovements = moveDirectionalRobot(eachMovementSequence.first);
			for (int j{}; j < newMovements.size(); j++)
			{
				eachSequence.push_back(newMovements[j]);
				if (newMovements[j] == 'A') { newMovementSequences[eachSequence] += eachMovementSequence.second; eachSequence = {}; }
			}
		}
		movementSequences = newMovementSequences;
	}
	//and calculate the result
	int64_t result{};
	for (const auto& eachPair : movementSequences)
	{
		result += eachPair.first.size() * eachPair.second;
	}
	return result;
}

int main()
{
	std::ifstream myReadFile("day21_input.txt");
	std::string inputLine{};

	uint64_t part1{};
	uint64_t part2{};

	while (std::getline(myReadFile, inputLine))
	{
		int64_t num = std::stoll(inputLine.substr(0, inputLine.size() - 1));
		part1 += calculateRobotLength(inputLine, false, 2) * num;	//generate one shortest sequence for part 2 with one robots
		part2 += calculateRobotLength(inputLine, false, 25) * num;	//generate the shortest sequence for part 2 with 25 robots
		
	}
	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
}
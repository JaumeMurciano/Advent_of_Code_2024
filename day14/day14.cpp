#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>
struct Robot {
	std::array<int32_t, 2> position{};
	std::array<int32_t, 2> velocity{};
	void simulateStep(int32_t xLimit, int32_t yLimit)	//function used to simulate one step of each robot
	{
		position[0] += velocity[0];
		if (position[0] < 0) { position[0] += xLimit; }
		position[1] += velocity[1];
		if (position[1] < 0) { position[1] += yLimit; }
		position[0] %= xLimit;
		position[1] %= yLimit;
	}
	int32_t getQuadrant(int32_t xLimit, int32_t yLimit)	//used to get the current quadrant of each robot for part 1
	{
		if (position[0] == (xLimit / 2) || position[1] == (yLimit / 2)) { return 0; }
		if (position[0] < (xLimit / 2) && position[1] < (yLimit / 2)) { return 1; }
		if (position[0] > (xLimit / 2) && position[1] < (yLimit / 2)) { return 2; }
		if (position[0] < (xLimit / 2) && position[1] > (yLimit / 2)) { return 3; }
		if (position[0] > (xLimit / 2) && position[1] > (yLimit / 2)) { return 4; }
		return 0;
	}
};

int main()
{
	std::ifstream myReadFile("day14_input.txt");
	std::ofstream outputFile("day14_part2.txt");

	std::string inputLine{};

	int64_t part1{};
	int64_t part2{};
	std::array<int, 5> robotQuadrants{};

	std::vector<Robot> allRobots{};
	int32_t xLimit{101};				//input grid size data
	int32_t yLimit{103};
	int32_t nSeconds{ 20000 };			//guess that this will be enough for part 2
	std::vector<std::vector<int32_t>> robotPositions = std::vector<std::vector<int32_t>>(yLimit, std::vector<int>(xLimit, 0));	//used to scan the shape on part 2

	while (std::getline(myReadFile, inputLine))	//store all robots information
	{
		Robot newRobot{};
		std::stringstream line(inputLine);
		std::string eachWord{};
		getline(line, eachWord, ',');
		newRobot.position[0] = std::stoi(eachWord.substr(2));
		getline(line, eachWord, ' ');
		newRobot.position[1] = std::stoi(eachWord);
		getline(line, eachWord, ',');
		newRobot.velocity[0] = std::stoi(eachWord.substr(2));
		getline(line, eachWord, ' ');
		newRobot.velocity[1] = std::stoi(eachWord);
		allRobots.emplace_back(newRobot);
		robotPositions[newRobot.position[1]][newRobot.position[0]]++;
		robotQuadrants[newRobot.getQuadrant(xLimit, yLimit)]++;
	}

	for (int i{}; i < nSeconds; i++)
	{
		robotPositions = std::vector<std::vector<int32_t>>(yLimit, std::vector<int>(xLimit, 0));	//used to have a pseudoImage of the robots position at each step
		robotQuadrants = { 0,0,0,0,0 };
		for (Robot& eachRobot : allRobots)
		{
			eachRobot.simulateStep(xLimit,yLimit);
			robotPositions[eachRobot.position[1]][eachRobot.position[0]] = 1;
			robotQuadrants[eachRobot.getQuadrant(xLimit, yLimit)]++;	//count the quadrant of the robot for part 1
		}
		for (const std::vector<int32_t>& eachLine : robotPositions)		//check if there are at least ten robots next to each other in the same row. It will be assumed that they will perform the christmas tree
		{
			int32_t robotsInARow{};
			for (const int32_t eachNum : eachLine)
			{
				if (eachNum != 0) { robotsInARow++; }
				else { robotsInARow = 0; }
				if (robotsInARow >= 10) { part2 = i+1;  break; }
			}
			if (part2 != 0) { break; }
		}
		if (i == 99) { part1 = robotQuadrants[1] * robotQuadrants[2] * robotQuadrants[3] * robotQuadrants[4]; }
		if (i > 99 && part2 != 0) { break; }							//if part1 and part2 have been computed, break
	}
	
	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
}
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>
#include <unordered_map>

struct Point {
	int32_t row{};
	int32_t column{};
	Point operator+(const Point& otherPoint)const { return Point{ row + otherPoint.row,column + otherPoint.column }; }
};

bool moveRock(std::vector<std::string>& inputMap, Point& currentPosition, Point& movementDirection) //used to move part1 rocks and horizontally part 2
{
	Point newPosition = currentPosition + movementDirection;		//calculate new position
	if (inputMap[newPosition.row][newPosition.column] == '.')		//if there isn't anything there, move everything recursively
	{
		inputMap[newPosition.row][newPosition.column] = inputMap[currentPosition.row][currentPosition.column];
		inputMap[currentPosition.row][currentPosition.column] = '.';
		currentPosition = newPosition;
		return true;												//flag to indicate previous rocks that the movement is possible
	}
	else if (inputMap[newPosition.row][newPosition.column] == 'O' || inputMap[newPosition.row][newPosition.column] == '[' || inputMap[newPosition.row][newPosition.column] == ']')
	{
		Point posToEnter = newPosition;
		if (moveRock(inputMap, posToEnter, movementDirection))		//check if the rock is possible to move, and if so, move everything recursively
		{
			inputMap[newPosition.row][newPosition.column] = inputMap[currentPosition.row][currentPosition.column];
			inputMap[currentPosition.row][currentPosition.column] = '.';
			currentPosition = newPosition;
			return true;											//flag to indicate previous rocks that the movement is possible
		}
		else { return false; }										//movement is not possible, flag false recursively		
	}
	else if (inputMap[newPosition.row][newPosition.column] == '#') { return false;} //There is a # and move is not possible, so nothing will be moved
	
	return false;													//should never arrive here
}

bool checkVerticalRock(const std::vector<std::string>& inputMap, const Point& currentPosition, const Point& movementDirection)	//we always move a rock entering its left coordinate
{
	Point newPosition = currentPosition + movementDirection;
	if (inputMap[newPosition.row][newPosition.column] == '.' && inputMap[newPosition.row][newPosition.column+1] == '.')			//nothing is there, movement is possible
	{
		return true;
	}
	else if (inputMap[newPosition.row][newPosition.column] == '#' || inputMap[newPosition.row][newPosition.column + 1] == '#')	//a rock is over, movement is not possible
	{
		return false;
	}
	else if (inputMap[newPosition.row][newPosition.column] == '[' && inputMap[newPosition.row][newPosition.column + 1] == ']')	//a rock is perfectly placed over current rock
	{
		return checkVerticalRock(inputMap,newPosition,movementDirection);
	}
	else if (inputMap[newPosition.row][newPosition.column] == ']' && inputMap[newPosition.row][newPosition.column + 1] == '[')	//two rocks are placed over current, check both
	{
		Point leftRock{ newPosition.row,newPosition.column - 1 };
		Point rightRock{ newPosition.row,newPosition.column + 1 };
		return checkVerticalRock(inputMap, leftRock, movementDirection) && checkVerticalRock(inputMap, rightRock, movementDirection);
	}
	else if (inputMap[newPosition.row][newPosition.column] == ']' && inputMap[newPosition.row][newPosition.column + 1] == '.')	//rock is on the left
	{
		newPosition.column -= 1;
		return checkVerticalRock(inputMap, newPosition, movementDirection);
	}
	else if (inputMap[newPosition.row][newPosition.column] == '.' && inputMap[newPosition.row][newPosition.column + 1] == '[')	//rock is on the right
	{
		newPosition.column += 1;
		return checkVerticalRock(inputMap, newPosition, movementDirection);
	}
	return false;							//should never arrive here
}

void moveVerticalRock(std::vector<std::string>& inputMap, Point& currentPosition, const Point& movementDirection)				//before entering here, we know that the movement is possible
{
	Point newPosition = currentPosition + movementDirection;
	if (inputMap[newPosition.row][newPosition.column] == '.' && inputMap[newPosition.row][newPosition.column + 1] == '.')		//space free, move all the rock
	{
		inputMap[newPosition.row][newPosition.column] = inputMap[currentPosition.row][currentPosition.column];
		inputMap[newPosition.row][newPosition.column+1] = inputMap[currentPosition.row][currentPosition.column+1];
		inputMap[currentPosition.row][currentPosition.column] = '.';
		inputMap[currentPosition.row][currentPosition.column+1] = '.';
		currentPosition = newPosition;
		return;
	}
	else if (inputMap[newPosition.row][newPosition.column] == '[' && inputMap[newPosition.row][newPosition.column + 1] == ']')	//move rock placed over current rock
	{
		Point posToEnter = newPosition;
		moveVerticalRock(inputMap, posToEnter, movementDirection);
		inputMap[newPosition.row][newPosition.column] = inputMap[currentPosition.row][currentPosition.column];
		inputMap[newPosition.row][newPosition.column + 1] = inputMap[currentPosition.row][currentPosition.column + 1];
		inputMap[currentPosition.row][currentPosition.column] = '.';
		inputMap[currentPosition.row][currentPosition.column + 1] = '.';
		currentPosition = newPosition;
		return;
	}
	else if (inputMap[newPosition.row][newPosition.column] == ']' && inputMap[newPosition.row][newPosition.column + 1] == '[')	//move two rocks over the current rock
	{
		Point leftRock{ newPosition.row,newPosition.column - 1 };
		Point rightRock{ newPosition.row,newPosition.column + 1 };
		moveVerticalRock(inputMap, leftRock, movementDirection);
		moveVerticalRock(inputMap, rightRock, movementDirection);
		inputMap[newPosition.row][newPosition.column] = inputMap[currentPosition.row][currentPosition.column];
		inputMap[newPosition.row][newPosition.column + 1] = inputMap[currentPosition.row][currentPosition.column + 1];
		inputMap[currentPosition.row][currentPosition.column] = '.';
		inputMap[currentPosition.row][currentPosition.column + 1] = '.';
		currentPosition = newPosition;
		return;
	}
	else if (inputMap[newPosition.row][newPosition.column] == ']' && inputMap[newPosition.row][newPosition.column + 1] == '.')	//move left rock over the current rock
	{
		Point posToEnter{ newPosition.row,newPosition.column -1};
		moveVerticalRock(inputMap, posToEnter, movementDirection);
		inputMap[newPosition.row][newPosition.column] = inputMap[currentPosition.row][currentPosition.column];
		inputMap[newPosition.row][newPosition.column + 1] = inputMap[currentPosition.row][currentPosition.column + 1];
		inputMap[currentPosition.row][currentPosition.column] = '.';
		inputMap[currentPosition.row][currentPosition.column + 1] = '.';
		currentPosition = newPosition;
		return;
	}
	else if (inputMap[newPosition.row][newPosition.column] == '.' && inputMap[newPosition.row][newPosition.column + 1] == '[')	//move right rock over current rock
	{
		Point posToEnter{ newPosition.row,newPosition.column + 1 };
		moveVerticalRock(inputMap, posToEnter, movementDirection);
		inputMap[newPosition.row][newPosition.column] = inputMap[currentPosition.row][currentPosition.column];
		inputMap[newPosition.row][newPosition.column + 1] = inputMap[currentPosition.row][currentPosition.column + 1];
		inputMap[currentPosition.row][currentPosition.column] = '.';
		inputMap[currentPosition.row][currentPosition.column + 1] = '.';
		currentPosition = newPosition;
		return;
	}
}

void addPart2Line(const std::string& inputLine, std::vector<std::string>& part2Map, Point& part2Robot)		//function used to convert input line to part2 map
{
	std::string constructedLine{};
	for (const char eachChar : inputLine)
	{
		if (eachChar == '#') { constructedLine.push_back('#'); constructedLine.push_back('#');}
		else if (eachChar == 'O') { constructedLine.push_back('['); constructedLine.push_back(']');}
		else if (eachChar == '.') { constructedLine.push_back('.'); constructedLine.push_back('.'); }
		else if (eachChar == '@') { part2Robot.row = part2Map.size(); part2Robot.column = constructedLine.size(); constructedLine.push_back('@'); constructedLine.push_back('.'); }
	}
	part2Map.emplace_back(constructedLine);
}

int main()
{
	std::ifstream myReadFile("day15_input.txt");
	std::string inputLine{};
	bool secondPart{ false };

	int64_t part1{};
	std::vector<std::string> inputMap{};
	Point robotPosition{};						//part 1 robot position

	int64_t part2{};
	std::vector<std::string> part2Map{};
	Point part2Robot{};							//part 2 robot position

	std::unordered_map<char, Point> possibleMovements{ {'^',Point{-1,0}}, {'>',Point{0,1}}, {'v',Point{1,0}}, {'<',Point{0,-1}} };

	while (std::getline(myReadFile, inputLine))
	{
		if (inputLine == "") { secondPart = true; continue; }
		if (!secondPart)						//reading map
		{ 
			if (inputLine.find('@') != std::string::npos)	//if robot is here, store its position
			{ 
				robotPosition.row = static_cast<int32_t>(inputMap.size()); 
				robotPosition.column = inputLine.find('@');
			};
			inputMap.emplace_back(inputLine); 
			addPart2Line(inputLine,part2Map,part2Robot);	//convert line to part 2 and store it
		}
		else
		{
			for (char eachMovement : inputLine) //move Part1 map
			{
				Point newPosition = robotPosition + possibleMovements[eachMovement];	//calculate next candidate position
				if (inputMap[newPosition.row][newPosition.column] == '.')				//if nothing is on the next position, move the robot
				{
					inputMap[robotPosition.row][robotPosition.column] = '.';
					inputMap[newPosition.row][newPosition.column] = '@';
					robotPosition = newPosition; 
				}
				else if (inputMap[newPosition.row][newPosition.column] == 'O') { moveRock(inputMap, robotPosition, possibleMovements[eachMovement]); }	//if there is a rock, try to move it
				else if (inputMap[newPosition.row][newPosition.column] == '#') { continue; }	//if there is a rock, don't do anything
			}

			for (char eachMovement : inputLine) //move Part2 map
			{
				Point newPosition = part2Robot + possibleMovements[eachMovement];		//calculate next position
				if (part2Map[newPosition.row][newPosition.column] == '.')				//if nothing is on the next position, move the robot
				{
					part2Map[part2Robot.row][part2Robot.column] = '.';
					part2Map[newPosition.row][newPosition.column] = '@';
					part2Robot = newPosition;
				}
				else if (part2Map[newPosition.row][newPosition.column] == '#') { continue; }	//if there is a rock, don't do anything
				else if (part2Map[newPosition.row][newPosition.column] == '[')			//try to move rock
				{
					if (eachMovement == '>') { moveRock(part2Map, part2Robot, possibleMovements[eachMovement]); }	//if movement is horizontal reuse part1 rock function
					else
					{																	//move is vertical
						Point posToMove = newPosition;
						if (checkVerticalRock(part2Map, part2Robot, possibleMovements[eachMovement])) { //check if the rock can be moved
							moveVerticalRock(part2Map, posToMove, possibleMovements[eachMovement]);		//move the rock knowing it is possible
							part2Map[newPosition.row][newPosition.column] = '@';						//move robot
							part2Map[part2Robot.row][part2Robot.column] = '.';
							part2Robot = newPosition;
						} 
					}
				}
				else if (part2Map[newPosition.row][newPosition.column] == ']')			//try to move rock
				{
					if (eachMovement == '<') { moveRock(part2Map, part2Robot, possibleMovements[eachMovement]); }	//if movement is horizontal reuse part1 rock function
					else
					{																	//move is vertical
						Point posToCheck{part2Robot.row,part2Robot.column-1};
						Point posToMove{ newPosition.row, newPosition.column - 1 };
						if (checkVerticalRock(part2Map, posToCheck, possibleMovements[eachMovement])) {	//check if the rock can be moved
							moveVerticalRock(part2Map, posToMove, possibleMovements[eachMovement]);		//move the rock knowing it is possible
							part2Map[newPosition.row][newPosition.column] = '@';						//move robot
							part2Map[part2Robot.row][part2Robot.column] = '.';
							part2Robot = newPosition;
						} 
					}
				}
			}
		}
	}

	//calculate part1 result
	for (int i{1}; i < inputMap.size(); i++)
	{
		for (int j{1}; j < inputMap[0].size(); j++)
		{
			if (inputMap[i][j] == 'O') { part1 += 100 * i + j; }
		}
	}
	//calculate part 2 result
	for (int i{ 1 }; i < part2Map.size(); i++)
	{
		for (int j{2}; j < part2Map[0].size(); j++)
		{
			if (part2Map[i][j] == '[') { part2 += 100 * i + j; }
		}
	}

	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
}
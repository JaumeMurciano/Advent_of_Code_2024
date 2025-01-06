#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>

struct Point {
	int32_t row{};
	int32_t column{};

	Point operator+(const Point& otherPoint)const { return Point{ row + otherPoint.row,column + otherPoint.column }; }
	Point operator*(const int32_t product)const { return Point{ row * product, column * product }; }
	bool operator<(const Point& otherPoint) const			//used to be able to have points in a set
	{
		if (row < otherPoint.row) { return true; }
		if (row == otherPoint.row && column < otherPoint.column) { return true; }
		return false;
	}
};
void exploreSide(std::vector<std::string>& inputMap, Point initialPos, Point movDirection, std::set<Point>& sidePositions, Point initialFencePos)
{
	Point fenceDirection{ movDirection.column,movDirection.row };	//fence direction is parallel to the movement, and is detected by crossing it
	Point movingPosition{ initialPos };								//"our" side of the fence					
	Point fencePosition{ initialFencePos };							//fence position
	Point otherSide{ initialPos.row + movDirection.row, initialPos.column + movDirection.column };	//the other side of the fence
	while (inputMap[movingPosition.row][movingPosition.column] == inputMap[initialPos.row][initialPos.column] && !(inputMap[otherSide.row][otherSide.column] == inputMap[initialPos.row][initialPos.column]))
	{	//we explore that side until we go out of the map, our side of the fence ends OR the fence ends bc another tile of our type is at "the other side"
		sidePositions.emplace(fencePosition);				//emplace fence position
		movingPosition = movingPosition + fenceDirection;	//move all positions in the fence direction
		otherSide = otherSide + fenceDirection;
		fencePosition = fencePosition + fenceDirection * 3;	//*3 is explained bc of fence coordinates
		if (movingPosition.row < 0 || movingPosition.column < 0 || movingPosition.row >= inputMap.size() || movingPosition.column >= inputMap[0].size())	//if we go out of the map, stop
		{
			break;
		}
	}
	fenceDirection.row *= -1;								//fence continues to the inverse direction
	fenceDirection.column *= -1;

	movingPosition = initialPos + fenceDirection;			//restart the search of the fence but to the ogher side
	fencePosition = initialFencePos + fenceDirection * 3;
	otherSide.row = movingPosition.row + movDirection.row;
	otherSide.column = movingPosition.column + movDirection.column;
	while (inputMap[movingPosition.row][movingPosition.column] == inputMap[initialPos.row][initialPos.column] && !(inputMap[otherSide.row][otherSide.column] == inputMap[initialPos.row][initialPos.column]))
	{
		sidePositions.emplace(fencePosition);
		movingPosition = movingPosition + fenceDirection;
		otherSide = otherSide + fenceDirection;
		fencePosition = fencePosition + fenceDirection * 3;
		if (movingPosition.row < 0 || movingPosition.column < 0 || movingPosition.row >= inputMap.size() || movingPosition.column >= inputMap[0].size())
		{
			break;
		}
	}
}
//BFS function that explores all tiles of the same type in one region
void calculateFencePrice(std::vector<std::string>& inputMap, char currentType, Point currentPos, int64_t& area, int64_t& perimeter, std::set<Point>& visitedPositions, int64_t& currentSides, std::set<Point>& sidePositions)
{
	int64_t addPerimeter{ 4 };								//fence perimeter is 4 and is decremented by 1 for each tile with the same type that this fence has
	area++;													//for each area visited, area is increased by one
	visitedPositions.emplace(currentPos);					//current position has been visited
	const std::vector<Point> possibleMovements{ {0, 1}, {1,0}, {0,-1}, {-1,0} };

	for (const Point eachMovement : possibleMovements)
	{
		Point newPos = currentPos + eachMovement;
		if (newPos.row < 0 || newPos.column < 0 || newPos.row >= inputMap.size() || newPos.column >= inputMap[0].size() || inputMap[newPos.row][newPos.column] != currentType)
		{	//if the position goes out of bound or the new tile is of a different type, a fence has to be built
			Point fencePos = Point{ currentPos.row * 3 + 1 + eachMovement.row, currentPos.column * 3 + 1 + eachMovement.column };//fence coordinates. two empty spaces are left between each row and each column to be able to place fences from the two sides
			if(sidePositions.find(fencePos) == sidePositions.end())		//search if current fence has been already found
			{															//explore that side and add one to the side counter
				exploreSide(inputMap, currentPos, eachMovement, sidePositions, fencePos);
				currentSides++;
			}
			continue;
		}
		addPerimeter--;
	}
	for (const Point eachMovement : possibleMovements)
	{	//BFS over all the same type neighbours
		Point newPos = currentPos + eachMovement;
		if (newPos.row < 0 || newPos.column < 0 || newPos.row >= inputMap.size() || newPos.column >= inputMap[0].size() || inputMap[newPos.row][newPos.column] != currentType)
		{
			continue;
		}
		if (visitedPositions.find(newPos) == visitedPositions.end())
		{
			calculateFencePrice(inputMap, currentType, newPos, area, perimeter, visitedPositions, currentSides, sidePositions);
		}
	}
	perimeter += addPerimeter;
}

int main()
{
	std::ifstream myReadFile("day12_input.txt");
	std::string inputLine{};

	int64_t part1{};
	int64_t part2{};
	std::vector<std::string> inputMap{};
	std::set<Point> visitedPositions{};			//store all the visited positions for the BFS

	while (std::getline(myReadFile, inputLine))
	{
		inputLine = "#" + inputLine;			//add a # frame
		inputLine.push_back('#');				//add a # frame
		if (inputMap.size() == 0) { inputMap.emplace_back(std::string(inputLine.size(), '#')); }
		inputMap.emplace_back(inputLine);
	}

	inputMap.emplace_back(std::string(inputMap[0].size(), '#'));	
	//search for fence price at all unvisited, non-fence positions
	for (int32_t row{ 1 }; row < inputMap.size() - 1; row++)
	{
		for (int32_t column{ 1 }; column < inputMap[0].size() - 1; column++)
		{
			std::set<Point> sidePositions{};
			int64_t area{};
			int64_t perimeter{};
			int64_t nSides{};
			if (visitedPositions.find(Point{ row, column }) == visitedPositions.end())
			{
				calculateFencePrice(inputMap, inputMap[row][column], Point{ row, column }, area, perimeter, visitedPositions, nSides, sidePositions);
			}
			part1 += area * perimeter;
			part2 += area * nSides;
		}
	}
	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
}
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <set>

void parseLine(std::string& inputLine, std::vector<double>& buttonCapabilities, bool resultLine)
{
	std::stringstream firstLine(inputLine);
	std::string eachNum{};
	getline(firstLine, eachNum, ' ');									//Skip Button or Prize:
	if (!resultLine) { getline(firstLine, eachNum, ' '); }				//Skip button letter (A/B) }

	while (getline(firstLine, eachNum, ' '))
	{
		if (eachNum[eachNum.size() - 1] == ',') { eachNum.erase(eachNum.size() - 1); }
		buttonCapabilities.emplace_back(std::stod(eachNum.substr(2)));	//store the number without the X+ or Y+
	}
}

void calculateButtonPresses(const std::vector<double>& buttonCapabilities, int64_t& result)
{
	//Solve equation: B = ((n2*a1/a2)-n1)/((b2*a1/a1)-b1)
	double B = static_cast<double>((buttonCapabilities[5] * buttonCapabilities[0] / buttonCapabilities[1]) - buttonCapabilities[4]) / ((buttonCapabilities[3] * buttonCapabilities[0] / buttonCapabilities[1]) - buttonCapabilities[2]);
	//Solve equation: A = (n1-a2*B)/a1
	double A = static_cast<double>((buttonCapabilities[4] - (buttonCapabilities[2] * B)) / buttonCapabilities[0]);
	int64_t A_presses = static_cast<int64_t>(A + 0.01);
	int64_t B_presses = static_cast<int64_t>(B + 0.01);

	if (A_presses * buttonCapabilities[0] + B_presses * buttonCapabilities[2] == buttonCapabilities[4] && A_presses * buttonCapabilities[1] + B_presses * buttonCapabilities[3] == buttonCapabilities[5])
	{
		//if integer pressed buttons are correct, add button cost
		result += A_presses * 3 + B_presses;
	}
}

int main()
{
	std::ifstream myReadFile("day13_input.txt");
	std::string inputLine{};

	int64_t part1{};
	int64_t part2{};

	while (std::getline(myReadFile, inputLine))
	{
		std::vector<double> buttonCapabilities{};
		parseLine(inputLine, buttonCapabilities, false);	//store Button A coefficients
		std::getline(myReadFile, inputLine);
		parseLine(inputLine, buttonCapabilities, false);	//store Button B coefficients
		std::getline(myReadFile, inputLine);
		parseLine(inputLine, buttonCapabilities, true);		//store results
		std::getline(myReadFile, inputLine);
		
		calculateButtonPresses(buttonCapabilities, part1);	//solve equation for part 1 coordinates
		buttonCapabilities[4] += 10000000000000;			//add offset coordinates
		buttonCapabilities[5] += 10000000000000;			//add offset coordinates
		calculateButtonPresses(buttonCapabilities, part2);	//solve equation for part 2 coordinates
	}

	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
}
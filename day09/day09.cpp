#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_map>

struct File
{
	int32_t id{};
	int32_t length{};						//file length, only used in part 2
	bool moved{ false };					//bool to store if a file has been moved, part 2
};

int main()
{
	std::ifstream myReadFile("day09_input.txt");
	std::string inputLine{};

	int64_t part1{};
	int64_t part2{};

	std::vector<File> allFiles{};			//part 1, each file id occupies one position, and is repeated n times
	std::vector<File> compressedFiles{};	//part 2, each file and each slot occupies one position
	int currentId{};

	while (std::getline(myReadFile, inputLine))
	{
		for (int i{}; i < inputLine.size(); i++)
		{
			if (i % 2 == 0)					//newFile
			{
				int32_t repeat{ inputLine[i] - '0' };
				compressedFiles.emplace_back(File{ currentId, repeat});		//emplace one file with id and length for part 2
				while (repeat > 0) { allFiles.emplace_back(File{ currentId }); repeat--; }	//emplace one slot for each length for part 1
				currentId++;
			}
			else							//repeat the process for new space
			{
				int repeat{ inputLine[i] - '0' };
				if(repeat>0){ compressedFiles.emplace_back(File{ -1, repeat }); }
				while (repeat > 0) { allFiles.emplace_back(File{ -1 }); repeat--; }
			}
		}
	}

	int32_t front{};						//declare front and back iterators
	int32_t back{ static_cast<int32_t>(allFiles.size()) - 1 };

	while (front <= back)					//part 1: two iterator approach. emplace each file at the back to each space at the front
	{
		if (allFiles[front].id != -1) { part1 += front * allFiles[front].id; front++; }	//part 1 solution can be calculated on the go
		else
		{
			if (allFiles[back].id == -1) { back--; continue; }
			else
			{
				allFiles[front].id = allFiles[back].id;
				back--;
			}
		}
	}

	for (back = compressedFiles.size() - 1; back > 0; back--)	//part 2: search for each file at the back and try to emplace it at each of the slots at the front
	{
		while (compressedFiles[back].id == -1 || compressedFiles[back].moved) { back--; }	//search for a file at the back
		for (front = 0; front < back; front++)
		{
			while (compressedFiles[front].id != -1 && front < back) { front++; }			//search for a slot at the front
			if (front >= back) { break; }
			//try to allocate the file
			if (compressedFiles[front].length == compressedFiles[back].length)				//if space matches perfectly, only the ID has to be swapped
			{
				compressedFiles[front].id = compressedFiles[back].id;
				compressedFiles[front].moved = true;
				compressedFiles[back].id = -1;
				break; //allocated!
			}
			if (compressedFiles[front].length > compressedFiles[back].length)				//if space does not match perfectly, emplace a file and subtract space from the original space
			{
				compressedFiles.insert(compressedFiles.begin() + front, compressedFiles[back]);
				back++;
				front++;
				compressedFiles[front].length -= compressedFiles[back].length;
				compressedFiles[front-1].moved = true;
				compressedFiles[back].id = -1;
				break;
			}

		}
		if (compressedFiles[back].id == -1)					//file has been moved, try to merge space if there was free space before or after the emptied space.
		{
			if (back < compressedFiles.size()-1 && compressedFiles[back+1].id == -1)
			{
				compressedFiles[back].length += compressedFiles[back+1].length;
				compressedFiles.erase(compressedFiles.begin()+back+1);
			}
			if (compressedFiles[back - 1].id == -1)
			{
				compressedFiles[back].length += compressedFiles[back - 1].length;
				compressedFiles.erase(compressedFiles.begin() + back - 1);
				//back--;
			}
		}
	}
	int32_t currentPos{};
	for (const File& eachFile : compressedFiles)		//calculate part 2 output
	{
		for (int i{}; i < eachFile.length; i++)
		{
			if (eachFile.id != -1) { part2 += eachFile.id * currentPos; }
			currentPos++;
		}
	}
	std::cout << "Part 1 result is: " << part1 << "\n";
	std::cout << "Part 2 result is: " << part2;
	return 0;
}
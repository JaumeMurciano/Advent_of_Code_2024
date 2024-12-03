#include <fstream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <regex>

int64_t solvePart1()
{
    std::ifstream myReadFile("proves.txt");
    std::string inputLine{};

    std::regex find_muls("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)"); //regex to search mul(XXX,XXX)
    int64_t part1{};

    while (std::getline(myReadFile, inputLine))
    {
        for (std::smatch sm; regex_search(inputLine, sm, find_muls);) //iterate over the regex to get the numbers and multiply them
        {
            part1 += std::stoi(sm[1]) * std::stoi(sm[2]);
            inputLine = sm.suffix();
        }
    }
    return part1;
}

int64_t solvePart2()
{
    std::ifstream myReadFile("day03_input.txt");
    std::string inputLine{};

    std::regex find_muls("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)"); //regex to get the num(XXX,XXX)
    std::regex find_do("do\\(\\)");                             //regex to search do()
    std::regex find_dont("don't\\(\\)");                        //regex to search don't()

    int64_t part2{};
    bool lastdont{ false };                                     //bool used to save if last line ended with a don't

    while (std::getline(myReadFile, inputLine))
    {
        std::smatch dopos;
        std::smatch dontpos;
        if (!lastdont)                                          //if last line didn't end with a don't search the first do and don't and store them
        {
            regex_search(inputLine, dopos, find_do);
            regex_search(inputLine, dontpos, find_dont);
        }
        if (lastdont)                                           //if last line ended with a don't, cut until the first do() and then start iterating
        {
            regex_search(inputLine, dopos, find_do);
            inputLine = dopos.suffix();
            regex_search(inputLine, dopos, find_do);
            regex_search(inputLine, dontpos, find_dont);
            lastdont = false;
        }
        int64_t doPos{ dopos.position() };
        int64_t dontPos{ dontpos.position() };
        for (std::smatch sm; regex_search(inputLine, sm, find_muls);) //iterate over the input searching for the mul()
        {
            doPos -= (sm.position() + static_cast<int>(sm[0].str().size())); 
            dontPos -= (sm.position() + static_cast<int>(sm[0].str().size())); //count if we have passed over any don't()

            if (dontPos < 0)                                    //if we have jumped over a don't(), cut the string until the next do and search again
            {
                regex_search(inputLine, dopos, find_do);
                if (static_cast<std::size_t>(dopos.position()) > inputLine.size())        //used to store if the line ended with an active don't()
                {
                    lastdont = true;
                }
                inputLine = dopos.suffix();                     //cut until the next do()
                regex_search(inputLine, dontpos, find_dont);    //search for the next don't()
                regex_search(inputLine, dopos, find_do);        //search for the next do()
                doPos = dopos.position();
                dontPos = dontpos.position();
                continue;
            }
            else                                                //if no don'ts have been jumped over, add the mul()
            {
                part2 += std::stoi(sm[1]) * std::stoi(sm[2]);
                inputLine = sm.suffix();
            }
        }
    }
    return part2;
}

int main() {
    std::cout << "Part 1 solution is: " << solvePart1() << "\n";
    std::cout << "Part 2 solution is: " << solvePart2() << "\n";
    return 0;
}
#include <fstream>
#include <iostream>
#include <regex>
#include <days.hpp>

/**
 --- Day 3: Mull It Over ---

"Our computers are having issues, so I have no idea if we have any Chief Historians in stock! You're welcome to check the warehouse, though," says the mildly flustered shopkeeper at the North Pole Toboggan Rental Shop. The Historians head out to take a look.

The shopkeeper turns to you. "Any chance you can see why our computers are having issues again?"

The computer appears to be trying to run a program, but its memory (your puzzle input) is corrupted. All of the instructions have been jumbled up!

It seems like the goal of the program is just to multiply some numbers. It does that with instructions like mul(X,Y), where X and Y are each 1-3 digit numbers. For instance, mul(44,46) multiplies 44 by 46 to get a result of 2024. Similarly, mul(123,4) would multiply 123 by 4.

However, because the program's memory has been corrupted, there are also many invalid characters that should be ignored, even if they look like part of a mul instruction. Sequences like mul(4*, mul(6,9!, ?(12,34), or mul ( 2 , 4 ) do nothing.

For example, consider the following section of corrupted memory:

xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))

Only the four highlighted sections are real mul instructions. Adding up the result of each instruction produces 161 (2*4 + 5*5 + 11*8 + 8*5).

Scan the corrupted memory for uncorrupted mul instructions. What do you get if you add up all of the results of the multiplications?

 */

int day_3_part_1(std::ifstream &fileStream) {
    std::regex expression("mul\\((\\d{1,3}),(\\d{1,3})\\)+");
    int result = 0;
    std::string line;

    while (getline(fileStream, line)) {
        auto words_begin = std::sregex_iterator(line.begin(), line.end(), expression);
        auto words_end = std::sregex_iterator();
        for (std::sregex_iterator i = words_begin; i != words_end; i++) {
            std::smatch match = *i;
            std::string match_str = match.str();
            int first = std::stoi(match.str(1));
            int second = std::stoi(match.str(2));
            result += first * second;
        }
    }
    return result;
}

/**
As you scan through the corrupted memory, you notice that some of the conditional statements are also still intact. If you handle some of the uncorrupted conditional statements in the program, you might be able to get an even more accurate result.

There are two new instructions you'll need to handle:

    The do() instruction enables future mul instructions.
    The don't() instruction disables future mul instructions.

Only the most recent do() or don't() instruction applies. At the beginning of the program, mul instructions are enabled.

For example:

xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))

This corrupted memory is similar to the example from before, but this time the mul(5,5) and mul(11,8) instructions are disabled because there is a don't() instruction before them. The other mul instructions function normally, including the one at the end that gets re-enabled by a do() instruction.

This time, the sum of the results is 48 (2*4 + 8*5).

Handle the new instructions; what do you get if you add up all of the results of just the enabled multiplications?
 */

int day_3_part_2(std::ifstream &fileStream) {
    std::regex conditionExpression(".*(do\\(\\))|.*(don't\\(\\))|.*(mul\\((\\d{1,3}),(\\d{1,3})\\))");
    int result = 0;
    char c;
    std::string word = "";
    bool executing = true;
    while (fileStream.get(c)) {
        word += c;
        if (c == ')') {
            std::smatch base_match;
            std::regex_match(word, base_match, conditionExpression);
            if (base_match[1].matched) {
                executing = true;
            } else if (base_match[2].matched) {
                executing = false;
            } else if (base_match[3].matched) {
                if (executing) {
                    int first = std::stoi(base_match.str(4));
                    int second = std::stoi(base_match.str(5));
                    result += first * second;
                }
            }
            word = "";
        }
    }
    return result;
}

void day_3() {
    std::ifstream file("../input/day_3_input.txt");
    int result = 0;
    int bonusResult = 0;
    if (file.is_open()) {
        result = day_3_part_1(file);
        file.clear();
        file.seekg(0, std::ios::beg);
        bonusResult = day_3_part_2(file);
    } else {
        std::cout << "We ded" << std::endl;
        std::cerr << "Could not open the input" << std::endl;
    }
    std::cout << "Day 3 Part 1: " << result << " Part 2: " << bonusResult << std::endl;
}
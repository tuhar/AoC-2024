#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <map>
#include <numeric>
#include <regex>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <days.hpp>


void load_input(std::vector<int>& list1, std::vector<int>& list2) {
    std::ifstream file("../input/day_1_input.txt");
    std::string line;

    if (file.is_open()) {
        while(getline(file, line)) {
            std::istringstream iss(line);
            int num1,num2;
            if (iss >> num1 >> num2) {
                list1.push_back(num1);
                list2.push_back(num2);
            } else {
                std::cerr << "Could not extract numbers from list" << std::endl;
            }
        }
        file.close();
    } else {
        std::cerr << "Could not open the input" << std::endl;
    }
}

void day_1() {

    std::vector<int> list1;
    std::vector<int> list2;
    load_input(list1, list2);

    std::stable_sort(list1.begin(), list1.end());
    std::stable_sort(list2.begin(), list2.end());

    std::vector<int>results;

    int result = std::accumulate(list1.begin(), list1.end(), 0, [&, i=0](int acc, int a) mutable {
                                                       int b = list2[i++];
                                                       return acc + abs(a -b);
    });

    std::map<int, int> groupedBy;
    for (int item : list2) {
        groupedBy[item]++;
    }
    int bonusResult = std::accumulate(list1.begin(), list1.end(), 0, [&](int acc, int a) mutable {
        return acc + (groupedBy[a] * a);
    });


    std::cout << "Day 1 Part 1: " << result << std::endl;
    std::cout << "Day 1 Part 2: " << bonusResult << std::endl;
}

bool check_unsafe(const int previous, const int current, const int streak) {
    int difference = previous - current;
    return (abs(difference) > 3 || difference == 0) || (streak != 0 && ((streak > 0 && difference < 0) || (streak < 0 && difference > 0)));
}

void day_2() {
//    std::ifstream file("../input/day_2_input.txt");
    std::ifstream file("../input/day_2_debug.txt");
//    std::ifstream file("../input/day_2_small_debug.txt");
    std::string line;
    int safeCounter = 0;
    if (file.is_open()) {
        while(getline(file, line)) {
            std::istringstream iss(line);
            int previous = -1;
            bool unsafe = false;
            int safeIgnore = 1;
            bool previousUnResolved = false;
            int streak = 0;
            int firstUnsafe = -1;
            int firstUnsafeStreak = 0;
            int previousUnsafe = -1;
            int previousUnsafeStreak = 0;
            int currentUnsafe = -1;
            int currentUnsafeStreak = 0;
            for(int current; iss >> current;) {
                if (previous < 0) {
                    previous = current;
                } else {
                    if (previousUnResolved) { //part 2
//                        std::cout << "pu: " << previousUnsafe << ", cu: " << currentUnsafe << ", streak: " << streak << ", pUs: " << previousUnsafeStreak << ", cUs: " << currentUnsafeStreak << std::endl;
                        bool previousSafe = check_unsafe(previousUnsafe, current, previousUnsafeStreak);
                        bool currentSafe = check_unsafe(currentUnsafe, current, currentUnsafeStreak);
                        bool firstSafe = check_unsafe(previousUnsafe, currentUnsafe, previousUnsafe - currentUnsafe);
//                        std::cout << "perS: " << previousSafe << ", curS: " << currentSafe << std::endl;
                        if (!previous && !currentUnsafe && !firstSafe) {
                            unsafe = true;
//                            std::cout << "unsafe level : " << line << std::endl;
                            break;
                        }
                        if (previousSafe) {
                            streak = previousUnsafe - current;
                        } else if (currentSafe) {
                            streak = currentUnsafe - current;
                        } else if (firstSafe) {
                            streak = previousUnsafe - currentUnsafe;
                        }
                        previous = current;
                        previousUnResolved = false;
//                        std::cout << "p: "<< previous << ", c: "<< current << ", pu: " << previousUnsafe << ", cu: " << currentUnsafe << ", streak: " << streak << std::endl;
                    } else {
//                        std::cout << "p: "<< previous << ", c: "<< current << ", pu: " << previousUnsafe << ", cu: " << currentUnsafe << ", streak: " << streak << std::endl;
                        unsafe = check_unsafe(previous, current, streak);
//                        std::cout << "unsafe: " << unsafe << std::endl;
                        if (unsafe) {
                            if (safeIgnore > 0) { //part 2
                                unsafe = false;
                                safeIgnore--;
                                firstUnsafe = previous + streak;
                                firstUnsafeStreak = streak;
                                previousUnResolved = true;
                                previousUnsafe = previous;
                                previousUnsafeStreak = streak;
                                currentUnsafe = current;
                                currentUnsafeStreak = (previous + streak) - current;
                            } else {
//                                std::cout << "unsafe level : " << line << std::endl;
                                break;
                            }
                        } else {
                            streak = previous - current; //part 2
                            previous = current;
                        }
                    }
                }
            }
            if (!unsafe) {
//                std::cout << "safe level : " << line << std::endl;
                safeCounter++;
            }
        }
        std::cout << "Day 2 Part 2: " << safeCounter << std::endl;
        file.close();
    } else {
        std::cerr << "Could not open the input" << std::endl;
    }
}

void day_3() {
    std::ifstream file("../input/day_3_input.txt");
    std::regex expression("mul\\((\\d{1,3}),(\\d{1,3})\\)+");
    std::regex conditionExpression(".*(do\\(\\))|.*(don't\\(\\))|.*(mul\\((\\d{1,3}),(\\d{1,3})\\))");
    std::string line;
    std::uint64_t result = 0;
    std::uint64_t bonus_result = 0;
    if (file.is_open()) {
        std::cout << "We have a file" << std::endl;

        char c;
        std::string word = "";
        bool executing = true;
        while(file.get(c)) {
            word += c;
            if (c == ')') {
                std::cout << "Found instruction: " << word << std::endl;
                std::smatch base_match;
                std::regex_match(word, base_match, conditionExpression);
                if (base_match[1].matched) {
                    std::cout << "Found do: " << std::endl;
                    executing = true;
                } else if (base_match[2].matched) {
                    std::cout << "Found dont " << std::endl;
                    executing = false;
                } else if (base_match[3].matched) {
                    std::cout << "Found mul: " << std::endl;
                    if (executing) {
                        std::cout << "we gut: " << std::endl;
                        int first = std::stoi(base_match.str(4));
                        int second = std::stoi(base_match.str(5));
                        bonus_result += first * second;
                    } else {
                        std::cout << "we not gut: " << std::endl;
                    }
                }
                word = "";
            }
        }

        while(getline(file, line)) {
            std::cout << "We have a line" << std::endl;
            auto words_begin = std::sregex_iterator(line.begin(), line.end(), expression);
            auto words_end = std::sregex_iterator();
            for(std::sregex_iterator i = words_begin; i != words_end; i++) {
                std::smatch match = *i;
                std::string match_str = match.str();
                int first = std::stoi(match.str(1));
                int second = std::stoi(match.str(2));
                std::cout << match_str << " first: " << first << " second: " << second << " multiple: " << first * second << std::endl;
                result += first * second;
            }
        }
    } else {
        std::cout << "We ded" << std::endl;
        std::cerr << "Could not open the input" << std::endl;
    }
    std::cout << "Day 3 Part 1: " << result << " Part 2: " << bonus_result << std:: endl;
}

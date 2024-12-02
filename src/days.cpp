#include <algorithm>
#include <cstdio>
#include <iostream>
#include <filesystem>
#include <map>
#include <numeric>
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

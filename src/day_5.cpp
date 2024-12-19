#include <ostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ranges>
#include <algorithm>

#include <days.hpp>
#include <day_5.hpp>

//build tree

//X|Y = put X then look at for Z|X and put all Zs before X
// line [X,Y]
// X
//  \
//   Y
//

void preceding(int x, std::vector<Rule> &rules, std::vector<int> results) {
    if (rules.empty()) {
        return;
    }
    std::vector<Rule> tail = std::vector(rules.begin() + 1, rules.end());
    int preceding_instruction = rules[0].x;
    preceding(preceding_instruction, tail, results);
}

std::vector<Rule> rules(const Rule &head, const std::vector<Rule> &tail) {
    if (tail.empty()) {
        return {};
    }
    //    std::vector<int> preceding_instructions = ;
    return {};
}



void check_preceding(const int& instruction, std::vector<Rule>& rules, std::vector<int>& precedingInstructions) {
    if (std::find(precedingInstructions.begin(), precedingInstructions.end(), instruction) != precedingInstructions.end()) {
        return;
    }
    for (auto it = rules.begin(); it != rules.end(); it++) {
        if (it->y == instruction && it->state == NOT_PROCESSED) {
            it->state = PROCESSING;
            check_preceding(it->x, rules, precedingInstructions);
        }
    }
    precedingInstructions.push_back(instruction);
}

void check_succeeding(const int& instruction, std::vector<Rule>& rules, std::vector<int>& succedingInstructions) {
    if (std::find(succedingInstructions.begin(), succedingInstructions.end(), instruction) != succedingInstructions.end()) {
        return;
    }
    for (auto it = rules.begin(); it != rules.end(); it++) {
        if (it->x == instruction && it->state == NOT_PROCESSED) {
            it->state = PROCESSING;
            check_succeeding(it->y, rules, succedingInstructions);
        }
    }
    succedingInstructions.insert(succedingInstructions.begin(), instruction);
}

std::vector<Rule> parse_rules(const std::string &path) {
    std::ifstream file(path);
    std::vector<Rule> rules;
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::istringstream ruleRaw(line);
            std::string num;
            std::vector<int> ruleParts;
            while (getline(ruleRaw, num, '|')) {
                ruleParts.push_back(std::stoi(num));
            }
            rules.push_back({ruleParts[0], ruleParts[1]});
        }
        std::cout << "Day 5 Part 1: " << rules[0].x << " Part 2: " << 0 << std::endl;
    } else {
        std::cout << "We ded" << std::endl;
        std::cerr << "Could not open the input " << path << std::endl;
    }
    return rules;
}

std::vector<int> compute_rule_ordering(std::vector<Rule> &rules) {
    std::vector<int> ruleOrdering;
    for (auto head = rules.begin(); head != rules.end(); head++) {
        //{47, 53}
        if (head->state == PROCESSED) {
            continue;
        }
        head->state = PROCESSING;
        std::vector<int> precedingInstructions, succedingInstructions;
        auto processedX = std::find(ruleOrdering.begin(), ruleOrdering.end(), head->x);
        auto processedY = std::find(ruleOrdering.begin(), ruleOrdering.end(), head->y);
        if (processedX == ruleOrdering.end()) {
            check_preceding(head->x, rules, precedingInstructions); // {97, 75, 43}
            check_succeeding(head->x, rules, succedingInstructions); // {43, 61, 53, 21, 13}
            std::cout << head->x << " processed. preceding: " << std::endl;
            for (const auto &item : precedingInstructions) {
                std::cout << item << ",";
                ruleOrdering.push_back(item);
            }
            std::cout << std::endl << "succeding: " << std::endl;
            for (auto it = succedingInstructions.begin() +1; it != succedingInstructions.end(); it++) {
                std::cout << *it << ",";
                ruleOrdering.push_back(*it);
            }
            std::cout << std::endl;
        }
//        if (processedY == ruleOrdering.end()) {
//            check_succeeding(head, rules, succedingInstructions); //
//            int index;
//            for (auto succ = succedingInstructions.end() -1; succ != succedingInstructions.begin(); succ--) {
//                auto it = std::find(ruleOrdering.begin(), ruleOrdering.end(), *succ);
//                if (it == ruleOrdering.end()) {
//
//                }
//                ruleOrdering.push_back(*succ);
//            }
//        }
        head->state = PROCESSED;
    }
    return ruleOrdering;
}
void day_5() {
    std::vector<Rule> rules = parse_rules("../input/day_5_example_rules.txt");
    auto result = compute_rule_ordering(rules);

    //take first rule
    //filter rules by predicate => rule.y == x
    //for each preceding rule, check its predecing rules until
}
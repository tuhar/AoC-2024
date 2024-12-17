#include <days.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <functional>
#include <map>

struct Cell {
    int row;
    int column;
};

bool operator<(const Cell &l, const Cell &r) {
    return l.row < r.row || (l.column < r.column && l.row == r.row);
}
bool operator==(const Cell &l, const Cell &r) {
    return l.row == r.row && l.column == r.column;
}

bool canRight(const int column, const std::vector<std::string> &grid) {
    return column + 1 < grid[0].size();
}

bool canDown(const int row, const std::vector<std::string> &grid) {
    return row + 1 < grid.size();
}

bool canUp(const int row) {
    return row > 0;
}

bool canLeft(const int column) {
    return column > 0;
}

void handle_candidate(std::map<Cell, std::vector<Cell>> &candidates,Cell leadingCell, Cell directionCell) {
    std::map<Cell, std::vector<Cell>>::iterator candidate = candidates.find(leadingCell);
    if (candidate != candidates.end()) {
        auto it = std::find(candidate->second.begin(), candidate->second.end(), directionCell);
        if (it != candidate->second.end()) {
            candidate->second.erase(it);
            std::cout << "Removing candidate: leading cell {" << leadingCell.row << ", " << leadingCell.column << "} direction {" << directionCell.row << ", " << directionCell.column << "}" << std::endl;
        };
    }
}

int complete_candidate(std::map<Cell, std::vector<Cell>> &candidates,Cell leadingCell, Cell directionCell) {
    std::map<Cell, std::vector<Cell>>::iterator candidate = candidates.find(leadingCell);
    if (candidate != candidates.end()) {
        if (std::find(candidate->second.begin(), candidate->second.end(), directionCell) != candidate->second.end()) {
//            std::cout << "Found XMAS: leading cell {" << leadingCell.row << ", " << leadingCell.column << "} direction {" << directionCell.row << ", " << directionCell.column << "}" << std::endl;
            return 1;
        };
    }
    return 0;
}

void check_M(const int row, const int column, const std::vector<std::string> &grid, std::map<Cell, std::vector<Cell>> &x_candidates, std::map<Cell, std::vector<Cell>> &s_candidates) {
    if (canRight(column, grid)) {
        char next_R = grid[row][column + 1];
        if (next_R != 'A') {
            handle_candidate(s_candidates, {row, column - 2}, { row, column -1 });
            handle_candidate(x_candidates, {row, column - 1}, { row, column });
        }
    }
    if (canRight(column, grid) && canDown(row, grid)) {
        char next_RD = grid[row + 1][column + 1];
        if (next_RD != 'A') {
            handle_candidate(s_candidates, {row - 2, column - 2}, { row - 1, column - 1 });
            handle_candidate(x_candidates, {row - 1, column - 1}, { row, column });
        }
    }
    if (canDown(row, grid)) {
        char next_D = grid[row + 1][column];
        if (next_D != 'A') {
            handle_candidate(s_candidates, {row - 2, column}, { row - 1, column });
            handle_candidate(x_candidates, {row - 1, column }, { row, column });
        }
    }
    if (canDown(row, grid) && canLeft(column)) {
        char next_LD = grid[row + 1][column - 1];
        if (next_LD != 'A') {
            handle_candidate(s_candidates, {row - 2, column + 2}, { row - 1, column + 1 });
            handle_candidate(x_candidates, {row - 1, column + 1}, { row, column });
        }
    }
}

void check_A(const int row, const int column, const std::vector<std::string> &grid, std::map<Cell, std::vector<Cell>> &x_candidates, std::map<Cell, std::vector<Cell>> &s_candidates) {
    if (canRight(column, grid)) {
        char next_R = grid[row][column + 1];
        if (next_R != 'S') {
            handle_candidate(s_candidates, {row, column - 1}, { row, column });
            handle_candidate(x_candidates, {row, column - 2}, { row, column - 1 });
        }
    }
    if (canRight(column, grid) && canDown(row, grid)) {
        char next_RD = grid[row + 1][column + 1];
        if (next_RD != 'S') {
            handle_candidate(s_candidates, {row - 1, column - 1}, { row, column });
            handle_candidate(x_candidates, {row - 2, column - 2}, { row - 1, column - 1 });
        }
    }
    if (canDown(row, grid)) {
        char next_D = grid[row + 1][column];
        if (next_D != 'S') {
            handle_candidate(s_candidates, {row - 1, column}, { row, column });
            handle_candidate(x_candidates, {row - 2, column}, { row - 1, column });
        }
    }
    if (canDown(row, grid) && canLeft(column)) {
        char next_LD = grid[row + 1][column - 1];
        if (next_LD != 'S') {
            handle_candidate(s_candidates, {row - 1, column + 1}, { row, column });
            handle_candidate(x_candidates, {row - 2, column + 2}, { row - 1, column + 1 });
        }
    }
}

int check_X(const int row, const int column, const std::vector<std::string> &grid, std::map<Cell, std::vector<Cell>> &x_candidates, std::map<Cell, std::vector<Cell>> &s_candidates) {
    int foundXmases = 0;
    //handle X candidates
//    std::cout << "Handling next:" << column << std::endl;
    if (canRight(column, grid)) {
        char next_R = grid[row][column + 1];
        if (next_R == 'M') {
            auto ret = x_candidates.insert(std::pair<Cell, std::vector<Cell>>({row, column}, {{row, column + 1}}));
            if (ret.second == false) {
                ret.first->second.push_back({row, column + 1});
            }
        }
    }
    if (canRight(column, grid) && canDown(row, grid)) {
        char next_RD = grid[row + 1][column + 1];
        if (next_RD == 'M') {
            auto ret = x_candidates.insert(std::pair<Cell, std::vector<Cell>>({row, column}, {{row + 1, column + 1}}));
            if (ret.second == false) {
                ret.first->second.push_back({row + 1, column + 1});
            }
        }
    }
    if (canDown(row, grid)) {
        char next_D = grid[row + 1][column];
        if (next_D == 'M') {
            auto ret = x_candidates.insert(std::pair<Cell, std::vector<Cell>>({row, column}, {{row + 1, column}}));
            if (ret.second == false) {
                ret.first->second.push_back({row + 1, column});
            }
        }
    }
    if (canDown(row, grid) && canLeft(column)) {
        char next_LD = grid[row + 1][column - 1];
        if (next_LD == 'M') {
            auto ret = x_candidates.insert(std::pair<Cell, std::vector<Cell>>({row, column}, {{row + 1, column - 1}}));
            if (ret.second == false) {
                ret.first->second.push_back({row + 1, column - 1});
            }
        }
    }

    //handle S candidates
    if (column - 3 >= 0) {
        foundXmases += complete_candidate(s_candidates, {row, column - 3}, { row, column - 2 });
    }
//    std::cout << "Handling last X:" << column << std::endl;
    if (column - 3 >= 0 && row - 3 >= 0) {
        foundXmases += complete_candidate(s_candidates, {row - 3, column - 3}, { row - 2, column - 2 });
    }
    if (row - 3 >= 0) {
        foundXmases += complete_candidate(s_candidates, { row - 3, column}, { row - 2 , column});
    }
    if (column + 3 < grid[0].size() && row - 3 >= 0) {
        foundXmases += complete_candidate(s_candidates, {row - 3, column + 3}, { row - 2, column + 2 });
    }
    return foundXmases;
}

int check_S(const int row, const int column, const std::vector<std::string> &grid, std::map<Cell, std::vector<Cell>> &x_candidates, std::map<Cell, std::vector<Cell>> &s_candidates) {
    int foundXmases = 0;
    //handle S candidates
    if (canRight(column, grid)) {
        char next_R = grid[row][column + 1];
        if (next_R == 'A') {
            auto ret = s_candidates.insert(std::pair<Cell, std::vector<Cell>>({row, column}, {{row, column + 1}}));
            if (ret.second == false) {
                ret.first->second.push_back({row, column + 1});
            }
        }
    }
    if (canRight(column, grid) && canDown(row, grid)) {
        char next_RD = grid[row + 1][column + 1];
        if (next_RD == 'A') {
            auto ret = s_candidates.insert(std::pair<Cell, std::vector<Cell>>({row, column}, {{row + 1, column + 1}}));
            if (ret.second == false) {
                ret.first->second.push_back({row + 1, column + 1});
            }
        }
    }
    if (canDown(row, grid)) {
        char next_D = grid[row + 1][column];
        if (next_D == 'A') {
            auto ret = s_candidates.insert(std::pair<Cell, std::vector<Cell>>({row, column}, {{row + 1, column}}));
            if (ret.second == false) {
                ret.first->second.push_back({row + 1, column});
            }
        }
    }
    if (canDown(row, grid) && canLeft(column)) {
        char next_LD = grid[row + 1][column - 1];
        if (next_LD == 'A') {
            auto ret = s_candidates.insert(std::pair<Cell, std::vector<Cell>>({row, column}, {{row + 1, column - 1}}));
            if (ret.second == false) {
                ret.first->second.push_back({row + 1, column - 1});
            }
        }
    }

    //handle X candidates
    if (column - 3 > 0) {
        foundXmases += complete_candidate(x_candidates, { row, column - 3 }, { row, column - 2});
    }
    if (column - 3 > 0 && row - 3 > 0) {
        foundXmases += complete_candidate(x_candidates, { row - 3, column - 3}, { row - 2 , column - 2 });
    }
    if (row - 3 > 0) {
        foundXmases += complete_candidate(x_candidates, { row - 3, column}, { row - 2 , column});
    }
    if (column + 3 < grid[0].size() && row - 3 > 0) {
        foundXmases += complete_candidate(x_candidates, { row - 3, column + 3 }, { row - 2 , column + 2 });
    }
    return foundXmases;
}

int day_4_part_1(std::ifstream &fileStream) {
    std::vector<std::string> grid;
    std::string inputLine;
    while (getline(fileStream, inputLine)) {
        grid.push_back(inputLine);
    }
    std::map<Cell, std::vector<Cell>> x_candidates;
    std::map<Cell, std::vector<Cell>> s_candidates;

    int result = 0;

    for (int row = 0; row < grid.size(); row++) {
//        std::cout << "row: " << row << std::endl;
        for (int column = 0; column < grid[row].size(); column++) {
//            std::cout << "column:" << column << std::endl;
            char character = grid[row][column];
            switch (character) {
            case 'X':
                result += check_X(row, column, grid, x_candidates, s_candidates);
                break;
            case 'S':
                result += check_S(row, column, grid, x_candidates, s_candidates);
                break;
            case 'A':
                check_A(row, column, grid, x_candidates, s_candidates);
                break;
            case 'M':
                check_M(row, column, grid, x_candidates, s_candidates);
                break;
            }
        }
    }

    return result;
}

int day_4_part_2(std::ifstream &fileStream) {
    return 0;
}

//2780 too high
//651 too low
//1405 too low

void day_4() {
    std::ifstream file("../input/day_4_input.txt");
    int result = 0;
    int bonusResult = 0;
    if (file.is_open()) {
        result = day_4_part_1(file);
        //        file.clear();
        //        file.seekg(0, std::ios::beg);
        //        bonusResult = part_2(file);
        std::cout << "Day 4 Part 1: " << result << " Part 2: " << bonusResult << std::endl;
    } else {
        std::cout << "We ded" << std::endl;
        std::cerr << "Could not open the input" << std::endl;
    }
}
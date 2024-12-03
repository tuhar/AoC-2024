#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <map>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <days.hpp>

/**
 * --- Day 2: Red-Nosed Reports ---

Fortunately, the first location The Historians want to search isn't a long walk from the Chief Historian's office.

While the Red-Nosed Reindeer nuclear fusion/fission plant appears to contain no sign of the Chief Historian, the engineers there run up to you as soon as they see you. Apparently, they still talk about the time Rudolph was saved through molecular synthesis from a single electron.

They're quick to add that - since you're already here - they'd really appreciate your help analyzing some unusual data from the Red-Nosed reactor. You turn to check if The Historians are waiting for you, but they seem to have already divided into groups that are currently searching every corner of the facility. You offer to help with the unusual data.

The unusual data (your puzzle input) consists of many reports, one report per line. Each report is a list of numbers called levels that are separated by spaces. For example:

7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9

This example data contains six reports each containing five levels.

The engineers are trying to figure out which reports are safe. The Red-Nosed reactor safety systems can only tolerate levels that are either gradually increasing or gradually decreasing. So, a report only counts as safe if both of the following are true:

    The levels are either all increasing or all decreasing.
    Any two adjacent levels differ by at least one and at most three.

In the example above, the reports can be found safe or unsafe by checking those rules:

    7 6 4 2 1: Safe because the levels are all decreasing by 1 or 2.
    1 2 7 8 9: Unsafe because 2 7 is an increase of 5.
    9 7 6 2 1: Unsafe because 6 2 is a decrease of 4.
    1 3 2 4 5: Unsafe because 1 3 is increasing but 3 2 is decreasing.
    8 6 4 4 1: Unsafe because 4 4 is neither an increase or a decrease.
    1 3 6 7 9: Safe because the levels are all increasing by 1, 2, or 3.

So, in this example, 2 reports are safe.

Analyze the unusual data from the engineers. How many reports are safe?

--- Part Two ---

The engineers are surprised by the low number of safe reports until they realize they forgot to tell you about the Problem Dampener.

The Problem Dampener is a reactor-mounted module that lets the reactor safety systems tolerate a single bad level in what would otherwise be a safe report. It's like the bad level never happened!

Now, the same rules apply as before, except if removing a single level from an unsafe report would make it safe, the report instead counts as safe.

More of the above example's reports are now safe:

    7 6 4 2 1: Safe without removing any level.
    1 2 7 8 9: Unsafe regardless of which level is removed.
    9 7 6 2 1: Unsafe regardless of which level is removed.
    1 3 2 4 5: Safe by removing the second level, 3.
    8 6 4 4 1: Safe by removing the third level, 4.
    1 3 6 7 9: Safe without removing any level.

Thanks to the Problem Dampener, 4 reports are actually safe!

Update your analysis by handling situations where the Problem Dampener can remove a single level from unsafe reports. How many reports are now safe?

 */

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
        while (getline(file, line)) {
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
            for (int current; iss >> current;) {
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

#pragma once
#include <vector>
#include <string>

enum RuleState {
    NOT_PROCESSED,
    PROCESSING,
    PROCESSED
};

struct Rule {
    int x;
    int y;
    RuleState state = NOT_PROCESSED;
};
std::vector<Rule> parse_rules(const std::string& path);
std::vector<int> compute_rule_ordering(std::vector<Rule>& rules);
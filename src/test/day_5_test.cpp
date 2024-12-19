#include <catch2/catch_test_macros.hpp>
#include <day_5.hpp>

TEST_CASE("Rules are parsed correctly") {
    std::vector<Rule> rules = parse_rules("../../input/day_5_example_rules.txt");
    std::vector<int> expectedResult = {97, 75, 47, 61, 53, 29, 13};
    REQUIRE(compute_rule_ordering(rules) == expectedResult);
}
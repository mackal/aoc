#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <scn/scan.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string_view> split_line(std::string_view line, char delimiter)
{
  std::vector<std::string_view> ret;
  std::string_view::size_type start = 0;
  auto end = line.find(delimiter);
  while (end != std::string_view::npos) {
    ret.emplace_back(line.substr(start, end - start));
    start = end + 1;
    end = line.find(delimiter, start);
  }
  if (line.length() > start) {
    ret.emplace_back(line.substr(start, line.length() - start));
  }

  return ret;
}

std::unordered_map<int, std::vector<int>> parse_rules(std::stringstream& file)
{
  std::unordered_map<int, std::vector<int>> ret;

  std::string line;

  while (std::getline(file, line)) {
    if (line.empty()) {
      break;
    }

    auto result = scn::scan<int, int>(line, "{}|{}");
    if (result) {
      auto [index, data] = result->values();
      auto& vec = ret[index];
      vec.push_back(data);
    }
  }

  return ret;
}

std::vector<std::vector<int>> parse_pages(std::stringstream& file)
{
  std::vector<std::vector<int>> ret;

  std::string line;
  while (std::getline(file, line)) {
    auto split = split_line(line, ',');
    std::vector<int> tmp;
    for (auto& it : split) {
      int result{};
      auto [ptr, ec] = std::from_chars(it.data(), it.data() + it.size(), result);
      if (ec == std::errc()) {
        tmp.push_back(result);
      }
    }
    ret.push_back(tmp);
  }

  return ret;
}

bool passes_rules(const std::unordered_map<int, std::vector<int>>& rules, const std::vector<int>& pages)
{
  for (auto it = pages.begin(); it != pages.end(); ++it) {
    auto rule = rules.find(*it);
    if (rule != rules.end()) {
      for (auto jt : rule->second) {
        auto found = std::ranges::find(pages, jt);
        if (std::distance(it, found) < 0) {
          return false;
        }
      }
    }
  }

  return true;
}

bool passes_rules_fixed(const std::unordered_map<int, std::vector<int>>& rules, std::vector<int>& pages)
{
  bool ret = false;
  while (!passes_rules(rules, pages)) {
    for (auto it = pages.begin(); it != pages.end(); ++it) {
      auto rule = rules.find(*it);
      if (rule != rules.end()) {
        for (auto jt : rule->second) {
          auto found = std::ranges::find(pages, jt);
          if (std::distance(it, found) < 0) {
            std::swap(*it, *found);
            ret = true;
          }
        }
      }
    }
  }

  return ret;
}

int calculate_middle_sum(const std::unordered_map<int, std::vector<int>>& rules,
                         const std::vector<std::vector<int>>& pages)
{
  int total = 0;

  for (auto& it : pages) {
    if (passes_rules(rules, it)) {
      total += it[it.size() / 2];
    }
  }

  return total;
}

int calculate_fixed_middle_sum(const std::unordered_map<int, std::vector<int>>& rules,
                               std::vector<std::vector<int>>& pages)
{
  int total = 0;

  for (auto& it : pages) {
    if (passes_rules_fixed(rules, it)) {
      total += it[it.size() / 2];
      if (!passes_rules(rules, it))
        std::cout << "Still broken!" << std::endl;
    }
  }

  return total;
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "Please provide a filename" << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);

  if (!file) {
    std::cout << "Could not read file: " << argv[1] << std::endl;
    return 1;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  //    std::string test =
  //    "47|53\n97|13\n97|61\n97|47\n75|29\n61|13\n75|53\n29|13\n97|29\n53|29\n61|53\n97|53\n61|29\n47|13\n75|47\n97|75\n47|61\n75|61\n47|29\n75|13\n53|13\n\n75,47,61,53,29\n97,61,53,29,13\n75,29,13\n75,97,47,61,53\n61,13,29\n97,13,75,29,47\n";
  //    buffer.str(test);

  auto rules = parse_rules(buffer);
  auto pages = parse_pages(buffer);

  std::cout << "Middle sum " << calculate_middle_sum(rules, pages) << std::endl;
  std::cout << "Middle fixed sum " << calculate_fixed_middle_sum(rules, pages) << std::endl;

  return 0;
}

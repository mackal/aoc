#include <charconv>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> split_line(std::stringstream& line)
{
  std::vector<int> ret;

  for (std::string i; std::getline(line, i, ' ');) {
    int res;
    auto [ptr, ec] = std::from_chars(i.data(), i.data() + i.size(), res);
    if (ec != std::errc())
      break;
    ret.push_back(res);
  }

  return ret;
}

// return index that is bad
bool check_safe(std::vector<int>& report)
{
  // all increasing or all decreasing
  // all adjacent levels are at least 1 and at most 3 difference

  int last = report[0];
  bool increasing = report[0] < report[1]; // which direction we're checking
  for (int i = 1; i < report.size(); ++i) {
    int difference = std::abs(last - report[i]);
    if (difference == 0 || difference > 3) {
      return false;
    }

    if (report[i] > last) {
      if (!increasing) {
        return false;
      }
      last = report[i];
    } else if (report[i] < last) {
      if (increasing) {
        return false;
      }
      last = report[i];
    }
  }

  return true;
}

int calculate_safe(std::stringstream& file)
{
  int safe = 0;

  std::string line;
  while (std::getline(file, line)) {
    std::stringstream str(line);
    auto split = split_line(str);

    if (split.size() == 1) {
      ++safe;
      continue;
    }

    if (check_safe(split)) {
      ++safe;
    } else {
      // try to dampener
      for (int i = 0; i < split.size(); ++i) {
        auto report_copy = split;
        report_copy.erase(report_copy.begin() + i);
        if (check_safe(report_copy)) {
          ++safe;
          break;
        }
      }
    }
  }

  return safe;
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

  //    std::string test = "7 6 4 2 1\n1 2 7 8 9\n9 7 6 2 1\n1 3 2 4 5\n8 6 4 4 1\n1 3 6 7 9";
  //    buffer.str(test);

  std::cout << "Safe: " << calculate_safe(buffer) << std::endl;

  return 0;
}

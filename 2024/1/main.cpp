#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <scn/scan.h>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

std::tuple<std::vector<int>, std::vector<int>> parse_data(std::stringstream& file)
{
  std::tuple<std::vector<int>, std::vector<int>> ret;
  auto& tleft = std::get<0>(ret);
  auto& tright = std::get<1>(ret);

  std::string line;

  while (std::getline(file, line)) {
    auto result = scn::scan<int, int>(line, "{}   {}");
    if (result) {
      auto [left, right] = result->values();
      tleft.push_back(left);
      tright.push_back(right);
    }
  }

  return ret;
}

int calculate_similarity(std::vector<int>& left, std::vector<int>& right)
{
  int total = 0;
  for (auto it : left) {
    int count = std::count(right.begin(), right.end(), it);
    total += it * count;
  }
  return total;
}

int calculate_distance(std::vector<int>& left, std::vector<int>& right)
{
  int total = 0;

  std::ranges::sort(left);
  std::ranges::sort(right);

  if (left.size() == right.size()) {
    for (int i = 0; i < left.size(); ++i) {
      total += std::abs(right[i] - left[i]);
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

  auto [left, right] = parse_data(buffer);
  //    std::vector<int> left{3, 4, 2, 1, 3, 3};
  //    std::vector<int> right{4, 3, 5, 3, 9, 3};
  /*
   * Similarity is: 22014209
   * Distance is: 1938424
   */
  std::cout << "Similarity is: " << calculate_similarity(left, right) << std::endl;

  std::cout << "Distance is: " << calculate_distance(left, right) << std::endl;
  return 0;
}

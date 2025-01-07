#include <charconv>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <system_error>
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

struct PuzzleData {
  uint64_t sum;
  std::vector<uint64_t> nums;
};

std::vector<PuzzleData> ParseData(std::stringstream& file)
{
  std::vector<PuzzleData> data;

  std::string line;

  while (std::getline(file, line)) {
    PuzzleData tmp;
    auto [ptr, ec] = std::from_chars(line.data(), line.data() + line.size(), tmp.sum);
    ptr += 2;

    auto split = split_line(ptr, ' ');

    for (auto& sv : split) {
      uint64_t num = 0;
      auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), num);
      if (ec == std::errc())
        tmp.nums.push_back(num);
    }
    data.push_back(tmp);
  }

  return data;
}

// fuck it
uint64_t concat(uint64_t a, uint64_t b)
{
  std::stringstream ss;
  ss << a << b;
  auto tmp = ss.str();
  uint64_t res;
  std::from_chars(tmp.data(), tmp.data() + tmp.size(), res);
  return res;
}

std::vector<std::vector<char>> cartesian(std::span<char> iterables, int repeat = 1)
{
  std::vector<std::vector<char>> ret(1, std::vector<char>());

  std::vector<std::span<char>> pools;

  while (repeat--)
    pools.push_back(iterables);

  for (int i = 0; i < pools.size(); ++i) {
    std::vector<std::vector<char>> temp;
    for (const auto& product : ret) {
      for (const auto& c : pools[i]) {
        auto temp2 = product;
        temp2.push_back(c);
        temp.push_back(temp2);
      }
    }
    ret = temp;
  }

  return ret;
}

uint64_t DoOperations(std::span<char> ops, std::vector<uint64_t>& values)
{
  auto sum = values[0];
  for (int i = 0; i < values.size() - 1; ++i) {
    auto num = values[i + 1];
    auto op = ops[i];
    switch (op) {
    case '+':
      sum += num;
      break;
    case '*':
      sum *= num;
      break;
    case '|':
      sum = concat(sum, num);
      break;
    default:
      return -1;
    }
  }

  return sum;
}

bool Solve(PuzzleData& data, bool part_two = false)
{
  std::vector<char> its{'+', '*'};
  if (part_two)
    its.push_back('|');

  for (auto ops : cartesian(its, data.nums.size() - 1)) {
    if (DoOperations(ops, data.nums) == data.sum) {
      return true;
    }
  }
  return false;
}

uint64_t PartOneTotal(std::vector<PuzzleData>& data)
{
  uint64_t total = 0;

  for (auto& puzzle : data) {
    if (Solve(puzzle)) {
      total += puzzle.sum;
    }
  }

  return total;
}

uint64_t PartTwoTotal(std::vector<PuzzleData>& data)
{
  uint64_t total = 0;

  for (auto& puzzle : data) {
    if (Solve(puzzle, true)) {
      total += puzzle.sum;
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

  std::stringstream buffer;
  if (argv[1] != std::string("test")) {
    std::ifstream file(argv[1]);

    if (!file) {
      std::cout << "Could not read file: " << argv[1] << std::endl;
      return 1;
    }

    buffer << file.rdbuf();
    file.close();
  } else {
    std::string test = "190: 10 19\n3267: 81 40 27\n83: 17 5\n156: 15 6\n7290: 6 8 6 15\n161011: 16 10 13\n192: 17 8 "
                       "14\n21037: 9 7 18 13\n292: 11 6 16 20";
    buffer.str(test);
  }

  auto data = ParseData(buffer);

  std::cout << "Part 1 " << PartOneTotal(data) << std::endl;
  std::cout << "Part 2 " << PartTwoTotal(data) << std::endl;

  return 0;
}

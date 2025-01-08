#include <charconv>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace utility
{
template <typename T> std::enable_if_t<std::is_arithmetic_v<T>, T> count_digits(T v)
{
  T count = 0;

  while (v != 0) {
    v /= 10;
    ++count;
  }

  return count;
}
}; // namespace utility

std::vector<int64_t> load_puzzle(std::stringstream& stream)
{
  std::vector<int64_t> ret;

  std::string line;
  while (std::getline(stream, line)) {
    const char* ptr = line.data();
    do {
      int64_t value{};
      auto res = std::from_chars(ptr, line.data() + line.size(), value);
      if (res.ec == std::errc())
        ret.push_back(value);
      else
        break;
      ptr = res.ptr + 1; // skip space

    } while (ptr < line.data() + line.size());
  }

  return ret;
}

std::vector<int64_t> process_blink(std::vector<int64_t>& data)
{
  std::vector<int64_t> ret;

  for (auto i : data) {
    if (i == 0) {
      ret.push_back(1);
    } else if ((utility::count_digits(i) % 2) == 0) {
      // split
      auto str = std::to_string(i);
      auto mid = str.size() / 2;
      int64_t value{};
      auto res = std::from_chars(str.data(), str.data() + mid, value);
      ret.push_back(value);
      res = std::from_chars(str.data() + mid, str.data() + str.size(), value);
      ret.push_back(value);
    } else {
      ret.push_back(i * 2024);
    }
  }

  return ret;
}

uint64_t process_blink2(std::vector<int64_t>& data)
{
  // order doesn't actually matter! map of stone value to count of them
  std::unordered_map<int64_t, uint64_t> results;
  uint64_t total = 0;

  for (auto v : data) {
    results[v]++;
  }

  for (int i = 0; i < 50; ++i) {
    // tmp list for new stones, which we shouldn't process this loop
    // we remove modified stones
    std::unordered_map<int64_t, uint64_t> tmp;
    for (auto [k, v] : results) {
      if (v == 0) // no stones, probably don't need this, but save time
        continue;
      if (k == 0) {
        tmp[1] += v;
      } else if ((utility::count_digits(k) % 2) == 0) {
        auto str = std::to_string(k);
        auto mid = str.size() / 2;
        int64_t stone1{}, stone2{};
        auto res = std::from_chars(str.data(), str.data() + mid, stone1);
        res = std::from_chars(str.data() + mid, str.data() + str.size(), stone2);
        tmp[stone1] += v;
        tmp[stone2] += v;
      } else {
        tmp[k * 2024] += v;
      }
      results[k] = 0;
    }

    // now we merge the tmp list
    for (auto [k, v] : tmp) {
      results[k] += v;
    }
  }

  for (auto [k, v] : results)
    total += v;

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
    // std::string test = "0 1 10 99 999";
    std::string test = "125 17";
    buffer.str(test);
  }

  auto data = load_puzzle(buffer);

  for (int i = 0; i < 25; ++i) {
    data = process_blink(data);
  }

  std::cout << "Part 1: " << data.size() << " stones" << std::endl;

  std::cout << "Part 2: " << process_blink2(data) << " stones" << std::endl;

  return 0;
}

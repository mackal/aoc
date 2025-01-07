#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

const int64_t EMPTY_FILE = -1;

std::string load_compression(std::stringstream& stream)
{
  std::string line;
  std::getline(stream, line);

  return line;
}

std::vector<int64_t> decompress(std::string_view sv)
{
  std::vector<int64_t> ret;

  int64_t id = 0;
  uint64_t loop = 0;

  for (auto c : sv) {
    int count = static_cast<int64_t>(c) - 0x30; // this works for single character numbers
    auto value = (loop++ % 2) == 0 ? id++ : EMPTY_FILE;
    for (int i = 0; i < count; ++i) {
      ret.push_back(value);
    }
  }

  return ret;
}

void compact(std::vector<int64_t>& fs)
{
  auto free = std::ranges::find(fs, EMPTY_FILE);
  auto data = std::find_if_not(fs.rbegin(), fs.rend(), [](int64_t x) { return x == EMPTY_FILE; });

  // gross
  auto free_dist = std::distance(fs.begin(), free);
  auto data_dist = fs.size() - std::distance(fs.rbegin(), data);

  if (free_dist > data_dist)
    return;

  do {
    std::swap(*free, *data);
    free = std::ranges::find(fs, EMPTY_FILE);
    data = std::find_if_not(fs.rbegin(), fs.rend(), [](int64_t x) -> bool { return x == EMPTY_FILE; });
    free_dist = std::distance(fs.begin(), free);
    data_dist = fs.size() - std::distance(fs.rbegin(), data);
  } while (free_dist < data_dist);

  return;
}

void print_array(std::vector<int64_t>& fs)
{
  for (auto it : fs) {
    if (it == EMPTY_FILE)
      std::cout << '.';
    else
      std::cout << it;
  }
  std::cout << std::endl;
}

void compat_defrag(std::vector<int64_t>& fs)
{
  std::set<int64_t> checked;
  std::map<size_t, int> empty;

  bool found = false;
  size_t cur = 0;
  for (auto it = fs.begin(); it != fs.end(); ++it) {
    if (*it != EMPTY_FILE) {
      found = false;
      continue;
    }
    if (!found) {
      found = true;
      cur = std::distance(fs.begin(), it);
    }
    empty[cur]++;
  }

  // print_array(fs);

  for (int i = fs.size() - 1; i >= 0; --i) {
    auto value = fs[i];
    if (value == EMPTY_FILE)
      continue;

    if (checked.contains(value))
      continue;

    int count = 1;
    auto x = i - 1;

    while (fs[x] == value) {
      count++;
      x--;
    }

    for (auto [k, v] : empty) {
      if (k >= i) // don't move from before us ... should probably break but w/e
        continue;
      // check for space
      if (v >= count) {
        for (int j = 0; j < count; ++j) {
          std::swap(fs[k + j], fs[i - j]);
        }

        // print_array(fs);
        //  need to update the start position of the free space and count
        v -= count;
        if (v) { // only add new if there is still space
          size_t new_k = k + count;
          empty[new_k] = v;
        }
        empty.erase(k);
        break;
      }
    }
    checked.insert(value);
  }
}

uint64_t checksum(std::vector<int64_t>& fs)
{
  uint64_t sum = 0;
  for (uint64_t i = 0; i < fs.size(); ++i) {
    if (fs[i] == EMPTY_FILE)
      continue;
    sum += i * fs[i];
  }

  return sum;
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
    std::string test = "2333133121414131402";
    buffer.str(test);
  }

  auto data = load_compression(buffer);

  auto decomp = decompress(data);

  auto decomp_copy = decomp;

  compact(decomp);

  std::cout << "Part 1: " << checksum(decomp) << std::endl;

  compat_defrag(decomp_copy);

  std::cout << "Part 2: " << checksum(decomp_copy) << std::endl;

  return 0;
}

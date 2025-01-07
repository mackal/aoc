#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::vector<std::vector<char>> load_map(std::stringstream& stream)
{
  std::vector<std::vector<char>> ret;

  std::string line;
  while (std::getline(stream, line)) {
    std::vector<char> vec;
    std::copy(line.begin(), line.end(), std::back_inserter(vec));
    ret.push_back(vec);
  }

  return ret;
}

int FindAntinodes(const std::vector<std::vector<char>>& map, bool part_two = false)
{
  std::set<std::pair<int, int>> antinodes;
  std::multimap<char, std::pair<int, int>> antennas;

  const int y_size = map.size();
  const int x_size = map[0].size();

  for (auto lt = map.begin(); lt != map.end(); ++lt) {
    auto& l = *lt;
    for (auto ft = l.begin(); ft != l.end(); ++ft) {
      if (*ft != '.') {
        int y1 = std::distance(map.begin(), lt);
        int x1 = std::distance(l.begin(), ft);
        antennas.emplace(*ft, std::make_pair(y1, x1));
        //                auto& k = antennas[*ft];
        //                k.insert(std::make_pair(y1, x1));
        for (auto lt2 = map.begin(); lt2 != map.end(); ++lt2) {
          auto& l2 = *lt2;
          for (auto ft2 = l2.begin(); ft2 != l2.end(); ++ft2) {
            if (*ft2 != '.' && *ft == *ft2) {
              int y2 = std::distance(map.begin(), lt2);
              int x2 = std::distance(l2.begin(), ft2);
              // not the same antenna
              if (y1 != y2 || x1 != x2) {
                int y_diff = y2 - y1;
                int x_diff = x2 - x1;
                // we are going to create the antinode on the other side of the new one we found
                int new_y = y2 + y_diff;
                int new_x = x2 + x_diff;
                // we're on the map still
                if (new_y >= 0 && new_y < y_size && new_x >= 0 && new_x < x_size) {
                  do {
                    antinodes.insert(std::make_pair(new_y, new_x));
                    new_y += y_diff;
                    new_x += x_diff;
                  } while (part_two && new_y >= 0 && new_y < y_size && new_x >= 0 && new_x < x_size);
                }
              }
            }
          }
        }
      }
    }
  }

  if (part_two) {
    for (auto it : antennas) {
      auto range = antennas.equal_range(it.first);
      if (std::distance(range.first, range.second) > 1) {
        for (auto i = range.first; i != range.second; ++i) {
          antinodes.insert(i->second);
        }
      }
    }
  }

  return antinodes.size();
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
    std::string test = "............\n"
                       "........0...\n"
                       ".....0......\n"
                       ".......0....\n"
                       "....0.......\n"
                       "......A.....\n"
                       "............\n"
                       "............\n"
                       "........A...\n"
                       ".........A..\n"
                       "............\n"
                       "............";
    buffer.str(test);
  }

  auto map = load_map(buffer);

  std::cout << "Part 1: " << FindAntinodes(map) << std::endl;
  std::cout << "Part 2: " << FindAntinodes(map, true) << std::endl;

  return 0;
}

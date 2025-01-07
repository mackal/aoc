#include <fstream>
#include <iostream>
#include <queue>
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
    ret.emplace_back(vec);
  }

  return ret;
}

int total_trailhead_scores(const std::vector<std::vector<char>>& map, bool part_two = false)
{
  int total = 0;
  const auto x_size = map[0].size();
  const auto y_size = map.size();

  // directions we are checking
  const std::vector<std::pair<int, int>> directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

  // queue for valid points for processing
  std::queue<std::pair<int, int>> queue;

  for (int i = 0; i < y_size; ++i) {   // row
    for (int j = 0; j < x_size; ++j) { // column
      if (map[i][j] == '0') {
        // all of the 9's we've visited
        std::set<std::pair<int, int>> visited;
        // now we try to find a path
        queue.emplace(i, j);

        while (!queue.empty()) {
          auto [r, c] = queue.front();
          queue.pop();

          // check if we found a 9 we haven't seen
          if (map[r][c] == '9' && (part_two || !visited.contains({r, c}))) {
            ++total;
            visited.emplace(r, c);
          } else {
            // now we traverse the rest of the directions
            for (auto& dir : directions) {
              auto [new_r, new_c] = dir;
              new_r += r;
              new_c += c;
              if (new_r >= 0 && new_r < y_size && new_c >= 0 && new_c < x_size && map[new_r][new_c] == map[r][c] + 1) {
                queue.emplace(new_r, new_c);
              }
            }
          }
        }
      }
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
    std::string test = "89010123\n"
                       "78121874\n"
                       "87430965\n"
                       "96549874\n"
                       "45678903\n"
                       "32019012\n"
                       "01329801\n"
                       "10456732";
    buffer.str(test);
  }

  auto map = load_map(buffer);
  std::cout << "Part one " << total_trailhead_scores(map) << std::endl;
  std::cout << "Part two " << total_trailhead_scores(map, true) << std::endl;

  return 0;
}

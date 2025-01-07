#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

std::vector<std::vector<char>> g_map;

enum class Direction {
  North,
  East,
  South,
  West,
};

class Guard
{
public:
  Guard() = default;
  ~Guard() = default;

  inline void UpdatePosition(int x, int y)
  {
    m_x = x;
    m_y = y;
  }
  inline void SetOrigin(int x, int y)
  {
    m_origin_x = x;
    m_origin_y = y;
  }
  inline void UpdateDirection(Direction dir) { m_dir = dir; }
  inline void Turn()
  {
    switch (m_dir) {
    case Direction::North:
      m_dir = Direction::East;
      break;
    case Direction::East:
      m_dir = Direction::South;
      break;
    case Direction::South:
      m_dir = Direction::West;
      break;
    case Direction::West:
      m_dir = Direction::North;
      break;
    }
  }
  inline int GetX() { return m_x; }
  inline int GetY() { return m_y; }
  inline int GetOriginX() { return m_origin_x; }
  inline int GetOriginY() { return m_origin_y; }
  inline void UpdateMemory(int x, int y) { m_memory.insert(std::make_pair(x, y)); }
  inline bool GetVisitStatus(int x, int y) { return m_memory.contains(std::make_pair(x, y)); }
  inline int GetDistinct() { return m_memory.size(); }
  bool ProcessMove();
  int ProcessMoveTwo();
  int ProcessPartTwo();

private:
  int m_x = 0;
  int m_y = 0;
  int m_origin_x = 0;
  int m_origin_y = 0;
  Direction m_dir = Direction::North;
  std::set<std::pair<int, int>> m_memory;
  std::set<std::tuple<int, int, Direction>> m_memory_direction;
};

bool Guard::ProcessMove()
{
  int new_x = 0, new_y = 0;
  switch (m_dir) {
  case Direction::North:
    new_x = m_x;
    new_y = m_y - 1;
    break;
  case Direction::East:
    new_x = m_x + 1;
    new_y = m_y;
    break;
  case Direction::South:
    new_x = m_x;
    new_y = m_y + 1;
    break;
  case Direction::West:
    new_x = m_x - 1;
    new_y = m_y;
    break;
  }

  if (!GetVisitStatus(m_x, m_y)) {
    UpdateMemory(m_x, m_y);
  }

  if (new_x < 0 || new_x >= g_map[0].size() || new_y < 0 || new_y >= g_map.size())
    return false;

  if (g_map[new_y][new_x] == '#') {
    Turn();
    return true;
  }

  UpdatePosition(new_x, new_y);

  return true;
}

// ret values: 0 -- keep processing
//             1 -- found looop
//             -1 -- off map
int Guard::ProcessMoveTwo()
{
  int new_x = 0, new_y = 0;
  switch (m_dir) {
  case Direction::North:
    new_x = m_x;
    new_y = m_y - 1;
    break;
  case Direction::East:
    new_x = m_x + 1;
    new_y = m_y;
    break;
  case Direction::South:
    new_x = m_x;
    new_y = m_y + 1;
    break;
  case Direction::West:
    new_x = m_x - 1;
    new_y = m_y;
    break;
  }

  auto tmp = std::make_tuple(m_x, m_y, m_dir);
  if (m_memory_direction.contains(tmp)) {
    return 1;
  } else {
    m_memory_direction.insert(tmp);
  }

  if (new_x < 0 || new_x >= g_map[0].size() || new_y < 0 || new_y >= g_map.size())
    return -1;

  if (g_map[new_y][new_x] == '#') {
    Turn();
    return 0;
  }

  UpdatePosition(new_x, new_y);

  return 0;
}

int Guard::ProcessPartTwo()
{
  int total = 0;
  // first entry is starting, which we're not allowed to place one at
  m_memory.erase(m_memory.begin());

  for (auto& p : m_memory) {
    m_memory_direction.clear();
    auto [obj_x, obj_y] = p;
    g_map[obj_y][obj_x] = '#';

    int res = 0;
    do {
      res = ProcessMoveTwo();
    } while (res == 0);

    if (res == 1) {
      total++;
    }

    g_map[obj_y][obj_x] = '.';
    UpdatePosition(GetOriginX(), GetOriginY());
    UpdateDirection(Direction::North);
  }

  return total;
}

void load_map(std::stringstream& stream, Guard& guard)
{
  g_map.clear();

  std::string line;
  while (std::getline(stream, line)) {
    std::vector<char> vec;
    std::copy(line.begin(), line.end(), std::back_inserter(vec));
    auto g = line.find('^');
    if (g != std::string::npos) {
      guard.UpdatePosition(g, g_map.size());
      guard.SetOrigin(g, g_map.size());
    }
    g_map.push_back(vec);
  }
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
  //    "....#.....\n.........#\n..........\n..#.......\n.......#..\n..........\n.#..^.....\n........#.\n#.........\n......#...";
  //    buffer.str(test);

  Guard g;

  load_map(buffer, g);

  if (g_map.empty())
    return 1;

  while (g.ProcessMove()) {
  }

  std::cout << "Distinct " << g.GetDistinct() << std::endl;

  g.UpdatePosition(g.GetOriginX(), g.GetOriginY());
  g.UpdateDirection(Direction::North);

  auto loops = g.ProcessPartTwo();

  std::cout << "Loops " << loops << std::endl;

  return 0;
}

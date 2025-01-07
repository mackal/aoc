#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<char>> parse_file(std::stringstream& stream)
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

int search_word(std::vector<std::vector<char>>& data)
{
  int total = 0;

  if (data.empty()) {
    return 0;
  }

  std::string search = "XMAS";

  const int rows = data.size();
  const int columns = data.at(0).size();

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (data[i][j] == search[0]) {
        // search to right
        if (j + 3 < columns && data[i][j + 1] == search[1]) {
          if (data[i][j + 2] == search[2] && data[i][j + 3] == search[3]) {
            total++;
          }
        }
        // search left
        if (j - 3 >= 0 && data[i][j - 1] == search[1]) {
          if (data[i][j - 2] == search[2] && data[i][j - 3] == search[3]) {
            total++;
          }
        }
        // search up
        if (i - 3 >= 0 && data[i - 1][j] == search[1]) {
          if (data[i - 2][j] == search[2] && data[i - 3][j] == search[3]) {
            total++;
          }
        }
        // search down
        if (i + 3 < rows && data[i + 1][j] == search[1]) {
          if (data[i + 2][j] == search[2] && data[i + 3][j] == search[3]) {
            total++;
          }
        }
        // search down and right
        if (j + 3 < columns && i + 3 < rows && data[i + 1][j + 1] == search[1]) {
          if (data[i + 2][j + 2] == search[2] && data[i + 3][j + 3] == search[3]) {
            total++;
          }
        }
        // search down and left
        if (j - 3 >= 0 && i + 3 < rows && data[i + 1][j - 1] == search[1]) {
          if (data[i + 2][j - 2] == search[2] && data[i + 3][j - 3] == search[3]) {
            total++;
          }
        }
        // search up and right
        if (j + 3 < columns && i - 3 >= 0 && data[i - 1][j + 1] == search[1]) {
          if (data[i - 2][j + 2] == search[2] && data[i - 3][j + 3] == search[3]) {
            total++;
          }
        }
        // search up and left
        if (j - 3 >= 0 && i - 3 >= 0 && data[i - 1][j - 1] == search[1]) {
          if (data[i - 2][j - 2] == search[2] && data[i - 3][j - 3] == search[3]) {
            total++;
          }
        }
      }
    }
  }

  return total;
}

int search_xmas(std::vector<std::vector<char>>& data)
{
  int total = 0;

  if (data.empty()) {
    return 0;
  }

  // searching for A's
  const int rows = data.size() - 1;
  const int columns = data.at(0).size() - 1;

  for (int i = 1; i < rows; ++i) {
    for (int j = 1; j < columns; ++j) {
      if (data[i][j] == 'A') {
        if (data[i - 1][j - 1] == 'M' && data[i + 1][j + 1] == 'S') {
          if (data[i + 1][j - 1] == 'M' && data[i - 1][j + 1] == 'S') {
            total++;
          }
          if (data[i + 1][j - 1] == 'S' && data[i - 1][j + 1] == 'M') {
            total++;
          }
        } else if (data[i - 1][j - 1] == 'S' && data[i + 1][j + 1] == 'M') {
          if (data[i + 1][j - 1] == 'M' && data[i - 1][j + 1] == 'S') {
            total++;
          }
          if (data[i + 1][j - 1] == 'S' && data[i - 1][j + 1] == 'M') {
            total++;
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

  std::ifstream file(argv[1]);

  if (!file) {
    std::cout << "Could not read file: " << argv[1] << std::endl;
    return 1;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  //    std::string test =
  //    "MMMSXXMASM\nMSAMXMSMSA\nAMXSXMAAMM\nMSAMASMSMX\nXMASAMXAMM\nXXAMMXXAMA\nSMSMSASXSS\nSAXAMASAAA\nMAMMMXMMMM\nMXMXAXMASX";
  //    buffer.str(test);

  auto data = parse_file(buffer);

  std::cout << "Total Part 1: " << search_word(data) << std::endl;
  std::cout << "Total Part 2: " << search_xmas(data) << std::endl;

  return 0;
}

#include <fstream>
#include <iostream>
#include <scn/scan.h>
#include <sstream>
#include <string_view>

int sum_multiplications(std::string_view sv)
{
  int sum = 0;
  int pos = 0;
  bool enabled = true;

  while (pos != sv.npos && pos < sv.size()) {
    if (enabled) {
      auto dont = sv.find("don't()", pos);
      auto start = sv.find("mul(", pos);
      if (start == sv.npos)
        break;

      if (start > dont) {
        enabled = false;
        pos = dont + 7;
        continue;
      }

      auto end = sv.find(")", start + 4);
      if (end == sv.npos)
        break;

      auto first_bad = sv.find_first_not_of("mul(),0123456789", start + 4);

      if (first_bad != sv.npos && first_bad < end) {
        pos += 4;
        continue;
      }

      auto instruction = sv.substr(start, end - start + 1);

      auto result = scn::scan<int, int>(instruction, "mul({},{})");
      if (result) {
        auto [first, second] = result->values();
        sum += (first * second);
      }

      pos = end + 1;
    } else {
      auto do_ = sv.find("do()", pos);
      if (do_ == sv.npos)
        break;
      pos = do_ + 4;
      enabled = true;
    }
  }

  return sum;
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

  //    std::string test = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";
  //    buffer.str(test);
  //    part two test
  //    std::string test = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";
  //    buffer.str(test);

  std::cout << "Sum: " << sum_multiplications(buffer.str()) << std::endl;

  return 0;
}

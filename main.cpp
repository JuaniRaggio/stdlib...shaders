#include <cstdint>
#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>

#define UNABLE_TO_OPEN_FILE 1
#define FACTOR 60
#define MAX_VALUE 255

int main(int argc, char *argv[]) {

  std::string const file_name("out.ppm");

  std::ofstream output(file_name, std::ios::out);

  if (!output.is_open()) {
    std::cerr << std::format("Unable to open file {}", file_name);
    return UNABLE_TO_OPEN_FILE;
  }

  std::uint16_t width{16 * FACTOR};
  std::uint16_t height{9 * FACTOR};
  std::ostringstream oss;
  oss << "P6\n"
      << std::format("%d %d\n", width, height)
      << std::format("%d\n", MAX_VALUE);

  for (uint64_t y {0}; y < height; ++y) {
    for (uint64_t x {0}; x < width; ++x) {
      oss << 0xFF << 0x00 << 0x00;
    }
  }
  output << oss.str();
  output.close();
  std::cout << std::format("Generated {}\n", file_name);

  return 0;
}

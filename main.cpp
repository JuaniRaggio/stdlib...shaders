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

  for (uint8_t i{0}; i < FACTOR; ++i) {

    std::string file_path = std::format("frames/out{}.ppm", i);

    std::ofstream output(file_path, std::ios::out);

    if (!output.is_open()) {
      std::cerr << std::format("Unable to open file {}", file_path);
      return UNABLE_TO_OPEN_FILE;
    }

    std::uint16_t width{16 * FACTOR};
    std::uint16_t height{9 * FACTOR};
    std::ostringstream oss;
    oss << "P6\n"
        << std::format("{} {}\n", width, height)
        << std::format("{}\n", MAX_VALUE);

    for (uint64_t y{0}; y < height; ++y) {
      for (uint64_t x{0}; x < width; ++x) {
        if (((x + i) / 60 + (y + i) / 60) % 2) {
          // This is like R G B
          oss << 0xFF << 0x00 << 0x00;
        } else {
          oss << 0x00 << 0x00 << 0x00;
        }
      }
    }
    output << oss.str();
    oss.clear();
    output.close();
    std::cout << std::format("Generated {}\n", file_path);
  }

  return 0;
}

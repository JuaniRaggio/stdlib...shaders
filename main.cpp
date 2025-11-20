#include <cstdint>
#include <cstdlib>
#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>

#define UNABLE_TO_OPEN_FILE 1
#define FACTOR 60
#define MAX_VALUE 255

typedef struct vec2 {
  float x, y;
  vec2 yx() const {
    return vec2(y, x);
  }
  vec4 xyyx() const {
    return vec4(x, y, y, x);
  }
} vec2;


vec2 operator*(const vec2 &a, float s) {
  return {
      .x = a.x * s,
      .y = a.y * s,
  };
}

vec2 operator+(const vec2 &a, float s) {
  return {
      .x = a.x + s,
      .y = a.y + s,
  };
}

vec2 operator*(float s, const vec2 &a) { return a * s; }

vec2 operator-(const vec2 &a, const vec2 &b) {
  return {
      .x = a.x - b.x,
      .y = a.y - b.y,
  };
}

vec2 operator+(const vec2 &a, const vec2 &b) {
  return {
      .x = a.x + b.x,
      .y = a.y + b.y,
  };
}

vec2 operator*(const vec2 &a, const vec2 &b) {
  return {
      .x = a.x * b.x,
      .y = a.y * b.y,
  };
}

vec2 operator/(const vec2 &a, float s) {
  return {
      .x = a.x / s,
      .y = a.y / s,
  };
}

float dot(const vec2 &a, const vec2 &b) { return a.x * b.x + a.y * b.y; }

vec2 abs(const vec2 &a) {
  return {
      .x = fabsf(a.x),
      .y = fabsf(a.y),
  };
}

vec2 &operator+=(vec2 &a, const vec2 &b) {
  a = a + b;
  return a;
}

vec2 &operator+=(vec2 &a, float s) {
  a = a + s;
  return a;
}

vec2 cos(const vec2 &a) {
  return {
      .x = cosf(a.x),
      .y = cosf(a.y),
  };
}

vec2 sin(const vec4 &a) {
  return {
      .x = cosf(a.x),
      .y = cosf(a.y),
  };
}


int main(int argc, char *argv[]) {

  for (uint8_t i{0}; i < FACTOR; ++i) {

    std::string file_path = std::format("frames/out{}.ppm", i);

    std::ofstream output(file_path, std::ios::out);

    if (!output.is_open()) {
      std::cerr << std::format("Unable to open file {}", file_path);
      return UNABLE_TO_OPEN_FILE;
    }

    const std::uint16_t width{16 * FACTOR};
    const std::uint16_t height{9 * FACTOR};
    const vec2 r = {
        .x = (float)width,
        .y = (float)height,
    };
    std::ostringstream oss;
    oss << "P6\n"
        << std::format("{} {}\n", width, height)
        << std::format("{}\n", MAX_VALUE);
    float t = (float)i / 60;

    for (uint64_t y{0}; y < height; ++y) {
      for (uint64_t x{0}; x < width; ++x) {
        vec4 o;
        vec2 FC = {
            .x = (float)x,
            .y = (float)y,
        };
        vec2 p = (FC * 2. - r) / r.y, l,
             v = p * (1. - (l += abs(.7 - dot(p, p)))) / .2;
        for (float i; i++ < 8.; o += (sin(v.xyyx) + 1.) * abs(v.x - v.y) * .2)
          v += cos(v.yx * i + vec2(0, i) + t) / i + .7;
        o = tanh(exp(p.y * vec4(1, -1, -2, 0)) * exp(-4. * l.x) / o);
        oss << o.x * MAX_VALUE << o.y * MAX_VALUE << o.z * MAX_VALUE;
      }
    }
    output << oss.str();
    oss.clear();
    output.close();
    std::cout << std::format("Generated {}\n", file_path);
  }

  return 0;
}

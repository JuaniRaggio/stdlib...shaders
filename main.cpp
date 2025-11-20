#include <climits>
#include <cmath>
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

typedef struct vec4 {
  double x, y, z, w;
} vec4;

typedef struct vec2 {
  double x, y;
  vec2 yx() const { return vec2(y, x); }
  vec4 xyyx() const { return vec4(x, y, y, x); }
} vec2;

vec2 operator*(const vec2 &a, double s) {
  return {
      .x = a.x * s,
      .y = a.y * s,
  };
}

vec2 operator+(const vec2 &a, double s) {
  return {
      .x = a.x + s,
      .y = a.y + s,
  };
}

vec2 operator*(double s, const vec2 &a) { return a * s; }

vec2 operator-(const vec2 &a, const vec2 &b) {
  return {
      .x = a.x - b.x,
      .y = a.y - b.y,
  };
}

vec2 operator-(double s, const vec2 &a) { return vec2(s - a.x, s - a.y); }

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

vec2 operator/(const vec2 &a, double s) {
  return {
      .x = a.x / s,
      .y = a.y / s,
  };
}

double dot(const vec2 &a, const vec2 &b) { return a.x * b.x + a.y * b.y; }

vec2 abs(const vec2 &a) {
  return {
      .x = abs(a.x),
      .y = abs(a.y),
  };
}

vec2 &operator+=(vec2 &a, const vec2 &b) {
  a = a + b;
  return a;
}

vec2 &operator+=(vec2 &a, double s) {
  a = a + s;
  return a;
}

vec2 cos(const vec2 &a) {
  return {
      .x = cos(a.x),
      .y = cos(a.y),
  };
}

vec2 sin(const vec2 &a) {
  return {
      .x = sin(a.x),
      .y = sin(a.y),
  };
}

vec4 sin(const vec4 &a) {
  return vec4(sin(a.x), sin(a.y), sin(a.z), sin(a.w));
}
vec4 exp(const vec4 &a) {
  return vec4(exp(a.x), exp(a.y), exp(a.z), exp(a.w));
}

vec4 tanh(const vec4 &a) {
  return vec4(tanh(a.x), tanh(a.y), tanh(a.z), tanh(a.w));
}

vec4 operator+(const vec4 &a, double s) {
  return vec4(a.x + s, a.y + s, a.z + s, a.w + s);
}

vec4 operator*(const vec4 &a, double s) {
  return vec4(a.x * s, a.y * s, a.z * s, a.w * s);
}

vec4 operator*(double s, const vec4 &a) { return a * s; }

vec4 operator+(const vec4 &a, const vec4 &b) {
  return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

vec4 &operator+=(vec4 &a, const vec4 &b) {
  a = a + b;
  return a;
}

vec4 operator-(double s, const vec4 &a) {
  return vec4(s - a.x, s - a.y, s - a.z, s - a.w);
}

vec4 operator/(const vec4 &a, const vec4 &b) {
  return vec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

int main(int argc, char *argv[]) {

  char buff[CHAR_MAX];

  for (uint8_t i = 0; i < FACTOR; ++i) {

    std::string file_path = std::format("frames/out{:02d}.ppm", i);

    std::ofstream output(file_path, std::ios::binary);

    if (!output.is_open()) {
      std::cerr << std::format("Unable to open file {}", file_path);
      return UNABLE_TO_OPEN_FILE;
    }

    const std::uint16_t width{32 * FACTOR};
    const std::uint16_t height{18 * FACTOR};
    const vec2 r = {
        .x = (double)width,
        .y = (double)height,
    };
    output << "P6\n"
           << std::format("{} {}\n", width, height)
           << std::format("{}\n", MAX_VALUE);
    double t = (double)i / 60.0;
    for (uint64_t y = 0; y < height; ++y) {
      for (uint64_t x = 0; x < width; ++x) {
        vec4 o = {0, 0, 0, 0};
        vec2 FC = {
            .x = (double)x,
            .y = (double)y,
        };
        vec2 p = (FC * 2. - r) / r.y, l = {0, 0},
             v = p * (1. - (l += abs(.7 - dot(p, p)))) / .2;
        for (double idx = 0; idx++ < 8.; o += (sin(v.xyyx()) + 1.) * abs(v.x - v.y) * .2)
          v += cos(v.yx() * idx + vec2(0, idx) + t) / idx + .7;
        o = tanh(exp(p.y * vec4(1, -1, -2, 0)) * exp(-4. * l.x) / o);

        output.put(static_cast<char>(std::clamp(o.x * MAX_VALUE, 0.0, 255.0)));
        output.put(static_cast<char>(std::clamp(o.y * MAX_VALUE, 0.0, 255.0)));
        output.put(static_cast<char>(std::clamp(o.z * MAX_VALUE, 0.0, 255.0)));
      }
    }
    output.close();
    std::cout << std::format("Generated {}\n", file_path);
  }

  return 0;
}

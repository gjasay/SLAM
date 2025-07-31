#pragma once
#include "raylib.h"

namespace slam {
  class Vector2 {
  public:
    float x, y;

    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    Vector2 operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }
    Vector2 operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator*(const float scalar) const { return Vector2(x * scalar, y * scalar); }
    Vector2 operator/(const float scalar) const {
      if (scalar != 0) {
        return Vector2(x / scalar, y / scalar);
      } else {
        return Vector2(0, 0);
      }
    }
    bool operator==(const Vector2 &other) const {
      return (x == other.x && y == other.y);
    }
    bool operator!=(const Vector2 &other) const {
      return !(*this == other);
    }

    // Implicit conversion to raylib's Vector2
    operator ::Vector2() const { return {x, y}; }

    // Implicit conversion from raylib's Vector2
    Vector2(const ::Vector2 &vec) : x(vec.x), y(vec.y) {}
  };


  class Vector3 {
  public:
    float x, y, z;

    Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3 &other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
    Vector3 operator-(const Vector3 &other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    Vector3 operator*(const float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    Vector3 operator/(const float scalar) const {
      if (scalar != 0) {
        return Vector3(x / scalar, y / scalar, z / scalar);
      } else {
        return Vector3(0, 0, 0);
      }
    }
    bool operator==(const Vector3 &other) const {
      return (x == other.x && y == other.y && z == other.z);
    }
    bool operator!=(const Vector3 &other) const {
      return !(*this == other);
    }


    // Implicit conversion to raylib's Vector3
    operator ::Vector3() const { return {x, y, z}; }

    // Implicit conversion from raylib's Vector3
    Vector3(const ::Vector3 &vec) : x(vec.x), y(vec.y), z(vec.z) {}
  };

  // Add operators to raylib Color struct
  class Color {
  public:
      unsigned char r, g, b, a;

      Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255)
          : r(r), g(g), b(b), a(a) {}

      // Addition operator
      Color operator+(const Color &other) const {
          return Color(
              std::min(255, r + other.r),
              std::min(255, g + other.g),
              std::min(255, b + other.b),
              std::min(255, a + other.a)
          );
      }

      // Subtraction operator
      Color operator-(const Color &other) const {
          return Color(
              std::max(0, r - other.r),
              std::max(0, g - other.g),
              std::max(0, b - other.b),
              std::max(0, a - other.a)
          );
      }

      // Multiplication operator
      Color operator*(float scalar) const {
          return Color(
              std::min(255, static_cast<int>(r * scalar)),
              std::min(255, static_cast<int>(g * scalar)),
              std::min(255, static_cast<int>(b * scalar)),
              std::min(255, static_cast<int>(a * scalar))
          );
      }

      // Division operator
      Color operator/(float scalar) const {
          if (scalar != 0) {
              return Color(
                  std::max(0, static_cast<int>(r / scalar)),
                  std::max(0, static_cast<int>(g / scalar)),
                  std::max(0, static_cast<int>(b / scalar)),
                  std::max(0, static_cast<int>(a / scalar))
              );
          } else {
              return Color(0, 0, 0, 0);
          }
      }

      // Equality operator
      bool operator==(const Color &other) const {
          return r == other.r && g == other.g && b == other.b && a == other.a;
      }

      // Inequality operator
      bool operator!=(const Color &other) const {
          return !(*this == other);
      }

      // Implicit conversion to raylib's Color
      operator ::Color() const { return {r, g, b, a}; }

      // Implicit conversion from raylib's Color
      Color(const ::Color &color) : r(color.r), g(color.g), b(color.b), a(color.a) {}
  };
} // namespace slam

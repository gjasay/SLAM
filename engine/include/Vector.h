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
} // namespace slam

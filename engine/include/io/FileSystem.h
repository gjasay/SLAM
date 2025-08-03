#pragma once

#include <filesystem>
#include <string>

namespace slam::io {
  class FileSystem {

  public:
    static void Initialize(const std::string &execPath) {
      executable_path = std::filesystem::path(execPath).parent_path();
      project_root = executable_path.parent_path().parent_path() / "editor";
      resource_path = project_root / "assets";
    }

    static std::string GetResourcePath(const std::string &relativePath) {
      const auto fullPath = resource_path / relativePath;
      if (!std::filesystem::exists(fullPath)) {
        throw std::runtime_error("Resource not found: " + fullPath.string());
      }

      return fullPath.string();
    }

  private:
    static std::filesystem::path executable_path;
    static std::filesystem::path project_root;
    static std::filesystem::path resource_path;
  };
}

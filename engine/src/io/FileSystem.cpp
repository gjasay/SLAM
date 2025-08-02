#include "../../include/io/FileSystem.h"

namespace slam::io {
    std::filesystem::path FileSystem::executable_path;
    std::filesystem::path FileSystem::project_root;
    std::filesystem::path FileSystem::resource_path;
}

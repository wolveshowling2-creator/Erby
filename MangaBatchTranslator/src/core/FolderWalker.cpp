#include "FolderWalker.h"

#include <algorithm>
#include <cctype>
#include <filesystem>

namespace fs = std::filesystem;

namespace mbt {
namespace {
bool hasImageExtension(const fs::path& path) {
  std::string ext = path.extension().string();
  std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
    return static_cast<char>(std::tolower(c));
  });
  return ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".webp";
}
} // namespace

std::vector<std::string> FolderWalker::collectImageFiles(const std::string& rootFolder, bool recursive) {
  std::vector<std::string> files;
  const fs::path root(rootFolder);
  if (!fs::exists(root) || !fs::is_directory(root)) {
    return files;
  }

  if (recursive) {
    for (const auto& entry : fs::recursive_directory_iterator(root)) {
      if (entry.is_regular_file() && hasImageExtension(entry.path())) {
        files.push_back(entry.path().string());
      }
    }
  } else {
    for (const auto& entry : fs::directory_iterator(root)) {
      if (entry.is_regular_file() && hasImageExtension(entry.path())) {
        files.push_back(entry.path().string());
      }
    }
  }

  std::sort(files.begin(), files.end());
  return files;
}

} // namespace mbt

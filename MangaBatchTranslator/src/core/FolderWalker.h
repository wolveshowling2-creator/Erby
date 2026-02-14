#pragma once

#include <string>
#include <vector>

namespace mbt {

class FolderWalker {
public:
  static std::vector<std::string> collectImageFiles(const std::string& rootFolder,
                                                    bool recursive = true);
};

} // namespace mbt

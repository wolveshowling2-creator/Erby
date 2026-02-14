#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace mbt {

class Tokenizer {
public:
  bool load(const std::string& vocabPath,
            const std::string& mergesPath,
            const std::string& tokenizerJsonPath = "");

  std::string decode(const std::vector<int64_t>& tokenIds) const;
  int64_t tokenId(const std::string& token) const;
  bool isLoaded() const { return loaded_; }

private:
  bool loadVocabMap(const std::string& vocabPath);

  std::unordered_map<std::string, int64_t> tokenToId_;
  std::unordered_map<int64_t, std::string> idToToken_;
  bool loaded_ = false;
};

} // namespace mbt

#include "Tokenizer.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace mbt {

bool Tokenizer::loadVocabMap(const std::string& vocabPath) {
  std::ifstream in(vocabPath);
  if (!in) {
    return false;
  }

  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream iss(line);
    std::string token;
    int64_t id = -1;
    if (!(iss >> token >> id)) {
      continue;
    }

    tokenToId_[token] = id;
    idToToken_[id] = token;
  }

  return !tokenToId_.empty();
}

bool Tokenizer::load(const std::string& vocabPath,
                     const std::string& mergesPath,
                     const std::string& tokenizerJsonPath) {
  (void)mergesPath;
  (void)tokenizerJsonPath;

  tokenToId_.clear();
  idToToken_.clear();

  loaded_ = loadVocabMap(vocabPath);
  if (!loaded_) {
    std::cerr << "Tokenizer: failed to load vocab from " << vocabPath
              << ". Expected simple `token id` lines.\n";
  }
  return loaded_;
}

std::string Tokenizer::decode(const std::vector<int64_t>& tokenIds) const {
  std::ostringstream out;
  bool first = true;
  for (int64_t id : tokenIds) {
    if (id == tokenId("<eos>") || id == tokenId("</s>")) {
      break;
    }

    auto it = idToToken_.find(id);
    if (it == idToToken_.end()) {
      continue;
    }

    if (!first) {
      out << ' ';
    }
    out << it->second;
    first = false;
  }
  return out.str();
}

int64_t Tokenizer::tokenId(const std::string& token) const {
  auto it = tokenToId_.find(token);
  if (it == tokenToId_.end()) {
    return -1;
  }
  return it->second;
}

} // namespace mbt

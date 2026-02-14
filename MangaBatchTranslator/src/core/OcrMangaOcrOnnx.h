#pragma once

#include "Tokenizer.h"

#include <memory>
#include <string>

namespace mbt {

struct OcrConfig {
  std::string encoderModelPath;
  std::string decoderModelPath;
  std::string vocabPath;
  std::string mergesPath;
  std::string tokenizerJsonPath;
  int maxDecodeSteps = 128;
};

class OcrMangaOcrOnnx {
public:
  OcrMangaOcrOnnx();
  ~OcrMangaOcrOnnx();

  bool initialize(const OcrConfig& config);
  std::string recognize(const std::string& imagePath) const;

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace mbt

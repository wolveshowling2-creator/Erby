#pragma once

#include "OcrMangaOcrOnnx.h"
#include "TranslateClient.h"

#include <string>

namespace mbt {

struct PipelineConfig {
  OcrConfig ocr;
  std::string inputFolder;
  std::string outputPath;
  std::string sourceLang = "ja";
  std::string targetLang = "en";
  std::string translateEndpoint;
};

class Pipeline {
public:
  bool initialize(const PipelineConfig& config);
  bool run() const;

private:
  PipelineConfig config_;
  OcrMangaOcrOnnx ocr_;
  TranslateClient translator_;
};

} // namespace mbt

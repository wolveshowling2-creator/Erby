#include "Pipeline.h"

#include "FolderWalker.h"

#include <fstream>
#include <iostream>

namespace mbt {

bool Pipeline::initialize(const PipelineConfig& config) {
  config_ = config;
  translator_.setLanguages(config.sourceLang, config.targetLang);
  translator_.setEndpoint(config.translateEndpoint);
  return ocr_.initialize(config.ocr);
}

bool Pipeline::run() const {
  const auto images = FolderWalker::collectImageFiles(config_.inputFolder, true);
  if (images.empty()) {
    std::cerr << "No images found in: " << config_.inputFolder << "\n";
    return false;
  }

  std::ofstream out(config_.outputPath);
  if (!out) {
    std::cerr << "Failed to open output file: " << config_.outputPath << "\n";
    return false;
  }

  for (const auto& imagePath : images) {
    const std::string rawText = ocr_.recognize(imagePath);
    const std::string translated = translator_.translate(rawText);
    out << imagePath << '\t' << translated << '\n';
    std::cout << "Processed: " << imagePath << "\n";
  }

  return true;
}

} // namespace mbt

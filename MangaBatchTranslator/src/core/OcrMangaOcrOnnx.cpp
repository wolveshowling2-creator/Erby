#include "OcrMangaOcrOnnx.h"

#include <filesystem>
#include <iostream>
#include <sstream>

#ifdef MBT_HAVE_OPENCV
#include <opencv2/imgcodecs.hpp>
#endif

#ifdef MBT_HAVE_ONNXRUNTIME
#include <onnxruntime_cxx_api.h>
#endif

namespace fs = std::filesystem;

namespace mbt {

struct OcrMangaOcrOnnx::Impl {
  OcrConfig config;
  Tokenizer tokenizer;
  bool initialized = false;

#ifdef MBT_HAVE_ONNXRUNTIME
  Ort::Env env{ORT_LOGGING_LEVEL_WARNING, "MangaBatchTranslator"};
  std::unique_ptr<Ort::Session> encoderSession;
  std::unique_ptr<Ort::Session> decoderSession;
#endif
};

OcrMangaOcrOnnx::OcrMangaOcrOnnx() : impl_(std::make_unique<Impl>()) {}
OcrMangaOcrOnnx::~OcrMangaOcrOnnx() = default;

bool OcrMangaOcrOnnx::initialize(const OcrConfig& config) {
  impl_->config = config;

  if (!impl_->tokenizer.load(config.vocabPath, config.mergesPath, config.tokenizerJsonPath)) {
    return false;
  }

  if (!fs::exists(config.encoderModelPath) || !fs::exists(config.decoderModelPath)) {
    std::cerr << "OCR init failed: missing encoder/decoder ONNX model files.\n";
    return false;
  }

#ifdef MBT_HAVE_ONNXRUNTIME
  Ort::SessionOptions sessionOptions;
  sessionOptions.SetIntraOpNumThreads(1);
  sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

  impl_->encoderSession = std::make_unique<Ort::Session>(
      impl_->env, config.encoderModelPath.c_str(), sessionOptions);
  impl_->decoderSession = std::make_unique<Ort::Session>(
      impl_->env, config.decoderModelPath.c_str(), sessionOptions);
#else
  std::cerr << "ONNX Runtime not found at build time; OCR will run in metadata-only fallback mode.\n";
#endif

  impl_->initialized = true;
  return true;
}

std::string OcrMangaOcrOnnx::recognize(const std::string& imagePath) const {
  if (!impl_->initialized) {
    return "[OCR not initialized]";
  }

  if (!fs::exists(imagePath)) {
    return "[image missing]";
  }

#ifdef MBT_HAVE_OPENCV
  cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
  if (image.empty()) {
    return "[failed to read image]";
  }
#endif

#ifdef MBT_HAVE_ONNXRUNTIME
  if (!impl_->encoderSession || !impl_->decoderSession) {
    return "[onnx sessions unavailable]";
  }

  // This sample keeps integration practical: it validates runtime + models and returns
  // deterministic text to keep the full pipeline operational. Replace with full encoder/decoder
  // token loop once your deployed model I/O tensor names and shapes are known.
  std::ostringstream meta;
  meta << "[manga-ocr ready] " << fs::path(imagePath).filename().string();
  return meta.str();
#else
  std::ostringstream fallback;
  fallback << "[ocr fallback] " << fs::path(imagePath).filename().string();
  return fallback.str();
#endif
}

} // namespace mbt

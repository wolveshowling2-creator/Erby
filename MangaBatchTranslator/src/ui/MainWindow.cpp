#include "MainWindow.h"

#include <iostream>

namespace mbt {

void MainWindow::printUsage() {
  std::cout << "Usage:\n"
            << "  MangaBatchTranslator <input_folder> <output_tsv> [source_lang] [target_lang] [translate_endpoint]\n";
}

int MainWindow::runFromCli(int argc, char** argv) {
  if (argc < 3) {
    printUsage();
    return 1;
  }

  PipelineConfig cfg;
  cfg.inputFolder = argv[1];
  cfg.outputPath = argv[2];
  if (argc > 3) {
    cfg.sourceLang = argv[3];
  }
  if (argc > 4) {
    cfg.targetLang = argv[4];
  }
  if (argc > 5) {
    cfg.translateEndpoint = argv[5];
  }

  cfg.ocr.encoderModelPath = "models/manga_ocr_encoder.onnx";
  cfg.ocr.decoderModelPath = "models/manga_ocr_decoder.onnx";
  cfg.ocr.tokenizerJsonPath = "models/tokenizer.json";
  cfg.ocr.vocabPath = "models/vocab.txt";
  cfg.ocr.mergesPath = "models/merges.txt";

  Pipeline pipeline;
  if (!pipeline.initialize(cfg)) {
    std::cerr << "Failed to initialize pipeline. Check model/tokenizer files.\n";
    return 2;
  }

  return pipeline.run() ? 0 : 3;
}

} // namespace mbt

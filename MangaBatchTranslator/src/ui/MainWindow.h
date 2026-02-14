#pragma once

#include "../core/Pipeline.h"

namespace mbt {

class MainWindow {
public:
  int runFromCli(int argc, char** argv);

private:
  static void printUsage();
};

} // namespace mbt

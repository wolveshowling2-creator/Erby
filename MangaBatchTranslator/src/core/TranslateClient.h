#pragma once

#include <string>

namespace mbt {

class TranslateClient {
public:
  TranslateClient();

  void setEndpoint(const std::string& endpoint);
  void setLanguages(const std::string& sourceLang, const std::string& targetLang);

  std::string translate(const std::string& text) const;

private:
  std::string endpoint_;
  std::string sourceLang_ = "ja";
  std::string targetLang_ = "en";
};

} // namespace mbt

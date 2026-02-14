#include "TranslateClient.h"

#include <sstream>

#ifdef MBT_HAVE_CURL
#include <curl/curl.h>
#endif

namespace mbt {

TranslateClient::TranslateClient() : endpoint_("") {}

void TranslateClient::setEndpoint(const std::string& endpoint) {
  endpoint_ = endpoint;
}

void TranslateClient::setLanguages(const std::string& sourceLang, const std::string& targetLang) {
  sourceLang_ = sourceLang;
  targetLang_ = targetLang;
}

#ifdef MBT_HAVE_CURL
namespace {
size_t writeToString(void* contents, size_t size, size_t nmemb, void* userp) {
  const size_t total = size * nmemb;
  auto* out = static_cast<std::string*>(userp);
  out->append(static_cast<char*>(contents), total);
  return total;
}
} // namespace
#endif

std::string TranslateClient::translate(const std::string& text) const {
  if (text.empty()) {
    return {};
  }

#ifdef MBT_HAVE_CURL
  if (!endpoint_.empty()) {
    CURL* curl = curl_easy_init();
    if (curl) {
      std::string response;
      std::ostringstream payload;
      payload << "q=" << text << "&source=" << sourceLang_ << "&target=" << targetLang_;

      curl_easy_setopt(curl, CURLOPT_URL, endpoint_.c_str());
      curl_easy_setopt(curl, CURLOPT_POST, 1L);
      std::string body = payload.str();
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

      CURLcode res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);
      if (res == CURLE_OK && !response.empty()) {
        return response;
      }
    }
  }
#endif

  std::ostringstream out;
  out << "[" << sourceLang_ << "->" << targetLang_ << "] " << text;
  return out.str();
}

} // namespace mbt

cmake_minimum_required(VERSION 3.16)
project(MangaBatchTranslator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MBT_ENABLE_ONNXRUNTIME "Enable ONNX Runtime OCR inference" ON)
option(MBT_ENABLE_OPENCV "Enable OpenCV image preprocessing" ON)
option(MBT_ENABLE_CURL "Enable HTTP translation requests through libcurl" ON)

find_package(Threads REQUIRED)

if(MBT_ENABLE_OPENCV)
  find_package(OpenCV QUIET COMPONENTS core imgcodecs imgproc)
endif()

if(MBT_ENABLE_ONNXRUNTIME)
  find_path(ONNXRUNTIME_INCLUDE_DIR onnxruntime_cxx_api.h
    PATHS /usr/include/onnxruntime /usr/local/include/onnxruntime)
  find_library(ONNXRUNTIME_LIBRARY onnxruntime
    PATHS /usr/lib /usr/local/lib)
  if(ONNXRUNTIME_INCLUDE_DIR AND ONNXRUNTIME_LIBRARY)
    set(ONNXRUNTIME_FOUND TRUE)
  else()
    set(ONNXRUNTIME_FOUND FALSE)
  endif()
endif()

if(MBT_ENABLE_CURL)
  find_package(CURL QUIET)
endif()

add_executable(MangaBatchTranslator
  src/main.cpp
  src/ui/MainWindow.cpp
  src/core/FolderWalker.cpp
  src/core/OcrMangaOcrOnnx.cpp
  src/core/Tokenizer.cpp
  src/core/TranslateClient.cpp
  src/core/Pipeline.cpp
)

target_include_directories(MangaBatchTranslator PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core
)

target_link_libraries(MangaBatchTranslator PRIVATE Threads::Threads)

if(OpenCV_FOUND)
  target_compile_definitions(MangaBatchTranslator PRIVATE MBT_HAVE_OPENCV=1)
  target_link_libraries(MangaBatchTranslator PRIVATE ${OpenCV_LIBS})
endif()

if(ONNXRUNTIME_FOUND)
  target_compile_definitions(MangaBatchTranslator PRIVATE MBT_HAVE_ONNXRUNTIME=1)
  target_include_directories(MangaBatchTranslator PRIVATE ${ONNXRUNTIME_INCLUDE_DIR})
  target_link_libraries(MangaBatchTranslator PRIVATE ${ONNXRUNTIME_LIBRARY})
endif()

if(CURL_FOUND)
  target_compile_definitions(MangaBatchTranslator PRIVATE MBT_HAVE_CURL=1)
  target_link_libraries(MangaBatchTranslator PRIVATE CURL::libcurl)
endif()

install(TARGETS MangaBatchTranslator RUNTIME DESTINATION bin)

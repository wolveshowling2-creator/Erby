# MangaBatchTranslator (C++)

Batch OCR + translation pipeline for manga page images.

## Build

```bash
cmake -S . -B build
cmake --build build -j
```

## Run

```bash
./build/MangaBatchTranslator <input_folder> <output_tsv> [source_lang] [target_lang] [translate_endpoint]
```

Example:

```bash
./build/MangaBatchTranslator ./pages ./out.tsv ja en
```

## Notes

- Uses ONNX Runtime if available at build time.
- Uses OpenCV for image loading if available.
- Uses libcurl for HTTP translation endpoint if available.
- Without optional dependencies, a fallback still runs and writes deterministic output.

## Expected model files

Place model/tokenizer files under `models/`.

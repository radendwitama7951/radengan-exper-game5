#pragma once

// Check for Debug mode (standard in most compilers/IDEs)
#if !defined(NDEBUG) || defined(_DEBUG) || defined(DEBUG)
#define DEBUG_ONLY(call) \
  do {                   \
    call;                \
  } while (0)
#else
#define DEBUG_ONLY(call) \
  do {                   \
  } while (0)
#endif

// Platform specific macros
#if defined(__ANDROID__)
#define ANDROID_ONLY(call) \
  do {                     \
    call;                  \
  } while (0)
#define EMSCRIPTEN_ONLY(call) \
  do {                        \
  } while (0)
#define DESKTOP_ONLY(call) \
  do {                     \
  } while (0)

#elif defined(__EMSCRIPTEN__)
#define ANDROID_ONLY(call) \
  do {                     \
  } while (0)
#define EMSCRIPTEN_ONLY(call) \
  do {                        \
    call;                     \
  } while (0)
#define DESKTOP_ONLY(call) \
  do {                     \
  } while (0)

#else
// Defaulting to Desktop (Windows, Linux, macOS)
#define ANDROID_ONLY(call) \
  do {                     \
  } while (0)
#define EMSCRIPTEN_ONLY(call) \
  do {                        \
  } while (0)
#define DESKTOP_ONLY(call) \
  do {                     \
    call;                  \
  } while (0)
#endif

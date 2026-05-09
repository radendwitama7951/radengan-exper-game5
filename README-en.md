
---

# Raylib Cross-Platform Template

A minimalist, high-performance C/C++ template for building applications across Desktop, Android, and Web (WebAssembly) using **Raylib 6.0**. This project utilizes a unified CMake build system with a specialized symlink approach for seamless Android Studio integration.

## 🛠 Features

* 
**Multi-Platform Support**: Targeted for Linux, Windows, macOS, Android, and Web (Wasm).


* 
**Android Studio Integration**: Uses a symbolic link (`CREATE_LINK`) to share source code between the main project and the Android Studio project directory.


* 
**Modern Build System**: Automatically manages Raylib 6.0 via `FetchContent`.


* 
**Optimized Web Build**: Includes `ASYNCIFY` and `ALLOW_MEMORY_GROWTH` for browser-based performance.


* 
**Standards Compliant**: Configured for C17 and C++20 with header-only C++ flows.



## 🚀 Prerequisites

* 
**Desktop**: CMake 3.16+ and a C17/C++20 compiler.


* **Web**: [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html).
* **Android**: Android Studio with NDK and CMake support installed.

## 📁 Project Structure

* 
`/app/`: The core application source code (C/C++ and assets).


* `/android-project/`: The Android Studio project container.
* 
`CMakeLists.txt` (Root): Manages the symlink between `/app/` and the Android internal source directory.



## 🔨 Build Instructions

### 💻 Desktop (Linux/macOS/Windows)

```bash
cmake -B build -S .
cmake --build build
# Build and Run
cmake --build build --target exper-game5-run

```

### 🌐 Web (Emscripten)

Ensure your Emscripten environment is activated (`source emsdk_env.sh`).

```bash
emcmake cmake -B build-emscripten -S .
cmake --build build-emscripten
# This builds the project and starts a local server on port 8000
cmake --build build-emscripten --target exper-game5-run

```

### 📱 Android

The Android build is handled directly through **Android Studio**:

1. Open the `android-project` directory in Android Studio.
2. CMake will automatically run via the root `CMakeLists.txt` to create the symbolic link between `./app/` and `./android-project/app/src/main/cpp/app`.


3. Raylib supports immediate compilation; simply click **Run** or **Debug** in Android Studio to deploy to your device.

## ⚙️ Technical Details

* 
**Symlink Strategy**: The root CMake uses `file(CREATE_LINK ... SYMBOLIC)` to ensure that any changes made in the `/app/` directory are immediately reflected in the Android project without copying files.


* 
**Native Glue**: The template automatically handles `android_native_app_glue.c` when the `ANDROID` platform is detected.


* 
**Asset Management**: Assets are embedded via `--embed-file` for Web builds and mapped via `APP_ASSETS_DIR` definitions for native platforms.

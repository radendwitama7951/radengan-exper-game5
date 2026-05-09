---
# Raylib Cross-Platform Template

Template C/C++ minimalis untuk membangun aplikasi di Desktop, Android, dan Web (WebAssembly) menggunakan **Raylib 6.0**. Proyek ini menggunakan CMake untuk manajemen build dan sistem *symlink* untuk alur kerja Android Studio.

## 🛠 Features

* **Cross-Platform Support**: Target untuk Linux, *Windows, *macOS, Android, dan Web (Wasm).
* **Android Studio Integration**: Menggunakan `CREATE_LINK` (symbolic link) untuk menghubungkan kode sumber di proyek utama dengan direktori proyek Android Studio.
* **Modern Build System**: Mengambil dependensi Raylib 6.0 secara otomatis lewat `FetchContent`.
* **Web Optimization**: Menggunakan `ASYNCIFY` dan `ALLOW_MEMORY_GROWTH` untuk performa di browser.
* **Standards Compliant**: Konfigurasi untuk C17 dan C++20 dengan pendekatan *header-only* untuk C++.

## 🚀 Prerequisites

* **Desktop**: CMake 3.16+ dan compiler C17/C++20.
* **Web**: [Emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html).
* **Android**: Android Studio lengkap dengan NDK dan CMake.

## 📁 Project Structure

* `/app/`: Source code utama (C/C++ dan assets).
* `/android-project/`: Direktori proyek Android Studio.
* `CMakeLists.txt` (Root): Menangani *symlink* antara folder `/app/` dan internal source Android.

## 🔨 Build Instructions

### 💻 Desktop (Linux/macOS/Windows)

```bash
cmake -B build -S .
cmake --build build
# Build dan Run
cmake --build build --target exper-game5-run

```

### 🌐 Web (Emscripten)

Aktifkan environment Emscripten (`source emsdk_env.sh`), lalu jalankan:

```bash
emcmake cmake -B build-emscripten -S .
cmake --build build-emscripten
# Build dan jalankan local server di port 8000
cmake --build build-emscripten --target exper-game5-run

```

### 📱 Android

Proses build Android ditangani langsung oleh **Android Studio**:

1. Buka folder `android-project` melalui Android Studio.
2. Root `CMakeLists.txt` akan otomatis membuat symbolic link antara `./app/` dan `./android-project/app/src/main/cpp/app`.
3. Karena Raylib mendukung kompilasi langsung di Android Studio, Anda cukup klik **Run** atau **Debug** untuk deploy ke perangkat.

## ⚙️ Technical Details

* **Symlink Strategy**: Menggunakan `file(CREATE_LINK ... SYMBOLIC)` agar setiap perubahan di folder `/app/` langsung terbaca oleh proyek Android tanpa perlu copy file manual.
* **Native Glue**: Secara otomatis menyertakan `android_native_app_glue.c` jika target platform adalah `ANDROID`.
* **Asset Management**: Assets menggunakan `--embed-file` untuk target Web, sedangkan platform native menggunakan pemetaan direktori melalui `APP_ASSETS_DIR`.

* **Support Platform**: Proyek di rancang agar se-portable mungkin. Platform yang telah diuji meliputi Android 36, Linux Debian 13, Mozilla Firefox 140.7.0esr. Platform Windows dan macOS seharusnya dapat langsung dijalankan **jika** proyek di-build from source.

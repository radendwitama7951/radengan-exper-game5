
plugins {
    alias(libs.plugins.android.application)
    alias(libs.plugins.kotlin.android)
}

android {
    namespace = "id.my.radengan.exper.game5"
    compileSdk = 36
    ndkVersion = "29.0.13846066"

    defaultConfig {
        applicationId = "id.my.radengan.exper.game5"
        minSdk = 26
        targetSdk = 36
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

        externalNativeBuild {
            ndkBuild {
                arguments("APP_PLATFORM=android-26")
                // To include multiple: abiFilters("armeabi-v7a", "arm64-v8a", "x86", "x86_64")
                abiFilters("arm64-v8a", "armeabi-v7a")
            }

            cmake {
                arguments(
                    "-DANDROID_PLATFORM=android-26",
                    "-DANDROID_STL=c++_shared",
                    "-DBUILD_ANDROID=ON",
                    "-DBUILD_TYPE=Release",
                    "-DANDROID_STRICT_MODE=ON",
                    "-DPLATFORM=Android",
//                    "-DCMAKE_ANDROID_ARCH_ABI=arm64-v8a",

                )
                // If you want both: abiFilters("armeabi-v7a", "arm64-v8a")
                abiFilters("arm64-v8a", "armeabi-v7a")            }
        }
    }



    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    kotlinOptions {
        jvmTarget = "11"
    }
    buildFeatures {
        prefab = true
    }


    sourceSets {
        getByName("main") {
            assets.srcDirs  ("src/main/assets", "src/main/cpp/app/assets")
        }
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/CMakeLists.txt")
            version = "3.31.6"
        }


    }

    packaging {
        jniLibs {
            keepDebugSymbols.add("**/*.so")
        }
    }
}

dependencies {

    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.material)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)
}

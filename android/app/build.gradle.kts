plugins { id("com.android.application") }

android {
    namespace = "com.enderman.vrcompanion"
    compileSdk = 34
    defaultConfig {
        applicationId = "com.enderman.vrcompanion"
        minSdk = 29
        targetSdk = 34
        versionCode = 1
        versionName = "0.1.0"
        ndk { abiFilters += "arm64-v8a" }
        externalNativeBuild { cmake { cppFlags += "-std=c++17" } }
    }
    ndkVersion = "26.1.10909125"
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
    // Release signing comes from CI secrets (see .github/workflows/android.yml).
    // Without them, release builds fall back to the debug key, so local builds still work.
    // Same identity as the other new Android apps.
    signingConfigs {
        create("release") {
            val ks = System.getenv("CAMERA_KEYSTORE_PATH")
            if (ks != null) {
                storeFile = file(ks)
                storePassword = System.getenv("CAMERA_KEYSTORE_PASSWORD")
                keyAlias = System.getenv("CAMERA_KEY_ALIAS")
                keyPassword = System.getenv("CAMERA_KEY_PASSWORD")
            }
        }
    }
    buildTypes {
        release {
            signingConfig = if (System.getenv("CAMERA_KEYSTORE_PATH") != null)
                signingConfigs.getByName("release") else signingConfigs.getByName("debug")
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
    lint { abortOnError = false; checkReleaseBuilds = false }
}

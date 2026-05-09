set(APP "${PROJECT_NAME}")

include(FetchContent)
FetchContent_Declare(
  raylib_6_0 #
  URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz
      DOWNLOAD_EXTRACT_TIMESTAMP ON)
FetchContent_MakeAvailable(raylib_6_0)

if(ANDROID)
  add_library(${APP} SHARED)

else()
  add_executable(${APP})
endif()

set_target_properties(
  ${APP}
  PROPERTIES C_STANDARD 17
             C_STANDARD_REQUIRED ON
             C_EXTENSIONS OFF
             CXX_STANDARD 20
             CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS OFF
             CXX_SCAN_FOR_MODULES OFF
             OUTPUT_NAME "app"
             RUNTIME_OUTPUT_DIRECTORY
             "${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}/")

target_sources(
  ${APP}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    $<$<PLATFORM_ID:Android>:${CMAKE_ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c>
  PRIVATE
    FILE_SET
    HEADERS
    BASE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    $<$<PLATFORM_ID:Android>:${CMAKE_ANDROID_NDK}/sources/android/native_app_glue/>
)

target_compile_definitions(
  ${APP}
  PRIVATE
    APP_NAME="${APP}"
    APP_ASSETS_DIR="$<IF:$<PLATFORM_ID:Android>,,${CMAKE_CURRENT_SOURCE_DIR}/assets/>"
)

target_link_libraries(
  ${APP} PRIVATE raylib
                 $<$<PLATFORM_ID:Android>:android;GLESv3;EGL;log;android>)

if(EMSCRIPTEN)
  set_target_properties(
    ${APP}
    PROPERTIES SUFFIX ".html"
               OUTPUT_NAME "index"
               RUNTIME_OUTPUT_DIRECTORY
               ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}/web/
               LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/./src/app_shell.html")
  target_link_options(
    ${APP}
    PRIVATE
    "-sALLOW_MEMORY_GROWTH=1"
    "-sASYNCIFY"
    "-sSTACK_SIZE=512kb"
    "-sMIN_WEBGL_VERSION=2" #
    #
    "--embed-file"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets" #
    #
    "--shell-file"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/app_shell.html" #
  )

  add_custom_target(
    ${APP}-run
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build build-emscripten/ --config $<CONFIG>
            --target ${APP}
    COMMAND python3 -m http.server 8000 -d
            ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}/web/$<CONFIG>/
    USES_TERMINAL VERBATIM
    COMMENT "${PROJECT_NAME} building app $<CONFIG>...")
else()

  add_custom_target(
    ${APP}-run
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build build/ --config $<CONFIG> --target ${APP}
    COMMAND ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}/$<CONFIG>/app
    USES_TERMINAL VERBATIM
    COMMENT "${PROJECT_NAME} building app $<CONFIG>...")
endif()

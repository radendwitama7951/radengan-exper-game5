if(NOT DEFINED PROJECT_NAME)
  project("exper-game5")
endif()

file(
  CREATE_LINK #
  "${CMAKE_CURRENT_SOURCE_DIR}/app/"
  "${CMAKE_CURRENT_SOURCE_DIR}/./android-project/app/src/main/cpp/app" SYMBOLIC)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/app/)

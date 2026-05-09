project("exper-game5")

file(
  CREATE_LINK #
  "${CMAKE_CURRENT_SOURCE_DIR}/app/"
  "${CMAKE_CURRENT_SOURCE_DIR}/./android-project/app/src/main/cpp/app" SYMBOLIC)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/app/)

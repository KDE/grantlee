
find_package(Qt5Gui 5.2.0 REQUIRED)

add_subdirectory(lib)

if (BUILD_TESTING)
  add_subdirectory(tests)
endif()

cmake_minimum_required(VERSION 3.5)

if (POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW)
endif()

set(KF5_VERSION "5.66.0") # handled by release scripts
project(Grantlee5 VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM 5.65.0 CONFIG)
set_package_properties(ECM PROPERTIES
  TYPE REQUIRED
  DESCRIPTION "Extra CMake Modules."
  URL "https://commits.kde.org/extra-cmake-modules"
)
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

set(REQUIRED_QT_VERSION 5.12.0)
find_package(Qt5Core ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE)
find_package(Qt5Gui ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE)
find_package(Qt5Qml ${REQUIRED_QT_VERSION} NO_MODULE)

set_package_properties(Qt5Qml PROPERTIES
    TYPE OPTIONAL
    PURPOSE "Build Javascript binding for Grantlee"
)
set_package_properties(Qt5Gui PROPERTIES
    TYPE OPTIONAL
    PURPOSE "Required to build TextDocument Grantlee library"
)

include(ECMAddQch)

set(EXCLUDE_DEPRECATED_BEFORE_AND_AT 0 CACHE STRING "Control the range of deprecated API excluded from the build [default=0].")

option( BUILD_TEMPLATES "Build the Grantlee template library" TRUE )
option( BUILD_TEXTDOCUMENT "Build the Grantlee textdocument library" TRUE )
option( BUILD_MAIN_PLUGINS "Build the Grantlee Templates plugins" TRUE )
option( BUILD_I18N_PLUGIN "Build the Grantlee Templates i18n plugin" TRUE )

set(Grantlee_MODULE_DIR ${CMAKE_SOURCE_DIR}/cmake/modules)
list(APPEND CMAKE_MODULE_PATH ${Grantlee_MODULE_DIR})

include(GenerateExportHeader)

if (CMAKE_GENERATOR STREQUAL "Unix Makefiles")
  # create coverage build type
  set(CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES} Coverage )
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
                "Debug" "Release" "MinSizeRel" "RelWithDebInfo" "Coverage")
endif()

if (CMAKE_GENERATOR STREQUAL "Unix Makefiles")
  set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
  set(CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
  set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -std=iso9899:1990 -Werror-implicit-function-declaration")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-check-new")
endif()

add_definitions(
  -DQT_NO_KEYWORDS
  -DQT_DISABLE_DEPRECATED_BEFORE=0x050d00
)

kde_enable_exceptions()

set(Grantlee5_MIN_PLUGIN_VERSION 0)

set(Grantlee5_MAJOR_MINOR_VERSION_STRING "${Grantlee5_VERSION_MAJOR}.${Grantlee5_VERSION_MINOR}" )

set(PLUGIN_INSTALL_DIR ${KDE_INSTALL_LIBDIR}/grantlee/${Grantlee5_MAJOR_MINOR_VERSION_STRING})

include("${Grantlee_MODULE_DIR}/GrantleeMacros.cmake")

if (BUILD_TEMPLATES)
  add_subdirectory(templates)
endif()

if (BUILD_TEXTDOCUMENT)
  add_subdirectory(textdocument)
endif()

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/Grantlee${Grantlee5_VERSION_MAJOR}" )

configure_file(
  "${Grantlee_MODULE_DIR}/Grantlee${Grantlee5_VERSION_MAJOR}Config.cmake.in"
  "${CMAKE_BINARY_DIR}/Grantlee${Grantlee5_VERSION_MAJOR}Config.cmake"
  @ONLY
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_BINARY_DIR}/Grantlee${Grantlee5_VERSION_MAJOR}ConfigVersion.cmake"
  VERSION ${Grantlee5_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  "${CMAKE_BINARY_DIR}/Grantlee${Grantlee5_VERSION_MAJOR}Config.cmake"
  "${CMAKE_BINARY_DIR}/Grantlee${Grantlee5_VERSION_MAJOR}ConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Common
)

install(FILES
  "${Grantlee_MODULE_DIR}/GrantleeMacros.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Common
)

install(EXPORT grantlee_targets
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  NAMESPACE Grantlee${Grantlee5_VERSION_MAJOR}::
  FILE GrantleeTargets.cmake
  COMPONENT Common
)

find_package(Doxygen)
if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
  message (STATUS "Doxygen and dot found. make docs target is available")
  # Add support for a docs target

  # Available from http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/libstdc++.tag
  set(STLTAGFILE "${CMAKE_SOURCE_DIR}/scripts/libstdc++.tag=http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen")
  find_file(qtcore_tag_file
    qtcore.tags
    HINTS
      "/usr/share/qt5/doc/qtcore"
    )
  set(QTTAGFILE "${qtcore_tag_file}=https://doc.qt.io/qt-5/")

  set(TAGFILES "${QTTAGFILE} ${STLTAGFILE}")
  set(DOCS_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  configure_file(
    "${Grantlee_MODULE_DIR}/Doxyfile.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
    @ONLY)

  # Add support for a Qt assistant docs target
  # qttags.tag is too big to distribute with Grantlee. Get it
  # from http://api.kde.org/kdereview-api/kdereview.tar.gz for example.
  if(EXISTS ${qtcore_tag_file})
    set(DOCS_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qtdocs")

    # Available from http://steveire.com/libstdc++-longnames.tag
    # Also recommended: http://www.steveire.com/libstdc++-v3.qch
    set(STLTAGFILE "${CMAKE_SOURCE_DIR}/scripts/libstdc++-longnames.tag=qthelp://org.gnu.libstdcxx/libstdc++-v3/")
    set(QTTAGFILE "${qtcore_tag_file}=qthelp://org.qt-project.qtcore/qtcore/")

    set(TAGFILES "${QTTAGFILE} ${STLTAGFILE}")
    set(GENERATE_QHP "yes")

    configure_file(
      "${Grantlee_MODULE_DIR}/Doxyfile.in"
      "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.qt"
      @ONLY)
  endif()

  set( DOCS_ARGS ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" )
  if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.qt")
    set(DOCS_ARGS ${DOCS_ARGS} COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.qt" )
  endif()

  add_custom_target(docs
    ${DOCS_ARGS}
  )
endif()

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)

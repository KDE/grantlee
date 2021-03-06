
add_library(Grantlee_TextDocument SHARED
  bbcodebuilder.cpp
  markupdirector.cpp
  plaintextmarkupbuilder.cpp
  texthtmlbuilder.cpp
  mediawikimarkupbuilder.cpp

  # Help IDEs find some non-compiled files.
  abstractmarkupbuilder.h
  grantlee_textdocument.h
  markupdirector_p.h
)
generate_export_header(Grantlee_TextDocument)
add_library(Grantlee::TextDocument ALIAS Grantlee_TextDocument)
set_property(TARGET Grantlee_TextDocument PROPERTY
  EXPORT_NAME TextDocument
)
target_compile_features(Grantlee_TextDocument
  PRIVATE
    cxx_auto_type
  PUBLIC
    cxx_override
)
if (CMAKE_GENERATOR MATCHES "Visual Studio")
  set_property(TARGET Grantlee_TextDocument PROPERTY DEBUG_POSTFIX "d")

  foreach(cfg ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${cfg} CFG)
    set_target_properties(Grantlee_TextDocument
      PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY_${CFG} "${CMAKE_CURRENT_BINARY_DIR}"
      RUNTIME_OUTPUT_DIRECTORY_${CFG} "${CMAKE_CURRENT_BINARY_DIR}"
      )
  endforeach()
endif()

target_link_libraries(Grantlee_TextDocument
  PUBLIC Qt5::Gui
)

set_target_properties(Grantlee_TextDocument PROPERTIES
  VERSION    ${Grantlee5_VERSION}
  SOVERSION  ${Grantlee5_VERSION_MAJOR}
)

install(TARGETS Grantlee_TextDocument EXPORT grantlee_targets
  RUNTIME DESTINATION ${KDE_INSTALL_BINDIR} COMPONENT TextDocument
  LIBRARY DESTINATION ${KDE_INSTALL_LIBDIR} COMPONENT TextDocument
  ARCHIVE DESTINATION ${KDE_INSTALL_LIBDIR} COMPONENT TextDocument
  INCLUDES DESTINATION ${KDE_INSTALL_INCLUDEDIR}
)

install(FILES
  abstractmarkupbuilder.h
  bbcodebuilder.h
  markupdirector.h
  plaintextmarkupbuilder.h
  texthtmlbuilder.h
  mediawikimarkupbuilder.h
  ${CMAKE_CURRENT_BINARY_DIR}/grantlee_textdocument_export.h
  DESTINATION ${KDE_INSTALL_INCLUDEDIR}/grantlee COMPONENT TextDocument
)

install(FILES
  grantlee_textdocument.h
  DESTINATION ${KDE_INSTALL_INCLUDEDIR} COMPONENT TextDocument
)

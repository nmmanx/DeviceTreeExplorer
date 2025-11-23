qt_add_executable(appDeviceTreeExplorer
    main.cpp
)

qt_add_qml_module(appDeviceTreeExplorer
    URI DeviceTreeExplorer
    VERSION 1.0
    QML_FILES
        Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appDeviceTreeExplorer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appDeviceTreeExplorer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE FALSE
)

add_subdirectory(parser)

target_link_libraries(appDeviceTreeExplorer
    PRIVATE Qt6::Quick
    dtparser
)

# Copy the dtparser library next to the executable after build
add_custom_command(
    TARGET appDeviceTreeExplorer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:dtparser> $<TARGET_FILE_DIR:appDeviceTreeExplorer>
    DEPENDS dtparser
)

include(GNUInstallDirs)
install(TARGETS appDeviceTreeExplorer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

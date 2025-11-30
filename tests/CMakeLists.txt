# Unit Test
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Test)

include(CTest)

qt_add_executable(testapp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/TreeItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/TreeItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/DeviceTreeModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/DeviceTreeModel.cpp
    TestDeviceTreeModel.cpp
)

add_custom_command(
    TARGET testapp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:dtparser> $<TARGET_FILE_DIR:testapp>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt6::Test> $<TARGET_FILE_DIR:testapp>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt6::Core> $<TARGET_FILE_DIR:testapp>
)

target_include_directories(testapp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_link_libraries(testapp PRIVATE
    Qt6::Core
    Qt6::Test
    dtparser
)

add_test(NAME testapp
         COMMAND testapp)

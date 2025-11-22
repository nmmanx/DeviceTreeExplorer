add_library(dtparser SHARED
    DeviceTree.cpp
    DeviceTreeSource.cpp
)

target_include_directories(dtparser PUBLIC include)

add_dependencies(dtparser flex_bison_tools)
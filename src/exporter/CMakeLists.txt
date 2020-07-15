# set source files
set(SOURCE_FILES
        main.cpp
        )

# add tracks directory
add_definitions(-DTRACKS_DIR=\"${PROJECT_SOURCE_DIR}/test/tracks\")

# create target
add_executable(simmap_exporter
        ${SOURCE_FILES}
        )

# link libraries
target_link_libraries(simmap_exporter PRIVATE
        odradapter
        )

# include directory
target_include_directories(simmap_exporter PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        )
project(spaceimpact)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp corals.cpp
                               shark.cpp starlayers.cpp)
enable_abcg(${PROJECT_NAME})

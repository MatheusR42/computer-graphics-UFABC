project(sharkker)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp corals.cpp
                               shark.cpp bubblelayers.cpp)
enable_abcg(${PROJECT_NAME})

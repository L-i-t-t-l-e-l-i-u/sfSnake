cmake_minimum_required(VERSION 3.12)

project(sfSnake)

#set(CMAKE_CXX_COMPILER D:/mingw64/bin/g++.exe)

add_executable(sfsnake main.cpp
Game.cpp 
Snake.cpp 
Fruit.cpp 
GameOverScreen.cpp
MenuScreen.cpp 
Screen.cpp 
SnakeNode.cpp 
GameScreen.cpp 
"Wall.h" 
"Wall.cpp"
"SettingScreen.cpp"
"SettingScreen.h" "RuleScreen.cpp" "RuleScreen.h")

find_package(SFML COMPONENTS graphics window system audio REQUIRED)

target_link_libraries(sfsnake PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)



file(COPY ${CMAKE_SOURCE_DIR}/wall.txt DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/Fonts DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/Music DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/Sounds DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/Images DESTINATION ${CMAKE_BINARY_DIR})
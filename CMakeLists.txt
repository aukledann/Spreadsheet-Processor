cmake_minimum_required(VERSION 3.24)
project(Big_Project_excel)

set(CMAKE_CXX_STANDARD 17)

add_executable(Big_Project_excel main.cpp CSpreadSheet.cpp CSpreadSheet.h CPos.cpp CPos.h MyParser.cpp MyParser.h)

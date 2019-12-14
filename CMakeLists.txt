cmake_minimum_required(VERSION 3.13)
project(HFMCODE)

set(CMAKE_CXX_STANDARD 11)

add_executable(HFMCODE main.cpp HFMNode.cpp HFMNode.h HFMTree.cpp HFMTree.h)
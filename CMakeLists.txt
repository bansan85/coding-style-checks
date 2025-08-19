cmake_minimum_required(VERSION 3.15)
project(CodingStyleCheck LANGUAGES CXX)

find_package(Clang REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG)

add_library(CodingStyleCheck MODULE MemberOrderCheck.cpp MemberOrderCheck.h)

target_include_directories(CodingStyleCheck PRIVATE ${CLANG_INCLUDE_DIRS}
                                                    ${LLVM_INCLUDE_DIRS})

message(STATUS ${CLANG_INCLUDE_DIRS})
message(STATUS ${LLVM_INCLUDE_DIRS})

# Gentoo needs LLVM clang clang-cpp
set(CL_LIBRARIES
    LLVM
    clang
    clang-cpp
    clangAST
    clangASTMatchers
    clangBasic
    clangFrontend
    clangTooling)
foreach(lib ${CL_LIBRARIES})
  if(TARGET ${lib})
    target_link_libraries(CodingStyleCheck PRIVATE ${lib})
  endif()
endforeach()

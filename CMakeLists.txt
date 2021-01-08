cmake_minimum_required(VERSION 3.15)
project(the_cure_language)

add_compile_options(-Wall -Wextra -pedantic -Wfloat-equal -fno-stack-protector)

set(CMAKE_CXX_STANDARD 11)

add_executable(the_cure_language
        main.cpp
        frontend/language.h
        frontend/frontend.cpp
        frontend/chooseTheCure.h
        frontend/lexer.cpp
        frontend/lexer.h
        dumps/dumps.cpp
        dumps/dumps.h
        frontend/frontend.h
        frontend/language.cpp
        backend/backend.cpp
        backend/backend.h)
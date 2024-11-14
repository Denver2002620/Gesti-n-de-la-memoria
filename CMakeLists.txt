cmake_minimum_required(VERSION 3.29)
project(Gesti_n_de_la_memoria C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Gesti_n_de_la_memoria
    main.c
        child.c
)

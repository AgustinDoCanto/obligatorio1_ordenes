cmake_minimum_required(VERSION 3.15)

project(Obligatorio1 VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(obligatorio1
    src/main.cpp
    src/lib/memplumber.cpp
    src/lib/catch_amalgamated.cpp

    src/ejercicios/arboles.cpp
    src/ejercicios/listas.cpp
    src/ejercicios/comienzo.cpp

    src/framework/cola_array.cpp
    src/framework/func_aux.cpp
    src/framework/func_aux_s.cpp
    
    src/pruebas/pruebas_arboles.cpp
    src/pruebas/pruebas_listas.cpp
    src/pruebas/pruebas_comienzo.cpp
    src/pruebas/mem_tracking_fixture.cpp

    src/reporters/grading_reporter.cpp
    src/reporters/readable_reporter.cpp
)

target_include_directories(obligatorio1 PRIVATE
    include
    include/lib
    include/ejercicios
    include/framework
    include/pruebas
)

target_compile_definitions(obligatorio1 PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
    CATCH_AMALGAMATED_CUSTOM_MAIN
)

# target_compile_options(obligatorio1 PRIVATE -fsanitize=address)
# target_link_options(obligatorio1 PRIVATE -fsanitize=address)

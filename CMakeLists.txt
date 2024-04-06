cmake_minimum_required(VERSION 3.0)
project(VIRT_CPU)

# Установка переменной со списком исходных файлов для статической библиотеки
set(SOURCES_STACK_LIB
    Stack/Stack.cpp
)

# Создание статической библиотеки
add_library(MyStack STATIC ${SOURCES_STACK_LIB})

# Указываем директорию, где находятся заголовочные файлы библиотеки
target_include_directories(MyStack PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Stack)

# Установка переменной со списком исходных файлов для исполняемого файла
set(SOURCES_CPU
    cpu/CPU_func.cpp
    cpu/CPU_main.cpp
)


set(SOURCES_ASM
    asm.v2/asm_func.cpp
    asm.v2/assembler.cpp
)



# Создание исполняемого файла и связывание его с нашей статической библиотекой
add_executable(cpu ${SOURCES_CPU})
target_include_directories(cpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Stack)

add_executable(asm ${SOURCES_ASM})
target_include_directories(asm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/asm.v2)

# Связывание исполняемого файла с нашей статической библиотекой
target_link_libraries(cpu PRIVATE MyStack)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
cmake_minimum_required(VERSION 3.0)
project(VIRT_CPU)

# Установка переменной со списком исходных файлов для статической библиотеки
set(SOURCES_STACK_LIB
    Stack/Stack.cpp
)

set(SOURCES_FILE_NAME_LIB
    filename/filename.cpp
)

# Создание статической библиотеки
add_library(MyStack STATIC ${SOURCES_STACK_LIB})
add_library(FileName STATIC ${SOURCES_FILE_NAME_LIB})

# Указываем директорию, где находятся заголовочные файлы библиотеки
target_include_directories(MyStack PUBLIC ${PROJECT_SOURCE_DIR}/Stack)
target_include_directories(FileName PUBLIC ${PROJECT_SOURCE_DIR}/filename)

# Установка переменной со списком исходных файлов для исполняемого файла
set(SOURCES_CPU
    cpu/CPU_func.cpp
    cpu/CPU_main.cpp
)

set(SOURCES_ASM
    asm.v2/asm_func.cpp
    asm.v2/assembler.cpp
)

set(SOURCES_MAKE
    make/make.cpp
)

# Создание исполняемого файла и связывание его с нашей статической библиотекой
add_executable(cpu ${SOURCES_CPU})
add_executable(asm ${SOURCES_ASM})
add_executable(make ${SOURCES_MAKE})


# Связывание исполняемого файла с нашей статической библиотекой
target_link_libraries(cpu PRIVATE MyStack FileName) 
target_link_libraries(asm PRIVATE FileName) 
target_link_libraries(make PRIVATE FileName) 

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
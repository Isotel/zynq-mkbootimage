cmake_minimum_required(VERSION 3.3)
project(zynq-mkbootimage VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

#Prevent in-source build
if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(SEND_ERROR "In-source builds are not allowed.")
endif ()

# Get the current working branch
execute_process(
        COMMAND git rev-parse --short HEAD
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE MKBOOTIMAGE_VER
        OUTPUT_STRIP_TRAILING_WHITESPACE
)


find_library(PCRE_LIBRARY pcre)
find_library(ELF_LIBRARY elf)

add_executable(mkbootimage
        src/arch/common.c src/arch/zynq.c src/arch/zynqmp.c
        src/file/bitstream.c src/file/elf.c
        src/bif.c src/bootrom.c src/common.c src/mkbootimage.c)

target_compile_options(mkbootimage PRIVATE $<$<C_COMPILER_ID:GNU>:-Wall -Werror>)
target_compile_options(mkbootimage PRIVATE $<$<C_COMPILER_ID:Clang>:-Wall -Werror>)

target_include_directories(mkbootimage PRIVATE
        "${PROJECT_SOURCE_DIR}/src")
target_compile_definitions(mkbootimage
        PRIVATE "MKBOOTIMAGE_VER=\"${MKBOOTIMAGE_VER}\"")
target_link_libraries(mkbootimage ${PCRE_LIBRARY} ${ELF_LIBRARY})

install(TARGETS mkbootimage
        RUNTIME DESTINATION bin)

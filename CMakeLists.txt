cmake_minimum_required(VERSION 3.6)
project(pal_traits VERSION 0.0.1 LANGUAGES CXX)

### compiler support
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include(CheckCXXCompilerFlag)
macro(pal_test_compiler_flag flag)
    string(REPLACE "-" "_" testname ${flag})
    string(REPLACE " " "_" testname ${testname})
    string(TOUPPER ${testname} testname)
    check_cxx_compiler_flag("${flag}" ${testname})
    if (${testname})
        list(APPEND PAL_COMPILER_FLAGS "${flag}")
    endif()
endmacro()

pal_test_compiler_flag("-Wall")
pal_test_compiler_flag("-Wextra")
pal_test_compiler_flag("-Wno-misleading-indentation")
pal_test_compiler_flag("-fdiagnostics-color=always")

### common properties
function(pal_set_properties target mode)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS NO)
    target_compile_definitions(${target} ${mode} $<$<CONFIG:Release>:NDEBUG>)
    target_compile_options(${target} ${mode} ${PAL_COMPILER_FLAGS})
endfunction()

### sigslot library
add_library(traits INTERFACE)
target_include_directories(traits INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties (traits PROPERTIES EXPORT_NAME Traits)
add_library(Pal::Traits ALIAS traits)

include(GNUInstallDirs)

install(TARGETS traits EXPORT PalTraitsConfig DESTINATION ${CMAKE_INSTALL_LIBDIR})
export(TARGETS traits NAMESPACE Pal:: FILE PalTraitsConfig.cmake)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pal)
install(EXPORT PalTraitsConfig NAMESPACE Pal:: DESTINATION "${CMAKE_INSTALL_DATADIR}/PalTraits/cmake")

enable_testing()
add_subdirectory(test EXCLUDE_FROM_ALL)


set(SOURCE_exe
tests.cpp)

add_executable(${PRO}_tests ${SOURCE_exe})

target_link_libraries(${PRO}_tests PUBLIC ${PRO}_lib)

target_link_libraries(${PRO}_tests PUBLIC gtest_main)

set_property(TARGET ${PRO}_tests PROPERTY FOLDER "calculator_tests")
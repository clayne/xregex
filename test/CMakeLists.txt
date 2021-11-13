
file(
    GLOB common_test_SRC
    "common/*.cpp"
)

add_executable(common_test
    ${common_test_SRC}
)

target_link_libraries(common_test
    common
    gtest
    gtest_main
    pthread
)

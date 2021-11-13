
file(
    GLOB common_SRC
    "common/*.cpp"
)

add_library(common SHARED
    ${common_SRC}
)
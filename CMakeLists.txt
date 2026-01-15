cmake_minimum_required(VERSION 3.28.3)
project(Tetris VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Export compile_commands.json file into 'build/Release' folder
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set binaary name
set(NAME tetris)

set(TETRIS_SOURCES
    main.cpp
    src/Game.cpp
    src/logs/Logs.cpp
    src/Board.cpp
    src/DropingEntity.cpp
    src/systems/BlockSpawner.cpp
    src/events/EventCollision.cpp
    src/events/EventSpawnBlock.cpp
    src/components/BlockId.cpp
    src/events/EventGameOver.cpp
    src/systems/LineDeletion.cpp
    src/events/EventInput.cpp
    src/events/EventLineComplete.cpp
    src/components/Tetromino.cpp
    src/TextFactory.cpp
)

include(FetchContent)

FetchContent_Declare(
    game_engine
    GIT_REPOSITORY https://github.com/yanisdolivet/RType-GameEngine
    GIT_TAG        release/tetris
)
FetchContent_MakeAvailable(game_engine)

# Disable macros of Windows
if(WIN32)
    add_compile_definitions(NOMINMAX)
endif()

# Options
option(ENABLE_COVERAGE "Enable code coverage instrumentation" OFF)
option(BUILD_TESTING "Build unit tests" OFF)

# Coverage settings
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Coverage enabled: Adding instrumentation flags globally")
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    else()
        message(WARNING "Coverage is only supported for GCC and Clang")
    endif()
endif()

# Find all package installed with Conan
find_package(raylib CONFIG REQUIRED)
find_package(GTest REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

if (BUILD_TESTING)
    # Find GTest package installed with Conan
    find_package(GTest CONFIG REQUIRED)
    enable_testing()
endif()

# Coverage target
if (ENABLE_COVERAGE AND BUILD_TESTING)
    find_program(GCOVR_EXECUTABLE gcovr)

    if (GCOVR_EXECUTABLE)
        add_custom_target(coverage
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure

            COMMAND ${GCOVR_EXECUTABLE}
                -r ${CMAKE_SOURCE_DIR}
                --exclude "tests/"
                --exclude "build/"
                --exclude ".*/conan/.*"

            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

            COMMENT "Running tests and generating coverage..."
        )
    else()
        message(WARNING "gcovr not found! Coverage target will not be available.")
    endif()
endif()

# Add executable
add_executable(${NAME} ${TETRIS_SOURCES})

target_include_directories(${NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/logs
    ${CMAKE_SOURCE_DIR}/include/events
    ${CMAKE_SOURCE_DIR}/include/systems
    ${CMAKE_SOURCE_DIR}/include/components
)

# Link libraries
target_link_libraries(${NAME} PRIVATE
    raylib
    GameEngine
    spdlog::spdlog
    fmt::fmt
)

# Set executable at root of project
set_target_properties(${NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# clang-format target
find_program(FORMAT_BIN clang-format)

if (FORMAT_BIN)
    add_custom_target(format
        COMMAND find src include
                -type f
                -name "*.*pp"
                -exec ${FORMAT_BIN} -i {} +

        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Formatting all source files using find..."
    )
else()
    message(WARNING "clang-format not found!")
endif()

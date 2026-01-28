cmake_minimum_required(VERSION 3.16)
project(portfolio_wt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(portfolio_wt
    src/main.cpp
    src/PortfolioApp.cpp
    src/ProjectPage.cpp
    src/Projects.cpp
    src/About.cpp
    src/Contact.cpp
    src/JsonUtils.cpp
    src/UIStrings.cpp
)

target_include_directories(portfolio_wt
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(portfolio_wt
    PRIVATE
        wt
        wthttp
)

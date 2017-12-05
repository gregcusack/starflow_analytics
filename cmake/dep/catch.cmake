
set(CATCH_VERSION 2.0.1)
file(DOWNLOAD
    https://github.com/catchorg/Catch2/releases/download/v${CATCH_VERSION}/catch.hpp
    ${CMAKE_HOME_DIRECTORY}/test/include/catch.h)


find_package(PkgConfig REQUIRED)
pkg_search_module(CPP_REDIS REQUIRED cpp_redis)
message("-- libcpp_redis ${CPP_REDIS_VERSION}:  ${CPP_REDIS_LIBRARIES}, ${CPP_REDIS_LIBRARY_DIRS}, ${CPP_REDIS_INCLUDE_DIRS}")
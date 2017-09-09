
find_package(PkgConfig REQUIRED)
pkg_search_module(PROTOBUF REQUIRED protobuf)
message("-- libprotobuf ${PROTOBUF_VERSION}: ${PROTOBUF_LIBRARY_DIRS}, include: ${PROTOBUF_INCLUDE_DIRS}")

add_executable(clfr_converter
        src/clfr_converter_main.cc
        src/kernels/benchmark_printer.h
        src/kernels/clfr_counter.h
        src/kernels/clfr_counter_replicated.h
        src/kernels/cloner.h
        src/kernels/microburst_detector.h
        src/kernels/microflow_reader.h
        src/kernels/tap.h)

target_include_directories(clfr_converter PUBLIC ${RAFT_INC})
target_link_libraries(clfr_converter raft)

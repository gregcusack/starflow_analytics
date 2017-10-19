
add_executable(clfr_driver
        src/clfr_driver_main.cc
        src/kernels/allstats_feature_calculator.h
        src/kernels/benchmark_printer.h
        src/kernels/clfr_counter.h
        src/kernels/clfr_counter_chain.h
        src/kernels/clfr_counter_replicated.h
        src/kernels/clfr_reader.h
        src/kernels/host_timing_profiler.h
        src/kernels/microburst_detector.h
        src/kernels/microflow_reader.h
        src/kernels/netflow_feature_calculator.h
        src/kernels/pfe_feature_calculator.h
        src/kernels/rich_feature_calculator.h)

target_include_directories(clfr_driver PUBLIC ${RAFT_INC})
target_link_libraries(clfr_driver raft)

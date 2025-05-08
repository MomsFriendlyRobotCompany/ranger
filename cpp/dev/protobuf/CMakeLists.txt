
LIST(APPEND DEVAPPS
    mcap-protobuf-read
    mcap-protobuf-write
)

foreach(MAIN ${DEVAPPS})
    add_executable(${MAIN} 
      ${MAIN}.cpp
      pose.pb.cc)

    target_include_directories(${MAIN}
      PUBLIC
        ${MCAP_INCLUDE_DIRS}
        ${Protobuf_INCLUDE_DIRS}
        ${PROTO_HDRS}
        ${CMAKE_CURRENT_BINARY_DIR})

    target_link_libraries(${MAIN}
      PUBLIC
        absl::log_internal_message
        absl::log_internal_check_op
        absl::status
        zstd
        protobuf
        lz4)
endforeach()
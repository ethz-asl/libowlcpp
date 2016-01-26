add_definitions(
  -DBOOST_TEST_DYN_LINK
  -DSAMPLE_DATA_DIR=${CMAKE_SOURCE_DIR}/data
  -DTEMPORARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
)

remake_add_executables(
  *run.cpp
  LINK
    logic
    io
    ${Boost_LIBRARIES}
  TESTING
)

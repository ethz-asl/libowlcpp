add_definitions(-DBOOST_TEST_DYN_LINK)

remake_add_executables(
  *run.cpp
  LINK
    rdf
    ${Boost_LIBRARIES}
  TESTING
)

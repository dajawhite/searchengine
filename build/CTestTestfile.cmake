# CMake generated Testfile for 
# Source directory: /Users/dajawhite/Desktop/searchengine
# Build directory: /Users/dajawhite/Desktop/searchengine/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(SimpleTest "tests")
set_tests_properties(SimpleTest PROPERTIES  _BACKTRACE_TRIPLES "/Users/dajawhite/Desktop/searchengine/CMakeLists.txt;24;add_test;/Users/dajawhite/Desktop/searchengine/CMakeLists.txt;0;")
add_test(HarderTest "/Users/dajawhite/Desktop/searchengine/build/catch_tests")
set_tests_properties(HarderTest PROPERTIES  _BACKTRACE_TRIPLES "/Users/dajawhite/Desktop/searchengine/CMakeLists.txt;27;add_test;/Users/dajawhite/Desktop/searchengine/CMakeLists.txt;0;")

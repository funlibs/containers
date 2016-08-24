cmake_minimum_required (VERSION 2.6)
project (Containers)

include_directories (.)

add_executable (containers_tests
  containers_tests.c
  fifo.h)

enable_testing()
add_test(NAME Fifo COMMAND containers_tests fifo)

cmake_minimum_required (VERSION 2.6)
project (Containers)

include_directories (.)

add_executable (fifo_test
  fifo_test.c
  fifo.h)

enable_testing()
add_test(NAME All COMMAND fifo_test)

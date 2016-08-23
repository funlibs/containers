cmake_minimum_required (VERSION 2.6)
project (FifoBuffer)

include_directories (.)

add_executable (fifo_buffer_test
  fifo_buffer_test.c
  fifo_buffer.h)

enable_testing()
add_test(NAME All COMMAND fifo_buffer_test)

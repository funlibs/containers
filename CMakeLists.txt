cmake_minimum_required (VERSION 2.6)
project (FifoBuffer)

include_directories (.)

add_executable (fifo_buffer_example
  fifo_buffer_example.c
  fifo_buffer.h)

cmake_minimum_required (VERSION 2.6)
project (Containers)

include_directories (.)

add_executable (containers_tests
  containers_tests.c
  queue.h
  stack.h)

enable_testing()
add_test(NAME Queue COMMAND containers_tests queue)
add_test(NAME Stack COMMAND containers_tests stack)

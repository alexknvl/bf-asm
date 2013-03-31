#!/bin/sh
  if [ $1 ] && [ $1 -eq 1 ];
  then make  && valgrind --leak-check=full --error-exitcode=255 \
  --suppressions=/testing/valgrind_suppressions.txt \
    /home/alex/Projects/PoissonRayTracer/core_tests --gtest_filter="-*Death*";
  else make  && /home/alex/Projects/PoissonRayTracer/core_tests;
  fi
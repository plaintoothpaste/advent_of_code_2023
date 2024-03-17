# Advent of code 2023

This is my solution folder for advent of code 2023. To run a particular solution with cpp use `build_and_run_cpp.ps1` followed by a day and part. Be aware this will also copy common cpp components into the desired folder.



# project structure

-  common: A collection of commonly used resources eg file parsing and loading
   -  cpp: common C++ components, included natively using cmake lists. linting and formatting files and presets are coppied into each project, ignored by git.
-  <day number>: each days directory
   -  cpp: project language, may be more in the future
      -  <part>: part 1 or 2
         -  main.cpp
         -  CMakeLists.txt
   -  instructions.md: optional, recommended to add the answers once obtained to here, however this is optional
   -  example.txt: an example input text file.
   -  input.txt: the full text file
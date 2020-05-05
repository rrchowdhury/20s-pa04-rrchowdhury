cmake_minimum_required(VERSION 3.10)
project(20S-3353-PA04)
set (EXE_NAME PA04)

###########################################
# Students, edit the items below.  In the quotes,
# put the names of the input files as they should appear
# in the argv param to main.
# Example:
#   set (input01 "train_data.csv")
#   set (input02 "train_target.csv")
# etc....

set (input01 "input01.txt")
set (input02 "input02.txt")
set (input03 "input03.txt")
set (input04 "input04.txt")
set (input05 "input05.txt")
set (input06 "input06.txt")
set (input07 "input07.txt")
set (input08 "input08.txt")
set (input09 "input09.txt")
set (input10 "input10.txt")
set (input11 "input11.txt")
set (input12 "input12.txt")
set (input13 "input13.txt")
set (input14 "input14.txt")
set (input15 "input15.txt")
set (input16 "input16.txt")
set (input17 "input17.txt")
set (input18 "input18.txt")


##
#########################################

# Copy data files to build dir
foreach(file IN LISTS input01 input02 input03 input04 input05 input06 input07 input08 input09 input10 input11 input12 input13 input14 input15 input16 input17 input18)
    configure_file(${file} ${file} COPYONLY)
endforeach()

set(CMAKE_CXX_STANDARD 14)
add_executable(${EXE_NAME} main.cpp Graph.cpp Tester.cpp)

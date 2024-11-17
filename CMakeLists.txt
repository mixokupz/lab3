cmake_minimum_required(VERSION 3.5.0)
project(lab3_new VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
add_executable(lab3_new main.cpp Conf_Parser.cpp Sound_Proc.cpp Wav_Parser.cpp Converters.cpp Wav_Writer.cpp)


mkdir ./debug

cmake -S . -B ./debug -D CMAKE_BUILD_TYPE=Debug && cmake --build ./debug

echo 'done compiling'



rm -rf ./build && mkdir ./build

cd ./build/
cmake .. && cmake --build .

echo 'done compiling'

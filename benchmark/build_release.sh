set -e

rm -rf bin
mkdir -p bin

cd ..
rm -rf build
mkdir -p buld
cd build

cmake ..
cmake --build . --config Release

cp cthulhu ../benchmark/bin

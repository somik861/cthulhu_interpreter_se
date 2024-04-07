python -m venv .env

.env\Scripts\Activate.ps1
pip3 install conan --upgrade

conan profile detect --name cthulhu --force

conan install . --output-folder=build --build=missing -pr:h=cthulhu -pr:b=cthulhu -s build_type=Debug -s compiler.cppstd=23
cmake --preset conan-default

deactivate

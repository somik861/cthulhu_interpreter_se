# Cthulhu interpreter with Somik's Extensions

## Description
This is 3rd party interpreter for *cthulhu* language that is currently being developed at Masaryk University Faculty of Informatics in Brno as a 'compile to' language used for formal verification.

## Important notes about compatibilty and requirements
All docs, definitions and rules that I have created (are inside this repo) are based on original *cthulhu*. Altough I will try to keep things compatible with official language, this interpreter might behave differently and may accept/reject situation's that are forbidden/allowed in the official language definition.

## OS support
The interpreter is compilable on Windows as well as on Linux with compiler providing c++20 standard.

## Compilation
The only thing you need to do is run cmake and build the project. System agnostic command below:
```
mkdir build
cmake ..
cmake --build . --config Release
```

## Up-to-dateness
Well, at this time, the docs are well ahead of the provided functionality. So if something should work and it does not, that it is probably not implemented :D.
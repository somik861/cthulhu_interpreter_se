# Interpreter

There are 4 interpreter modes:
* debug
* normal
* fast
* parallel

## Debug
Pauses after each execution step and gives you information about the program state. This is helpfull when debuggin your code. Apart from that, it will behave the same as *normal*

## Normal (default)
Runs code and produces results. This mode tries to check restrictions provided in documentation and emits human-friendly error message when program does something forbidden.

## Fast
This is (most of the time) significantly faster than *normal*. On the other, this mode does not check any restrictions. When you do something invalid, it may just work, or it may crash without anything helpfull to tell you. Try to run *normal* mode, if you encounter wierd behaviour using this mode to check errors in your code.

## Parallel
Executetes cthulhu_se threads as a real threads. This may increase performance of some programs. Please note that executing real threads takes significantly more overhead than running everyhing in one thread, therefore, do not use this mode as *your default*. Similarly as *fast* mode, this mode does not check any restrictions and will crash horribly when given invalid instruction. You can se maximum number of threads using `--max_threads=N`.

# Command line options
`./cthulhu_se source_code`

* `--imode=MODE` sets interpreter mode (default: normal)
* `--max_threads=N` limits max number of threads in parallel mode (default: your CPU core count (including HT)).
* `--read_result=N` instead of printing final state, read a number from stack **N** and print it as a result.
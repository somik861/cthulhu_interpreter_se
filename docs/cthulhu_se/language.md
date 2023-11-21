# Overview

The main building blocks of the whole language are:
* dictionary
* stack
* instruction
* word
  
## Word
32 bit unsigned int

## Instruction
Each instruction consists of <domain_name> <operation> <operands>.

For example: `word add 1 2 3` (adds words from stack 1 and 2 and pushes the result to stack 3, see [domain_docs](domains.md)).

## Stack
Container for elements.

Element can be:
* dictionary
* stack
* instruction
* word

## Dictionary
Maps *word* to *stack*.

### state dicitonary
Is a special type of *dictionary*.
It is used for executing the program. The instructions are taken from stack at key 0.
This type of dictionary must always contain exactly these keys: 0, 1, 2, 3, 4, 5
Initional dictionary (program start) is called **init**.


# Program syntax
Indentation does not have any meaning.

Comments are denoted using \#. (note that only whole-line comments are supported).

Sample self-teaching program
```
# Comments are always on whole line

# specify domains to be used (line starting with .struct)
.struct bv32 builtin
# we specified that we want to use bv32 domain from builtin
# domain specification must always be first thing in the source code (except from comments)

# we can create stack using its name and double dot
stack1:
    # word literals has dolar prefix
    $15
    # other stacks (as an elements in this stack) are prefixed with @
    @empty
    # dictionary (as an element in the stack) is prefixed with %
    %empty

# stack can be also empty
empty:

# we can create dicitionary using its name prefixed with % and double dot
%empty:

# each program must contain 'init' dictionary
# note that pushing anything to stack at key 1
# does not change stack at other keys.
%init:
    # word => @stack_name
    0 => @stack1
    1 => @empty
    2 => @empty
    3 => @empty
    4 => @empty
    5 => @empty
```

# Domains

## Default domains
These domains are always available and does not need to be specified at the start of the source code.
* `stck` (short for stack)
* `dict` (short for dictionary)
* `word`
* `instr` (short for instruction)

## Builtin domains
These are domains that can be included using:
```
.struct <domain_name> builtin
```
They are:
* `bv32` - 32 bit long bitvector


## Operations
Arguments are always specified in source code and denote keys in *state-dictionary*
To define define the semantic meaning of operation, we use following syntax:
```
xyz add A B -> C # operation add of domain xyz with 2 input arguments and 1 output argument
    pop A as X # pop element from stack A and save it as X
    pop B as Y # pop element from stack B and save it as Y
    push X + Y to C # push element computed from X + Y to stack C

xyz complex A B C -> D # more complex operation (without usefull meaning)
    pack [A] as X # take the whole stack A and save it as X
    unpack X to [A] # replace stack A with stack X
    pop A as Y
    fetch X[Y] as Z # fetch key Y of dictionary in X as denote it Z
```

### Important restritions and informations
All *push* (and similar) operations pushes copies of the object (the language has pass-by-value sematic).

All operands must be distinct. This means instruction `xyz add 1 1 2` is invalid. (`xyz add 1 2 3` is the valid alternative).

### Generic operations
These has to be defined on all domains. Note: you still need to specify the domain for type correctness checks.
We denote this domain as **gen** (short for generic).
```
gen move A -> B
    pop A as X
    push X to B

gen dup A -> B C
    pop A as X
    push X to B
    push X to C

gen swap A B
    pop A as X
    pop B as Y
    push Y to A
    push Y to B

gen drop A
    pop A
```

### Arithmetic operations
We denote this domain as **arth** (short for arithmetic).
```
arth add A B -> C
    pop A as X
    pop B as Y
    push X + Y to C

arth sub A B -> C
    pop A as X
    pop B as Y
    push X + Y to C

arth mul A B -> C
    pop A as X
    pop B as Y
    push X + Y to C

arth div A B -> C
    pop A as X
    pop B as Y
    push X + Y to C

arth mod A B -> C
    pop A as X
    pop B as Y
    push X + Y to C
```
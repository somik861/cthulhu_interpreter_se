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
Arguments are always specified in source code.
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
    get X[Y] as Z # fetch key Y of dictionary in X as denote it Z
    put Z to X[Y] # store stack to dictionary X at key Y
```

### Important restritions and informations
All *push* (and similar) operations pushes copies of the object (the language has pass-by-value sematic).

All key-operands must be distinct. This means instruction `xyz add 1 1 2` is invalid. (`xyz add 1 2 3` is the valid alternative).

### Generic domains
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

gen zero A
    push empty_elem to A # empty element can be for example empty stack/dict or 0
    # note that empty element for instruction is 'instr:noop'
```

### Generatable domains
We denote the domain as **gen**.
```
gen const _16bit_ A:
    push _16bit_ A # _16bit_ is any number from 0 <= x < 2**16
```

### Arithmetic domains
We denote the domain as **arth**.
```
arth add A B -> C
    pop A as X
    pop B as Y
    push X + Y to C

arth sub A B -> C
    pop A as X
    pop B as Y
    push X - Y to C

arth mul A B -> C
    pop A as X
    pop B as Y
    push X * Y to C

arth div A B -> C
    pop A as X
    pop B as Y
    push X / Y to C

arth mod A B -> C
    pop A as X
    pop B as Y
    push X % Y to C
```

### Equitable domains
Must be generateble. We denote the domain as **eql**.

```
eql eq A B -> C
    pop A as X
    pop B as Y
    if (X == Y)
        'eql const 1 C'
    else
        'eql zero C'

eql neq A B -> C
    pop A as X
    pop B as Y
    if (X != Y)
        'eql const 1 C'
    else
        'eql zero C'
```

### Orderable domains
Must be generateble. We denote the domain as **ord**.
```
ord lt A B -> C
    pop A as X
    pop B as Y
    if (X < Y)
        'ord const 1 C'
    else
        'ord zero C'

ord le A B -> C
    pop A as X
    pop B as Y
    if (X <= Y)
        'ord const 1 C'
    else
        'ord zero C'

ord gt A B -> C
    pop A as X
    pop B as Y
    if (X > Y)
        'ord const 1 C'
    else
        'ord zero C'

ord ge A B -> C
    pop A as X
    pop B as Y
    if (X >= Y)
        'ord const 1 C'
    else
        'ord zero C'
```

### Bit domains
We denote the domain as **bit**.
```
bit and A B -> C
    pop A as X
    pop B as Y
    push X & Y to C

bit xor A B -> C
    pop A as X
    pop B as Y
    push X ^ Y to C

bit or A B -> C
    pop A as X
    pop B as Y
    push X | Y to C

bit neg A -> B
    pop A as X
    push ~X to B
```

### Guardable domains
Must be Equitable. We denote the domain as **guard**.

```
guard guard_zero A
    pop A as X
    'guard zero A'
    pop A as Y
    if (X != Y)
        kill current thread

guard guard_nonzero A
    pop A as X
    'guard zero A'
    pop A as Y
    if (X == Y)
        kill current thread
```

### Logical domains
Must be Equitable. We denot the domain as **logic**.
Element is log.1 iff elem != elem_zero;
The result log.0 == elem_zero; log.1 == elem const 1;

```
logic land A B -> C
    pop A as X
    pop B as Y
    push X && Y to C

logic lor A B -> C
    pop A as X
    pop B as Y
    push X || Y to C
```

# Dict

Does not belong to any instruction group (apart from generic)

Special operations:
```
fetch A B C -> D E
    pack [A] as X
    pop B as Y
    pop C as Z
    unpack Y[Z] to [A]
    push Y[~Z] to D # Y without key Z
    push X to E

store A B C D -> E
    pack [A] as X
    pop B as Y
    pop C as Z
    put X to Y[Z]
    pop D as W
    unpack W to [A]
    push [Y] to E
```
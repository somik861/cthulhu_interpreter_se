# Stck

Does not belong to any instruction group (apart from generic)

Special operations:
```
exec A
    pop A as X
    execute new thread with instruction stack replaced with X

pivot A B -> C
    pack [A] as X
    pop B as Y
    unpack Y to [A]
    push X to C
```
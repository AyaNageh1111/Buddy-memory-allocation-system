# Buddy memory allocation system
The app uses linked list implementation as a tree, to store data as
each parent has size 2n space and each one of his two children has n space,
and so on. If the OS needs a size smaller than n, it will separate the 2n to
two childs each has space of n and store in the first child.

## Algorithm Assumptions:
The data of each process is stored in one line in a file.

### Input Example:

Order      | ID | Size
Allocate   | 1  | 200
Allocate   | 2  | 30
Allocate   | 3  | 55
Deallocate | 2  |
Deallocate | 3  |
Allocate   | 4  | 14
Deallocate | 1  |
Deallocate | 4  |
Allocate   | 5  |255
Allocate   | 6  |150
Deallocate | 5  |
Deallocate |6   |

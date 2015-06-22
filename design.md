# design

## architecture

```
computer = processor { processor }
processor = message_queue program_memory instruction_executer
```


## behavior

### launch of processors

### dispatch of a program

### execution of a program

### termination of processors


## instruction set

name     | id prefix | src | dest | id
---------|----------:|----:|-----:|---:
copy     |         0 |   1 |    2 |  1
dirty    |         0 |   1 |    1 |  2
bit not  |         1 |   1 |    1 |  1
bit and  |         1 |   2 |    1 |  2
bit or   |         1 |   2 |    1 |  3
bit xor  |         1 |   2 |    1 |  4
bool not |         2 |   1 |    1 |  1
bool and |         2 |   2 |    1 |  2
bool or  |         2 |   2 |    1 |  3
bool xor |         2 |   2 |    1 |  4
int +    |         2 |   2 |    1 |  5
int *    |         2 |   2 |    1 |  6
int -x   |         2 |   1 |    1 |  7
int 1/x  |         2 |   1 |    1 |  8
int ==   |         2 |   2 |    1 |  9
real +   |         3 |   2 |    1 |  1
real *   |         3 |   2 |    1 |  2
real -x  |         3 |   1 |    1 |  3
real 1/x |         3 |   1 |    1 |  4
real ==  |         3 |   2 |    1 |  5

* int's 1/x is defined in GF
  * difficult to implement
* definition of virtual bool type
  * false := 0b0..0
  * true /= false
  * virtual bool type has same expression as int
* dirty
  * take an id of syscall and its arguments as a stream
  * read command line arguments by dirty
  * sync by dirty


## program

```
program_file = symbol header program

symbol = "JACK"

header = header_size program_position program_size
header_size = Word
program_position = Word
program_size = Word

program = initial_messages nodes

initial_messages = message { message }
message = { dest value }

nodes = node { node }
node = node_id node_struct
node_struct = one_dest_node_struct | two_dest_node_struct
one_dest_node_struct = instruction_id presence_bit src dest
two_dest_node_struct = instruction_id dest dest

src = Word
dest = node_id

Word = uint64_t
Byte = uint8_t
```


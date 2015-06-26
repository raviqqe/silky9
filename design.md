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

name         | id prefix | src | dest | id
-------------|----------:|----:|-----:|---:
copy         |         0 |   1 |    2 |  1
dirty        |         0 |   1 |    1 |  2
bit not      |         1 |   1 |    1 |  1
bit and      |         1 |   2 |    1 |  2
bit or       |         1 |   2 |    1 |  3
bit xor      |         1 |   2 |    1 |  4
bool not     |         1 |   1 |    1 |  9
bool and     |         1 |   2 |    1 | 10
bool or      |         1 |   2 |    1 | 11
bool xor     |         1 |   2 |    1 | 12
int ==       |         2 |   2 |    1 |  1
int <0       |         2 |   1 |    1 |  2
int +        |         2 |   2 |    1 |  3
int *        |         2 |   2 |    1 |  4
int -x       |         2 |   1 |    1 |  5
int 2^x      |         2 |   1 |    1 |  6
int log2(x)  |         2 |   1 |    1 |  7
real ==      |         3 |   2 |    1 |  1
real <0      |         3 |   1 |    1 |  2
real +       |         3 |   2 |    1 |  3
real *       |         3 |   2 |    1 |  4
real -x      |         3 |   1 |    1 |  5
real 2^x     |         3 |   1 |    1 |  6
real log2(x) |         3 |   1 |    1 |  7
real 1/x     |         3 |   1 |    1 |  8

* int's 1/x seems not to be defined
* definition of virtual bool type
  * false := 0b0..0
  * true /= false
  * virtual bool type has same expression as int
* dirty
  * take an id of syscall and its arguments as a stream
  * read command line arguments by dirty
  * sync only by dirty and sync only for dirty

* bigger and smaller instuctions for int and real?


## program

```
program_file = symbol header program

symbol = "JACK"

header = word_size num_of_initial_messages num_of_nodes
word_size = Byte
num_of_initial_messages = Word
num_of_nodes = Word

program = initial_messages nodes

initial_messages = message { message }
message = dest value

nodes = node { node }
node = one_dest_node | two_dest_node
one_dest_node = instruction src dest
two_dest_node = instruction dest dest
src = Word
dest = Word

instruction = instruction_id instruction_prefix presence_bit
instruction_id = uint5_t
instruction_prefix = uint2_t
presence_bit = uint1_t

src = Word
dest = Word # index to a node

Word = uint64_t
Byte = uint8_t
```

# Design

## Architecture

```
computer = processor { processor }
processor = token_queue program_memory instruction_executer
```


## Behavior

### launch of processors

### dispatch of a program

### execution of a program

### termination of processors


## Instruction set

name         | id prefix | id | operands | dests
-------------|----------:|---:|---------:|------:
copy         |         0 |  0 |        1 |     2
io           |         0 |  1 |        1 |     1
bit not      |         1 |  0 |        1 |     1
bit and      |         1 |  1 |        2 |     1
bit or       |         1 |  2 |        2 |     1
bit xor      |         1 |  3 |        2 |     1
bool not     |         1 |  4 |        1 |     1
bool and     |         1 |  5 |        2 |     1
bool or      |         1 |  6 |        2 |     1
bool xor     |         1 |  7 |        2 |     1
int ==       |         2 |  0 |        2 |     1
int <0       |         2 |  1 |        1 |     1
int +        |         2 |  2 |        2 |     1
int *        |         2 |  3 |        2 |     1
int -x       |         2 |  4 |        1 |     1
int 2^x      |         2 |  5 |        1 |     1
int log2(x)  |         2 |  6 |        1 |     1
real ==      |         3 |  0 |        2 |     1
real <0      |         3 |  1 |        1 |     1
real +       |         3 |  2 |        2 |     1
real *       |         3 |  3 |        2 |     1
real -x      |         3 |  4 |        1 |     1
real 2^x     |         3 |  5 |        1 |     1
real log2(x) |         3 |  6 |        1 |     1
real 1/x     |         3 |  7 |        1 |     1

* int's 1/x seems not to be defined
* definition of virtual bool type
  * false := 0b0..0
  * true /= false
  * virtual bool type has same expression as int
* io
  * take an id of syscall and its arguments as a stream
  * read command line arguments by io
  * sync only by io and sync only for io
  * shutdown by io?

* bigger and smaller instuctions for int and real?
* io's operandBuff = fd?


## Program

```
program_file = symbol word_size program

symbol = "JACK"
word_size = Byte # (number of bits) / 8

program = num_of_nodes nodes num_of_initial_tokens initial_tokens

num_of_nodes = Int
num_of_initial_tokens = Int

nodes = node { node }
node = one_dest_node | two_dest_node
one_dest_node = instruction operand dest
two_dest_node = instruction dest dest
operand = Word
dest = Int

instruction = operand_presence_bit instruction_prefix instruction_id
operand_presence_bit = uint1_t
instruction_prefix = uint2_t
instruction_id = uint5_t

initial_tokens = token { token }
token = dest value
dest = Int
value = Word

Byte = uint8_t
Int = uint64_t
Real = double # Real type must have the same endianness as Int type
Word = Int | Real
```

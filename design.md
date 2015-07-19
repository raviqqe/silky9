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

### Instruction categories

name    | id
--------|---:
control |  0
logic   |  1
int     |  2
real    |  3

### Instructions taking constant and variable operands

category | name          |            id
---------|---------------|--------------:
control  | shutdown      |             0
control  | copy          |             1
control  | input         |             2
control  | output        |             3
logic    | bit and       |             0
logic    | bit or        |             1
logic    | bit xor       |             2
logic    | bool and      |             3
logic    | bool or       |             4
logic    | bool xor      |             5
int      | ==            |             0
int      | x&gt;c        |             1
int      | x&lt;c        |             2
int      | +             |             3
int      | \*            |             4
int      | c-x           |             5
int      | x-c           |             6
int      | c/x           |             7
int      | x/c           |             8
int      | x^c           |             9
int      | c^x           |            10
int      | log\_c(x)     |            11
int      | log\_x(c)     |            12
real     | (same as int) | (same as int)

### Instructions taking 2 variable operands

category | name          |            id
---------|---------------|--------------:
logic    | bit and       |             0
logic    | bit or        |             1
logic    | bit xor       |             2
logic    | bool and      |             3
logic    | bool or       |             4
logic    | bool xor      |             5
int      | ==            |             0
int      | +             |             1
int      | \*            |             2
real     | (same as int) | (same as int)

* shutdown instruction doesn't take any constant operand
* do the not operations by xor with 1
* int's 1/x seems not to be defined
* definition of bool type
  * false := 0b0..0
  * true /= false
  * bool type has same expression as int type
* input / output
  * take a file descriptor as an constant operand
  * read / write one byte reprensented in int type

* bigger and smaller instuctions for int and real?


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

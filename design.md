# design

## architecture

```
computer = processor { processor }
processor = token_queue node_memory instruction_executer
```


## Behavior

### launching processors

### loading a program

### execution of a program

### termination of processors


## instruction set

### opcode categories

name    | id
--------|---:
control |  0
logic   |  1
int     |  2
real    |  3

### opcodes taking constant and variable operands

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

### opcodes taking 2 variable operands

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
* do the not operations by xor
* int's 1/x seems not to be defined
* definition of bool type is;
  * false := 0b0..0
  * true /= false
  * bool type has same expression as int type
* input / output
  * take a file descriptor as an constant operand
  * read / write one byte reprensented in int type

* bigger and smaller instuctions for int and real?


## program

```
program_file = symbol word_size program

symbol = "JACK"
word_size = s9_byte_t # (number of bits) / 8

program = num_of_nodes nodes num_of_initial_tokens initial_tokens

num_of_nodes = s9_int_t
num_of_initial_tokens = s9_int_t

nodes = node { node }
node = header ( stock_operand | constatnt_operand ) dest
stock_operand = operand
constant_operand = operand
operand = s9_word_t
dest = s9_int_t

header = operand_presence_bit instruction_prefix instruction_id
operand_presence_bit = uint1_t
instruction_prefix = uint2_t
instruction_id = uint5_t

initial_tokens = token { token }
token = dest value
dest = s9_int_t
value = s9_word_t

s9_byte_t = uint8_t
s9_int_t = uint64_t
s9_real_t = double
  # s9_real_t type must have the same endianness as s9_int_t type has.
s9_word_t = s9_int_t | s9_real_t
```

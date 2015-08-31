# design

## architecture

```
computer = processor { processor } memory
processor = instruction_queue instruction_executer
```


## instruction set

### opcode categories

name    | id
--------|---:
control |  0
logic   |  1
int     |  2
real    |  3


### opcodes taking constant and variable operands

category | name      | id
---------|-----------|---:
control  | nop       |  0
control  | copy      |  1
control  | cas       |  2
control  | jump      |  3
control  | fork      |  1
control  | exit      |  1
control  | input     |  2
control  | output    |  3
logic    | bit and   |  0
logic    | bit or    |  1
logic    | bit xor   |  2
logic    | bool and  |  3
logic    | bool or   |  4
logic    | bool xor  |  5
int      | ==        |  0
int      | x&gt;c    |  1
int      | x&lt;c    |  2
int      | +         |  3
int      | \*        |  4
int      | c-x       |  5
int      | x-c       |  6
int      | c/x       |  7
int      | x/c       |  8
int      | x^c       |  9
int      | c^x       | 10
int      | log\_c(x) | 11
int      | log\_x(c) | 12
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

* halt instruction doesn't take any constant operand
* do the not operations by xor
* int's 1/x seems not to be defined
* definition of bool type is;
  * false := 0b0..0
  * true /= false
  * bool type has same expression as int type
* input / output
  * take a file descriptor as an constant operand
  * read / write one byte reprensented in int type


## program

```
program_file = function_offset_table funtion { function }
function_offset_table = number_of_entries Int { Int }
number_of_entries = Int
function = code constant initialized_frame
code = Byte { Byte }
constant = Word { Word }
initialized_frame = frame
frame = Word { Word }

Byte = uint8
Int = uint32 | uint64
Real = float32 | float64
Word = Int | Real
```

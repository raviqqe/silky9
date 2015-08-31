package s9

import (
  "log"
  "os"
  "runtime"
  "unsafe"
)


// Constants

const BYTES_PER_WORD = 8
const BITS_PER_BYTE = 8
const FRAME_SIZE = 256 // 2 ^ BITS_PER_BYTE
const TOKEN_QUEUE_BUFFER_SIZE = 256


// Types

//// Primitives

type Byte uint8
type Int uint64
type Real float64
type Pointer unsafe.Pointer

////// Word

type Word Int
const DUMMY_WORD = Word(0xDEADCAFFEBADBEEF)

func (word *Word) storeInt(intNum Int) {
  *word = Word(intNum)
}

func (word *Word) storeReal(realNum Real) {
  type IntPointer *Int
  *word = Word(*IntPointer(unsafe.Pointer(&realNum)))
}

func (word *Word) storePointer(pointer Pointer) {
  *word = Word(Int(uintptr(pointer)))
}

func (word *Word) AsInt() Int {
  return *word
}

func (word *Word) AsReal() Real {
  return *word
}

func (word *Word) AsPointer() Pointer {
  return *word
}

func WordFromBytes(bytes [BYTES_PER_WORD]byte) Word {
  // bytes represents a word in big endian
  intNum := Int(bytes[0])

  for _, byteNum := range bytes[1:] {
    intNum <<= BITS_PER_BYTE
    intNum += byteNum
  }

  return Word(intNum)
}


//// Architecture

type Frame [FRAME_SIZE]Word


type Function {
  code []Byte
  constant []Word
  initialFrame Frame
}


type Program struct {
  funtions []Function
}


type InstructionPointer struct {
  functionIndex Int
  instructionOffset Int
}

type (instructionPointer *InstructionPointer) Add(offset Int) {
  // offset may be negative
  instructionPointer.instructionOffset += offset
}


type Token struct {
  instructionPointer InstructionPointer
  framePoiner *Frame
}

func NewToken(instructionPointer InstructionPointer, framePointer *Frame)
    Token {
  return Token{
    instrctionPointer : instructionPointer,
    framePointer : framePointer,
  }
}


type TokenQueue chan Token

func NewTokenQueue() TokenQueue {
  return make(TokenQueue)
}

func (tokenQueue TokenQueue) Send(token Token) {
  tokenQueue <- token
}

func (tokenQueue TokenQueue) Receive() Token {
  return <- tokenQueue
}


type Processor struct {
  tokenQueue chan Token
  instructionPointer *Byte
  framePointer *Frame
}

func NewProcessor() Processor {
  return Processor{
    tokenQueue: make(TokenQueue, TOKEN_QUEUE_BUFFER_SIZE),
  }
}

type Computer struct {
  processors []Processor
  program Program
}

func NewComputer() Computer {
  processors := make([]Processor, runtime.GOMAXPROCS())

  for index, _ := range processors {
    processors[index] = NewProcessor()
  }

  return Computer{processors: processors}
}

func (computer *Computer) ExecuteProgram(filename string) error {
  err := computer.loadProgram(filename)
  if err != nil {
    log.Println("Failed to load program.")
    return err
  }

  for processor := range computer.processors {
    go processor.launch()
  }

  processor[0].

  return nil
}


func (computer *Computer) loadProgram(filename string) error {
  file, err := os.Open(filename)
  defer file.Close()
  if err != nil {
    return err
  }

  numOfEntries, err := file.ReadWord()
  if err != nil {
    log.Fatalln("File size is too small.")
  }

  for entry := 0; entry < numOfEntries.AsInt(); entry++ {
    offset, err := file.Seek(len(numOfEntriesInBytes)
                             + entry * len(offsetInBytes, 0)
    if offset != {
      log.Fatalln("File size is too small.")
    }

    numOfBytesRead, err := file.Read(bytes)
    if numOfBytesRead != len(offsetInBytes) {
      log.Fatalln("File size is too small.")
    }
  }
}


// functions

func (file *File) ReadWord() Word, error {
  buffer := make([]byte, BYTES_PER_WORD)

  numOfBytesRead, err := file.Read(buffer)
  if err != nil {
    return DUMMY_WORD, err
  }

  return WordFromBytes(buffer), nil
}

func (file *File) ReadFunction(offset int64, size int64) Function, error {
  File.Seek(offset, 0)
  offsetOfCode, err := file.ReadWord()
  if err != nil {
    log.Println("Failed to read offset of code of a function.")
    return DUMMY_WORD, err
  }

  return WordFromBytes(buffer), nil
}

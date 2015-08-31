package main

import (
  "fmt"
  "log"
  "os"
  "runtime"
  "./s9"
)


func init() {
  log.SetOutput(os.Stderr)
  log.SetPrefix("SILKY9:")
}


func main() {
  if len(os.Args) != 2 {
    log.Fatalln("Usage:", os.Args[0], "<program file>")
  }

  computer := s9.NewComputer()
  computer.ExecuteProgram(os.Args[1])
}

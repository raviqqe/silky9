#!/bin/sh

# constants

DEBUG=false
DEFAULT_NUM_OF_PROCS=1
MPIRUN_LOG_FILE_PREFIX="silky9_log"
SILKY9_BIN=s9
USAGE="Usage: $0 [-n <numOfProcs>] <programFile>"


# functions

err() {
  echo "$@" >&2
}

fail() {
  if [ $# -gt 0 ]
  then
    err "$@"
  fi
  exit 1
}

debug() {
  if $DEBUG
  then
    err "s9.sh:DEBUG:" "$@"
  fi
}


# main routine

numOfProcs=$DEFAULT_NUM_OF_PROCS

## process command line arguments

while getopts dn: option
do
  case $option in
  d)
    DEBUG=true
    ;;
  n)
    if ! [ "$OPTARG" -gt 0 ]
    then
      fail "$USAGE"
    fi
    numOfProcs=$OPTARG
    ;;
  \?)
    fail "$USAGE"
    ;;
  esac
done
shift $(expr $OPTIND - 1)

if [ $# -ne 1 ]
then
  fail "$USAGE"
elif ! [ -f "$1" ]
then
  fail "no such file, $1"
fi

programFileName=$1

debug "numOfProcs = $numOfProcs"

## run silky 9

if $DEBUG
then
  mpirunFlags="-output-filename $MPIRUN_LOG_FILE_PREFIX"
else
  mpirunFlags="-q"
fi

if ! [ -f "$SILKY9_BIN" ]
then
  fail "could not find binary file of silky 9, $SILKY9_BIN"
fi

mpirun $mpirunFlags -n $numOfProcs $SILKY9_BIN $programFileName

#!/bin/sh

# constants

DEBUG=false
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
    err "DEBUG:" "$@"
  fi
}


# main routine

numOfProcs=1

while getopts dn: option
do
  case $option in
  d)
    DEBUG=true
    ;;
  n)
    numOfProcs=$OPTARG
    if ! [ "$numOfProcs" -gt 0 ]
    then
      fail "$USAGE"
    fi
    ;;
  \?)
    fail "$USAGE"
    ;;
  esac
done
shift $(expr $OPTIND - 1)

debug "numOfProcs = $numOfProcs"

if [ $# -eq 1 ]
then
  programFileName=$1
  mpirun -q -n $numOfProcs main $programFileName
else
  fail "$USAGE"
fi

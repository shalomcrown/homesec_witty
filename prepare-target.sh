#! /bin/bash

ruser=roger@dox

executable=$1
executable="${executable%\"}" # quoted with \"
executable="${executable#\"}"


remote=${executable}.remote

echo "Start remote gdb-server"
ssh ${ruser} gdbserver jeloin:2345 "${remote##/*/}" #  will output result



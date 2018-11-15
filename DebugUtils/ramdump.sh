#!/bin/sh
./main $1 | grep "Ram Dump" -A 17

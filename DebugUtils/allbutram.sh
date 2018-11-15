#!/bin/sh
./main $1 | grep "NEXT" -A 8

#!/bin/bash

a="$(dmesg | grep ttyUSB | tail -1 | grep ttyUSB | sed 's/\s/&/')";
if [[ "$a" = *attached* ]]
then
  echo "${a##* }";
fi
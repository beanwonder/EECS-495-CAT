#!/bin/bash

if ! test -f compiler_output ; then
  echo "Test failed" ; 
  echo "File \"compiler_output\" is missing" ;
  exit 1;
fi
if ! test -f output/oracle_output ; then
  echo "Test failed" ; 
  echo "File \"output/oracle_output\" is missing" ;
  exit 1;
fi

diffOut=`diff compiler_output output/oracle_output` ;
if test "$diffOut" == "" ; then 
  echo "Test passed!" ; 
else 
  echo "Test failed" ; 
  echo "Output differences can be found in \"./diff_output\"" ;
  echo "$diffOut" > diff_output ;
fi

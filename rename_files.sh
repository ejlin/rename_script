#!/bin/bash
echo Name a file to write to of all files with spaces in them

read varname

echo Writing to \'$varname\':
find -name "* *" -type f > $varname
./file_script $varname



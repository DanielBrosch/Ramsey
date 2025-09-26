#!/bin/sh


echo "Entry   UB FA   rounded_Value"

ls -d */ | while read entry ; do

cd "$entry"

value=`cat rounding_log.txt | grep  '# ='  |  sed 's/.*= //'`

table_entry=`echo "import math;  print(math.ceil(1/$value))" | python3`

cd ..
echo "$entry $table_entry  $value"

done 

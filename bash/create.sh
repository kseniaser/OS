#!/bin/bash
i=0
while (($i <=  25 ))
do
#creating new files
touch a$i
touch b$i
touch c$i
i=$(( i+1 ))
done

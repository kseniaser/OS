#!/bin/bash
echo "type word to found"
read a
echo | grep -i $a target.txt > result.txt


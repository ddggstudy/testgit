#!/bin/bash

for i in `seq 1 100`
do
echo $i
done

echo "aaaaaaaa"

for((j=1;j<=66;++j))
do
echo $j
done

echo "bbbbbbbbbb"

for x in {1..100}
do
echo $x
done

#bash for.sh can work, but sh for.sh err TODO:study sh in linux/ubuntu

#!/bin/bash
for i in {1..5}
do
  for text in 100000 1000000 10000000 100000000
  do
    for nthreads in 1 2 4
    do
      echo "Calculando caso de teste ${i}: Texto: message${dim}.txt, Threads: ${nthreads}";
      ./a.out ${nthreads} message${text}.txt > out/out_${i}_${dim}_${nthreads}.txt;
    done
  done
done

#do
#  cd ./out
#  python3 result.py
#done
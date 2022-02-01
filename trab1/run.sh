#!/bin/bash
for i in {1..10}
do
  for text in {1..100}
  do
    for nthreads in 1 2 4
    do
      echo "Calculando caso de teste ${i}: Texto: message${dim}.txt, Threads: ${nthreads}";
      ./a.out message${text} ${nthreads} > out/out_${i}_${dim}_${nthreads}.txt;
    done
  done
done

#do
#  cd ./out
#  python3 result.py
#done
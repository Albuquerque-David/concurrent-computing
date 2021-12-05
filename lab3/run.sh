#!/bin/bash
s_lim=$RANDOM
i_lim=$RANDOM

for i in {1..5}
do
  for dim in 100000 1000000 10000000 100000000 1000000000
  do
    for nthreads in 1 2 4
    do
      echo "Calculando caso de teste ${i}: Dimensao do array:${dim}, Threads: ${nthreads}, Limite Inferior: ${i_lim}, Limite Superior: ${s_lim}";
      ./a.out ${nthreads} ${dim} ${i_lim} ${s_lim} > out/out_${i}_${dim}_${nthreads}_${i_lim}_${s_lim}.txt;
    done
  done
done

#do
#  cd ./out
#  python3 result.py
#done
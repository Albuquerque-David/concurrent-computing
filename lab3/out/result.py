import matplotlib.pyplot as plt

minSequentially = []
minConcurrently = []
minFreeMemory = []
for dim in (100000,1000000,10000000,100000000,1000000000):
  for nthreads in (1,2,4):
    minSequentiallyValue = 1000
    minConcurrentlyValue = 1000
    minFreeMemoryValue = 1000
    for i in range(1,6):
      fileName = f'out_{i}_{dim}_{nthreads}_13315_15356.txt'
      with open(fileName) as f:
        lines = f.readlines()
        lines = [line.split() for line in lines]
        if(lines[2][-1] != 'correto'):
          print(f'Erro no calculo de ocorrencias {fileName}\n')
          exit()
        minSequentiallyValue = min(minSequentiallyValue, float(lines[0][-2]))
        minConcurrentlyValue = min(minConcurrentlyValue, float(lines[1][-2]))
        minFreeMemoryValue = min(minFreeMemoryValue, float(lines[3][-2]))
    minSequentially.append(minSequentiallyValue)
    minConcurrently.append(minConcurrentlyValue)
    minFreeMemory.append(minFreeMemoryValue)

minSequentially.insert(0, "Sequencial")

minConcurrently.insert(0, "Concorrente: 1 Thread")
minConcurrently.insert(6, "Concorrente: 2 Thread")
minConcurrently.insert(12, "Concorrente: 4 Thread")

minFreeMemory.insert(0, "Liberar memória")

print(minFreeMemory)
print(minSequentially)
print(minConcurrently)

outFinal = []

out = minSequentially[:1]
out.append(minSequentially[1])
out.append(minSequentially[4])
out.append(minSequentially[7])
out.append(minSequentially[10])
out.append(minSequentially[13])
outFinal.append(out)

out = minConcurrently[0:1]
out.append(minConcurrently[1])
out.append(minConcurrently[4])
out.append(minConcurrently[8])
out.append(minConcurrently[11])
out.append(minConcurrently[15])
outFinal.append(out)

out = ['Desempenho: 1 Thread']
out.append(minSequentially[1] / minConcurrently[1])
out.append(minSequentially[4] / minConcurrently[4])
out.append(minSequentially[7] / minConcurrently[8])
out.append(minSequentially[10] / minConcurrently[11])
out.append(minSequentially[13] / minConcurrently[15])
outFinal.append(out)

out = minConcurrently[6:7]
out.append(minConcurrently[2])
out.append(minConcurrently[5])
out.append(minConcurrently[9])
out.append(minConcurrently[13])
out.append(minConcurrently[16])
outFinal.append(out)

out = ['Desempenho: 2 Threads']
out.append(minSequentially[1] / minConcurrently[2])
out.append(minSequentially[4] / minConcurrently[5])
out.append(minSequentially[7] / minConcurrently[9])
out.append(minSequentially[10] / minConcurrently[13])
out.append(minSequentially[13] / minConcurrently[16])
outFinal.append(out)

out = minConcurrently[12:13]
out.append(minConcurrently[3])
out.append(minConcurrently[7])
out.append(minConcurrently[10])
out.append(minConcurrently[14])
out.append(minConcurrently[17])
outFinal.append(out)

out = ['Desempenho: 4 Threads']
out.append(minSequentially[1] / minConcurrently[3])
out.append(minSequentially[4] / minConcurrently[7])
out.append(minSequentially[7] / minConcurrently[10])
out.append(minSequentially[10] / minConcurrently[14])
out.append(minSequentially[13] / minConcurrently[17])
outFinal.append(out)

#out = minFreeMemory[:1]
#out.append(minFreeMemory[1])
#out.append(minFreeMemory[4])
#out.append(minFreeMemory[7])
#out.append(minFreeMemory[10])
#out.append(minFreeMemory[13])
#outFinal.append(out)

table = plt.table(cellText=outFinal, colLabels=['Tipo','Tamanho: 10^5', 'Tamanho: 10^6', 'Tamanho: 10^7','Tamanho: 10^8','Tamanho: 10^9', 'Desempenho'], loc='center', 
                  cellLoc='center', colColours=['#FFFFFF','#FFFFFF', '#FFFFFF', '#FFFFFF', '#FFFFFF','#FFFFFF', '#FFFFFF',])
table.auto_set_font_size(False)
h = table.get_celld()[(0,0)].get_height()
w = table.get_celld()[(0,0)].get_width()

plt.title("Mínimo encontrado em todos os casos.\n Tabela gerada através do result.py.")
plt.axis('off')
plt.show()
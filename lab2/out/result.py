import matplotlib.pyplot as plt

minSequentially = []
minConcurrently = []
minFreeMemory = []
for dim in (500,1000,2000):
  for nthreads in (1,2,4):
    minSequentiallyValue = 1000
    minConcurrentlyValue = 1000
    minFreeMemoryValue = 1000
    for i in range(1,6):
      fileName = f'out_{i}_{dim}_{nthreads}.txt'
      with open(fileName) as f:
        lines = f.readlines()
        lines = [line.split() for line in lines]
        if(lines[2][-1] != 'correto'):
          print(f'Erro no calculo da matriz {fileName}\n')
          exit()
        minSequentiallyValue = min(minSequentiallyValue, float(lines[0][-2]))
        minConcurrentlyValue = min(minConcurrentlyValue, float(lines[1][-2]))
        minFreeMemoryValue = min(minFreeMemoryValue, float(lines[3][-2]))
    minSequentially.append(minSequentiallyValue)
    minConcurrently.append(minConcurrentlyValue)
    minFreeMemory.append(minFreeMemoryValue)
print(minFreeMemory)
print(minSequentially)
print(minConcurrently)

minSequentially.insert(0, "Sequencial")
minSequentially.insert(4, "Sequencial")
minSequentially.insert(8, "Sequencial")

minConcurrently.insert(0, "Concorrente: 1 Thread")
minConcurrently.insert(4, "Concorrente: 2 Thread")
minConcurrently.insert(8, "Concorrente: 4 Thread")

minFreeMemory.insert(0, "Liberar memória")
minFreeMemory.insert(4, "Liberar memória")
minFreeMemory.insert(8, "Liberar memória")

outFinal = []

out1 = minSequentially[:4]
out2 = minSequentially[4:8]
out3 = minSequentially[8:12]

outFinal.append(out1)
outFinal.append(out2)
outFinal.append(out3)

out1 = minConcurrently[:4]
out2 = minConcurrently[4:8]
out3 = minConcurrently[8:12]

outFinal.append(out1)
outFinal.append(out2)
outFinal.append(out3)

out1 = minFreeMemory[:4]
out2 = minFreeMemory[4:8]
out3 = minFreeMemory[8:12]

outFinal.append(out1)
outFinal.append(out2)
outFinal.append(out3)

outFinal.append(out1)
outFinal.append(out2)
outFinal.append(out3)

table = plt.table(cellText=outFinal, colLabels=['Tipo','Tamanho: 500', 'Tamanho: 1000', 'Tamanho: 2000'], loc='center', 
                  cellLoc='center', colColours=['#FFFFFF','#FFFFFF', '#F3CC32', '#2769BD'])
table.auto_set_font_size(False)
h = table.get_celld()[(0,0)].get_height()
w = table.get_celld()[(0,0)].get_width()

plt.title("Caso 1")
plt.axis('off')
plt.show()
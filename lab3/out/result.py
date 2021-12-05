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
# print(minFreeMemory)
# print(minSequentially)
# print(minConcurrently)

minSequentially.insert(0, "Sequencial")

minConcurrently.insert(0, "Concorrente: 1 Thread")
minConcurrently.insert(4, "Concorrente: 2 Thread")
minConcurrently.insert(8, "Concorrente: 4 Thread")

minFreeMemory.insert(0, "Liberar memória")

outFinal = []

out = minSequentially[:1]
out.append(minSequentially[1])
out.append(minSequentially[4])
out.append(minSequentially[7])
outFinal.append(out)

out = minConcurrently[0:1]
out.append(minConcurrently[1])
out.append(minConcurrently[5])
out.append(minConcurrently[9])
outFinal.append(out)

out = ['Desempenho: 1 Thread']
out.append(minSequentially[1] / minConcurrently[1])
out.append(minSequentially[4] / minConcurrently[5])
out.append(minSequentially[7] / minConcurrently[9])
outFinal.append(out)

out = minConcurrently[4:5]
out.append(minConcurrently[2])
out.append(minConcurrently[6])
out.append(minConcurrently[10])
outFinal.append(out)

out = ['Desempenho: 2 Threads']
out.append(minSequentially[1] / minConcurrently[2])
out.append(minSequentially[4] / minConcurrently[6])
out.append(minSequentially[7] / minConcurrently[10])
outFinal.append(out)

out = minConcurrently[8:9]
out.append(minConcurrently[3])
out.append(minConcurrently[7])
out.append(minConcurrently[11])
outFinal.append(out)

out = ['Desempenho: 4 Threads']
out.append(minSequentially[1] / minConcurrently[3])
out.append(minSequentially[4] / minConcurrently[7])
out.append(minSequentially[7] / minConcurrently[11])
outFinal.append(out)

out = minFreeMemory[:1]
out.append(minFreeMemory[1])
out.append(minFreeMemory[4])
out.append(minFreeMemory[7])
outFinal.append(out)

table = plt.table(cellText=outFinal, colLabels=['Tipo','Tamanho: 500', 'Tamanho: 1000', 'Tamanho: 2000', 'Desempenho'], loc='center', 
                  cellLoc='center', colColours=['#FFFFFF','#FFFFFF', '#FFFFFF', '#FFFFFF'])
table.auto_set_font_size(False)
h = table.get_celld()[(0,0)].get_height()
w = table.get_celld()[(0,0)].get_width()

plt.title("Mínimo encontrado em todos os casos.\n Tabela gerada através do result.py.")
plt.axis('off')
plt.show()
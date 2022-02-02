import matplotlib.pyplot as plt

minSequentiallyCryptography = []
minSequentiallyDecryptography = []
minConcurrentlyCryptography = []
minConcurrentlyDecryptography = []
for dim in (100000,1000000,10000000,100000000):
  for nthreads in (1,2,3):
    minSequentiallyCryptographyValue = 1000
    minSequentiallyDecryptographyValue = 1000
    minConcurrentlyCryptographyValue = 1000
    minConcurrentlyDecryptographyValue = 1000
    for i in range(1,6):
      fileName = f'out_{i}_{dim}_{nthreads}.txt'
      with open(fileName) as f:
        lines = f.readlines()
        lines = [line.split() for line in lines]
        minSequentiallyCryptographyValue = min(minSequentiallyCryptographyValue, float(lines[2][-2]))
        minSequentiallyDecryptographyValue = min(minSequentiallyDecryptographyValue, float(lines[3][-2]))
        minConcurrentlyCryptographyValue = min(minConcurrentlyCryptographyValue, float(lines[4][-2]))
        minConcurrentlyDecryptographyValue = min(minConcurrentlyDecryptographyValue, float(lines[5][-2]))
    minSequentiallyCryptography.append(minSequentiallyCryptographyValue)
    minSequentiallyDecryptography.append(minSequentiallyDecryptographyValue)
    minConcurrentlyCryptography.append(minConcurrentlyCryptographyValue)
    minConcurrentlyDecryptography.append(minConcurrentlyDecryptographyValue)

print(minSequentiallyCryptography)
print(minSequentiallyDecryptography)
print(minConcurrentlyCryptography)
print(minConcurrentlyDecryptography)
print(minSequentiallyCryptographyValue)
print(minSequentiallyDecryptographyValue)
print(minConcurrentlyCryptographyValue)
print(minConcurrentlyDecryptographyValue)
# minySequentially.insert(0, "Sequencial")

# minConcurrently.insert(0, "Concorrente: 1 Thread")
# minConcurrently.insert(6, "Concorrente: 2 Thread")
# minConcurrently.insert(12, "Concorrente: 4 Thread")

# minFreeMemory.insert(0, "Liberar memória")

# print(minFreeMemory)
# print(minSequentially)
# print(minConcurrently)

# outFinal = []

# out = minSequentially[:1]
# out.append(minSequentially[1])
# out.append(minSequentially[4])
# out.append(minSequentially[7])
# out.append(minSequentially[10])
# out.append(minSequentially[13])
# outFinal.append(out)

# out = minConcurrently[0:1]
# out.append(minConcurrently[1])
# out.append(minConcurrently[4])
# out.append(minConcurrently[8])
# out.append(minConcurrently[11])
# out.append(minConcurrently[15])
# outFinal.append(out)

# out = ['Desempenho: 1 Thread']
# out.append(minSequentially[1] / minConcurrently[1])
# out.append(minSequentially[4] / minConcurrently[4])
# out.append(minSequentially[7] / minConcurrently[8])
# out.append(minSequentially[10] / minConcurrently[11])
# out.append(minSequentially[13] / minConcurrently[15])
# outFinal.append(out)

# out = minConcurrently[6:7]
# out.append(minConcurrently[2])
# out.append(minConcurrently[5])
# out.append(minConcurrently[9])
# out.append(minConcurrently[13])
# out.append(minConcurrently[16])
# outFinal.append(out)

# out = ['Desempenho: 2 Threads']
# out.append(minSequentially[1] / minConcurrently[2])
# out.append(minSequentially[4] / minConcurrently[5])
# out.append(minSequentially[7] / minConcurrently[9])
# out.append(minSequentially[10] / minConcurrently[13])
# out.append(minSequentially[13] / minConcurrently[16])
# outFinal.append(out)

# out = minConcurrently[12:13]
# out.append(minConcurrently[3])
# out.append(minConcurrently[7])
# out.append(minConcurrently[10])
# out.append(minConcurrently[14])
# out.append(minConcurrently[17])
# outFinal.append(out)

# out = ['Desempenho: 4 Threads']
# out.append(minSequentially[1] / minConcurrently[3])
# out.append(minSequentially[4] / minConcurrently[7])
# out.append(minSequentially[7] / minConcurrently[10])
# out.append(minSequentially[10] / minConcurrently[14])
# out.append(minSequentially[13] / minConcurrently[17])
# outFinal.append(out)

# #out = minFreeMemory[:1]
# #out.append(minFreeMemory[1])
# #out.append(minFreeMemory[4])
# #out.append(minFreeMemory[7])
# #out.append(minFreeMemory[10])
# #out.append(minFreeMemory[13])
# #outFinal.append(out)

# table = plt.table(cellText=outFinal, colLabels=['Tipo','Tamanho: 10^5', 'Tamanho: 10^6', 'Tamanho: 10^7','Tamanho: 10^8','Tamanho: 10^9', 'Desempenho'], loc='center', 
#                   cellLoc='center', colColours=['#FFFFFF','#FFFFFF', '#FFFFFF', '#FFFFFF', '#FFFFFF','#FFFFFF', '#FFFFFF',])
# table.auto_set_font_size(False)
# h = table.get_celld()[(0,0)].get_height()
# w = table.get_celld()[(0,0)].get_width()

# plt.title("Mínimo encontrado em todos os casos.\n Tabela gerada através do result.py.")
# plt.axis('off')
# plt.show()

# Criptografa/Descriptografa Sequencial

xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minSequentiallyCryptography[0])
yAxis.append(minSequentiallyCryptography[3])
yAxis.append(minSequentiallyCryptography[6])
yAxis.append(minSequentiallyCryptography[9])
plt.plot(xAxis,yAxis)
xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minSequentiallyDecryptography[0])
yAxis.append(minSequentiallyDecryptography[3])
yAxis.append(minSequentiallyDecryptography[6])
yAxis.append(minSequentiallyDecryptography[9])
plt.plot(xAxis,yAxis)
plt.title('Criptografia/Descriptografia Sequencial - Tempos Mínimos')
plt.xlabel('Quantidade de Caracteres')
plt.ylabel('Tempo em ms')
plt.legend(['Criptografa', 'Descriptografa'])
plt.show()

# Criptografa/Descriptografa Concorrente 1 Thread

xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minConcurrentlyCryptography[0])
yAxis.append(minConcurrentlyCryptography[3])
yAxis.append(minConcurrentlyCryptography[6])
yAxis.append(minConcurrentlyCryptography[9])
plt.plot(xAxis,yAxis)
xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minConcurrentlyDecryptography[0])
yAxis.append(minConcurrentlyDecryptography[3])
yAxis.append(minConcurrentlyDecryptography[6])
yAxis.append(minConcurrentlyDecryptography[9])
plt.plot(xAxis,yAxis)
plt.title('Criptografia/Descriptografia Concorrente - 1 Thread - Tempos Mínimos')
plt.xlabel('Quantidade de Caracteres')
plt.ylabel('Tempo em ms')
plt.legend(['Criptografa', 'Descriptografa'])
plt.show()

# Criptografa/Descriptografa Concorrente 2 Thread

xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minConcurrentlyCryptography[1])
yAxis.append(minConcurrentlyCryptography[4])
yAxis.append(minConcurrentlyCryptography[7])
yAxis.append(minConcurrentlyCryptography[10])
plt.plot(xAxis,yAxis)
xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minConcurrentlyDecryptography[1])
yAxis.append(minConcurrentlyDecryptography[4])
yAxis.append(minConcurrentlyDecryptography[7])
yAxis.append(minConcurrentlyDecryptography[10])
plt.plot(xAxis,yAxis)
plt.title('Criptografia/Descriptografia Concorrente - 2 Thread - Tempos Mínimos')
plt.xlabel('Quantidade de Caracteres')
plt.ylabel('Tempo em ms')
plt.legend(['Criptografa', 'Descriptografa'])
plt.show()

# Criptografa/Descriptografa Concorrente 4 Thread

xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minConcurrentlyCryptography[2])
yAxis.append(minConcurrentlyCryptography[5])
yAxis.append(minConcurrentlyCryptography[8])
yAxis.append(minConcurrentlyCryptography[11])
plt.plot(xAxis,yAxis)
xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minConcurrentlyDecryptography[2])
yAxis.append(minConcurrentlyDecryptography[5])
yAxis.append(minConcurrentlyDecryptography[8])
yAxis.append(minConcurrentlyDecryptography[11])
plt.plot(xAxis,yAxis)
plt.title('Criptografia/Descriptografia Concorrente - 3 Thread - Tempos Mínimos')
plt.xlabel('Quantidade de Caracteres')
plt.ylabel('Tempo em ms')
plt.legend(['Criptografa', 'Descriptografa'])
plt.show()

# Desempenho 1 Thread

xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []

yAxis.append(minSequentiallyCryptography[0] / minConcurrentlyCryptography[0])
yAxis.append(minSequentiallyCryptography[3] / minConcurrentlyCryptography[3])
yAxis.append(minSequentiallyCryptography[6] / minConcurrentlyCryptography[6])
yAxis.append(minSequentiallyCryptography[9] / minConcurrentlyCryptography[9])
plt.plot(xAxis,yAxis)
xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minSequentiallyCryptography[0] / minConcurrentlyDecryptography[0])
yAxis.append(minSequentiallyCryptography[3] / minConcurrentlyDecryptography[3])
yAxis.append(minSequentiallyCryptography[6] / minConcurrentlyDecryptography[6])
yAxis.append(minSequentiallyCryptography[9] / minConcurrentlyDecryptography[9])
plt.plot(xAxis,yAxis)
plt.title('Desempenho Criptografia/Descriptografia Concorrente - 1 Thread')
plt.xlabel('Quantidade de Caracteres')
plt.ylabel('Tempo em ms')
plt.legend(['Criptografa', 'Descriptografa'])
plt.show()

# Desempenho Concorrente 2 Thread

xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minSequentiallyCryptography[0] / minConcurrentlyCryptography[1])
yAxis.append(minSequentiallyCryptography[3] / minConcurrentlyCryptography[4])
yAxis.append(minSequentiallyCryptography[6] / minConcurrentlyCryptography[7])
yAxis.append(minSequentiallyCryptography[9] / minConcurrentlyCryptography[10])
plt.plot(xAxis,yAxis)
xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minSequentiallyCryptography[0] / minConcurrentlyDecryptography[1])
yAxis.append(minSequentiallyCryptography[3] / minConcurrentlyDecryptography[4])
yAxis.append(minSequentiallyCryptography[6] / minConcurrentlyDecryptography[7])
yAxis.append(minSequentiallyCryptography[9] / minConcurrentlyDecryptography[10])
plt.plot(xAxis,yAxis)
plt.title('Desempenho Criptografia/Descriptografia Concorrente - 2 Thread')
plt.xlabel('Quantidade de Caracteres')
plt.ylabel('Tempo em ms')
plt.legend(['Criptografa', 'Descriptografa'])
plt.show()

# Desempenho Concorrente 4 Thread

xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minSequentiallyCryptography[0] / minConcurrentlyCryptography[2])
yAxis.append(minSequentiallyCryptography[3] / minConcurrentlyCryptography[5])
yAxis.append(minSequentiallyCryptography[6] / minConcurrentlyCryptography[8])
yAxis.append(minSequentiallyCryptography[9] / minConcurrentlyCryptography[11])
plt.plot(xAxis,yAxis)
xAxis = ['10^5','10^6','10^7','10^8']
yAxis = []
yAxis.append(minSequentiallyCryptography[0] / minConcurrentlyDecryptography[2])
yAxis.append(minSequentiallyCryptography[3] / minConcurrentlyDecryptography[5])
yAxis.append(minSequentiallyCryptography[6] / minConcurrentlyDecryptography[8])
yAxis.append(minSequentiallyCryptography[9] / minConcurrentlyDecryptography[11])
plt.plot(xAxis,yAxis)
plt.title('Desempenho Criptografia/Descriptografia Concorrente - 3 Thread')
plt.xlabel('Quantidade de Caracteres')
plt.ylabel('Ganho de Desempenho em %')
plt.legend(['Criptografa', 'Descriptografa'])
plt.show()
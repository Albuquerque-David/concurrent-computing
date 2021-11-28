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
        print("teste")
        minSequentiallyValue = min(minSequentiallyValue, float(lines[0][-2]))
        minConcurrentlyValue = min(minConcurrentlyValue, float(lines[1][-2]))
        minFreeMemoryValue = min(minFreeMemoryValue, float(lines[3][-2]))
    minSequentially.append(minSequentiallyValue)
    minConcurrently.append(minConcurrentlyValue)
    minFreeMemory.append(minFreeMemoryValue)
print(minFreeMemory)
print(minSequentially)
print(minConcurrently)

data = [[1,2,3],[1,2,3],[1,2,3]]

table = plt.table(cellText=data, colLabels=['A', 'B', 'C'], loc='center', 
                  cellLoc='center', colColours=['#FFFFFF', '#F3CC32', '#2769BD'])
table.auto_set_font_size(False)
h = table.get_celld()[(0,0)].get_height()
w = table.get_celld()[(0,0)].get_width()

plt.axis('off')
plt.show()
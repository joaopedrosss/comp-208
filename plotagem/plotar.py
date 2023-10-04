import matplotlib.pyplot as plt
from math import log2

lista_x = []
lista_y = []
bst_x = []
bst_y = []

log_x = []
log_y = []

for x in range(1,50000):
    log_x.append(x)
for i in log_x:
    i = log2(i)
    log_y.append(i)



with open("dados.txt","r") as arquivo:
    for line in arquivo:
        line = line.split()
        #print("({},{}),({},{})".format(line[0],line[1],line[0],line[2]))

        lista_x.append(int(line[0]))
        lista_y.append(int(line[1]))

        bst_x.append(int(line[0]))
        bst_y.append(int(line[2]))

plt.title("Busca - Lista Encadeada x Árvore de Busca Binária")
plt.xlabel('Número buscado')
plt.ylabel('Número de Comparações')

plt.grid(True, 'major', 'both')

plt.plot(lista_x,lista_y,'.',color='red',label='Lista Encadeada')
plt.plot(bst_x,bst_y,'.',color='blue',label='Árvore de Busca Binária')
plt.plot(log_x,log_y,color='purple',label='O(log2(n))')


plt.legend()

plt.show()


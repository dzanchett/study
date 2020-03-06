import numpy as np
import matplotlib.pyplot as plt
import math

indice = -1

while indice <= 0 or indice >= 6:
	print("Insira o indice da equação de 1 a 5:")
	indice = int(input())

	if indice <= 0 or indice >= 6:
		print("Indice Inválido!")
		print()

# Define o número de passos no calculo
passos = -1
while passos < 0:
	print("Insira o número de passos no calculo")
	passos = int(input())

	if passos < 0:
		print("O número de passos deve ser maior do que 0!")
		print()

print()
print()
print()

# Define a função que será aproximada pela série exponencial de Fourier
def fa(t):
	tt = t % math.pi
	return math.e**(-tt/2)
def fb(t):
	if t%4 > 2:
		return 1
	else:
		return -1
def fc(t):
	tt = t%4
	if tt > 2:
		return 4-tt
	else:
		return tt
def fd(t):
	return abs(5*math.sin(t + 25))
def fe(t):
	tt = t%10

	if tt < 1:
		return 0
	elif tt < 2:
		return 1
	elif tt < 3:
		return 3
	elif tt < 4:
		return 4
	elif tt < 5:
		return 5
	elif tt < 6:
		return 4
	elif tt < 7:
		return 3
	elif tt < 8:
		return 2
	elif tt < 9:
		return 1
	elif tt < 10:
		return 0

# Escolhe a equação de acordo com o índice
f1 = None

if indice == 1:
	f1 = fa
elif indice == 2:
	f1 = fb
elif indice == 3:
	f1 = fc
elif indice == 4:
	f1 = fd
elif indice == 5:
	f1 = fe


# Define alguns paramêtros sobre a função que será aproximada pela série exponecial de Fourier
T0 = None

if indice == 1:
	T0 = math.pi
elif indice == 2:
	T0 = 4
elif indice == 3:
	T0 = 4
elif indice == 4:
	T0 = 2 * math.pi
elif indice == 5:
	T0 = 10

omega0 = (2*math.pi)/T0


# Plota o gráfico da função que será aproximada pela série exponencial de Fourier na cor verde
x = np.linspace(-3*T0, 3*T0, passos)
y = np.vectorize(f1)(x)

plt.plot(x, y, color='g')

# Cálculo Dn atravéz de integração numérica
Dn = []

def f2(n, omega, t):
	return math.e**(complex(0, -n*omega*t))

def f3(n, omega, t):
	return math.e**(complex(0, n*omega*t))

xx = np.linspace(0, T0, passos)
yy = np.vectorize(f1)(xx)

xf = []
yf = []

for a in range(-10, 10):
	dd = 0
	for i in range(passos-1):
		dd += (((yy[i]*f2(a, omega0, xx[i]))+(yy[i+1]*f2(a, omega0, xx[i+1])))*(xx[i+1]-xx[i]))/2

	dd /= T0

	Dn.append(dd)

# Calculo da função x(t) aproximada pela série exponencial de Fourier
for xt in x:
	temp = 0

	for a in range(-10, 10):
		temp += (Dn[a+10] * f3(a, omega0, xt))

	xf.append(xt)
	yf.append(temp)

# Plota o gráfico da função aproximada pela série exponencial de Fourier na cor azul
plt.plot(xf, np.real(yf), color='b', linestyle='dashed')

potencia = 0

# Calculo da potência utilizando o teorema de Parseval
for i in Dn:
	potencia += abs(i)**2

# Imprime a Potência
print("Px = " + str(potencia))

# Exibe o Gráfico com as funções
plt.show()
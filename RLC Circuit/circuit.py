import math
import matplotlib.pyplot as plt
import numpy as np

A = 0
B = 0

p1 = 0
p2 = 0

alpha = 0
omega0 = 0
omegaD = 0

def f1(t):
	return (A * (math.e**(p1*t))) + (B * (math.e**(p2*t)))

def df1(t):
	return (p1 * A * (math.e**(p1*t))) + (p2 * B * (math.e**(p2*t)))

def f2(t):
	return (A * t * (math.e**(-1 * alpha * t))) + (B * (math.e**(-1*alpha*t)))

def df2(t):
	return (A * (math.e**(-1*alpha*t))) - (A * alpha * t * (math.e**(-1*alpha*t))) - (B * alpha * (math.e**(-1*alpha*t)))

def f3(t):
	return (math.e**(-1*alpha*t)) * ((A*math.cos(omegaD * t)) + (B*math.sin(omegaD * t)))

def df3(t):
	return (-1 * alpha * (math.e**(-1*alpha*t)) * ((A*math.cos(omegaD*t)) + (B*math.sin(omegaD*t)))) + ((math.e**(-1*alpha*t)) * ((B*omegaD*math.cos(omegaD*t)) - (A*omegaD*math.sin(omegaD*t))))

print("Plot Circuito RLC Paralelo")
print()

print("Insira o valor da resistencia (R) em Ohms:")
R = float(input())

print("Insira o valor da indutancia (L) em Henry:")
L = float(input())

print("Insira o valor da capacitancia (C) em Farad:")
C = float(input())

print("Insira o valor da corrente da fonte em Amperes:")
If = float(input())

print("Insira a carga inicial do capacitor em Volts:")
Vc = float(input())

print("Insira a corrente inicial do indutor em Amperes:")
Il = float(input())

print()

i = None
di = None

alpha = 1 / (2 * R * C)
omega0 = 1 / math.sqrt(L * C)

print(alpha, omega0)

Ic = -Il - (Vc/R) + If
Vlinha = Ic/C

if alpha >= omega0-0.05 and alpha<=omega0+0.05:
	print("Sistema Críticamente Amortecido")

	B = Vc
	A = Vlinha + (B*alpha)

	i = f2
	di = df2
elif alpha > omega0:
	print("Sistema Superamortecido")

	p1 = (-1*alpha) + math.sqrt((alpha**2) - (omega0**2))
	p2 = (-1*alpha) - math.sqrt((alpha**2) - (omega0**2))

	B = (Vlinha - (p1 * Vc))/(p2 - p1)
	A = Vc - B

	i = f1
	di = df1
elif alpha < omega0:
	print("Sistema Subamortecido")

	omegaD = math.sqrt(((omega0**2) - (alpha**2)))

	A = Vc
	B = (Vlinha - (alpha*A))/omegaD

	i = f3
	di = df3

print(omegaD)
print(Ic, Vlinha)
print(p1, p2)
print(A, B, alpha)

x = np.linspace(0, 0.1, 1000)
v = [i(k) for k in x]
vLinha = [di(k) for k in x]

Ir = [k/R for k in v]
Ic = [C * k for k in vLinha]

Il = []

for k in range(0, len(v)):
	Il.append(If - Ir[k] - Ic[k])


plt.subplot(2, 1, 1)
plt.plot(x, v)
plt.ylabel("v(V)")
plt.xlabel("T(s)")

plt.subplot(2, 1, 2)
plt.plot(x, Ir, color="blue")
plt.plot(x, Ic, color="green")
plt.plot(x, Il, color="red")
plt.ylabel("i(A)")
plt.xlabel("T(s)")

plt.show()

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
# Задание 2 Решить дифференциальное уравнение d^2y/dt^2 = d^2y/dx^2  с начальным условием
# y(x,0)=(1-x)cos(pi*x/2)
# dy(x,0)/dt = 2x+1
# y(0,t) = 2t+1  граничные условие слева
# y(1,t) = 0     граничные условие справа
# x принадлежит отрезку [0,1]
# h = 0,01  0<=t<=4   delta t =0,01
# для решения этой задачи и построения графика пользовался сайтом  devpractice.ru/matplotlib
# ----------------------------------------------------------------------------

# Глобальные параметры которые мы знаем
h = 0.01  # размер сетки
x0 = 0    # начала отезка
xn = 1    # конец отрезка
Array_x = np.arange(x0, xn, h)  # создадим массив для x в следующем диапазоне. Начало 0 конец 1 и шаг 0.01
deltat = 0.01  # шаг для t
t0 = 0  # левое ограничение для t
tn = 4  # правое ограничение для t
Array_t = np.arange(t0, tn, deltat)  # создадим массив для t в следующем диапазоне. Начало 0 конец 4 и шаг 0.01
Z = np.empty(shape=(tn*100, (t0+1)*100))    # размер пространства для графика изходя из условия задачи
# где 100 это количество пространственных узлов  а 400 количество временнЫх узлов
# функия для начального условия f(x) = y(x,0)=(1-x)cos(pi*x/2)
def y(x):
    return (1-x) * np.cos((np.pi * x)/2)

# dy(x,0)/dt = 2x+1
def dydt(x):
    return 2*x+1

for i in range(99):
    Z[0][i] = y(Array_x[i]) # момент в нулевой момент времени
    # по формуле находим следующий элемент у Z    y1j = psi1(xj)+psi2(xj)*h  заполняется второй момент слоя
    Z[1][i] = Z[0][i] + h * dydt(Array_x[i])

# здесь в цикле меняем следующие элементы чтобы они удовлетворяли условию y(0,t) = 2t+1 то есть для левой границы
# Правую границу мы уже знаем что будет 0
for j in range(400):
    Z[j][0] = 2 * Array_t[j] + 1

# выводим систему из 98 уравнений так как на границах знаем
def solve(b):
# тут матрица кторая состоит -3 и 1 по методу трёх диагональной прогонке
    A = np.zeros(shape=(98, 98))
    A[0][0] = -3
    A[97][97] = -3
    for i in range(1, 97):
        A[i][i] = -3
        A[i+1][i] = 1
        A[i][i-1] = 1
    uk = np.linalg.inv(A) @ b # матричное умножение матрицы и наших уравнений
    return uk

for k in range(2, 400):
    b = Z[k - 2] - 2 * Z[k - 1]
    uk = solve(b[1:-1])
    Z[k][1:-1] = uk

with open('answer.txt', 'w') as file:
    s = ''
    for i in range(400):
        for j in range(100):
            s += str(Z[i][j]) + ' '
        s += '\n'
    file.write(s)


# ------------------------------------------------------------------------------------------
# для решения этой задачи и построения графика пользовался сайтом  devpractice.ru/matplotlib
# при помощи функции mesgrid делаем сетку из этих массивов для будущего графика
X, Y = np.meshgrid(Array_x, Array_t)
# Дальше создаём переменую для нашего проекта который будет в 3D
project = plt.figure()
ax = project.add_subplot(111, projection='3d')
Axes3D.plot_surface(ax, X=X, Y=Y, Z=Z)
plt.show()


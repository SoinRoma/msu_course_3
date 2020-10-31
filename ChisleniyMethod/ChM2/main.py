import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D



u = np.empty(shape=(400, 100))

tk = np.arange(0, 4, 0.01)
xj = np.arange(0, 1, 0.01)

h = 0.01


def f_x(x):
    return (x + 0.4) * np.sin(np.pi * x)


def du_dt_0(x):
    return (x + 1) ** 2


for j in range(99):
    u[0][j] = f_x(xj[j])
    u[1][j] = u[0][j] + h * du_dt_0(xj[j])


for k in range(400):
    u[k][0] = 0.5 * tk[k]



def solve(b):
    zz = np.zeros(shape=(98, 98))
    zz[0][0] = -3
    zz[97][97] = -3
    for i in range(1, 97):
        zz[i][i] = -3
        zz[i+1][i] = 1
        zz[i][i-1] = 1
    uk = np.linalg.inv(zz) @ b
    return uk


for k in range(2, 400):
    b = u[k-2] - 2 * u[k-1]
    uk = solve(b[1:-1])
    u[k][1:-1] = uk

print("abc")


xgrid, tgrid = np.meshgrid(x, t)
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
Axes3D.plot_surface(ax, X=xgrid, Y=tgrid, Z=u)
plt.show()


# from mpl_toolkits.mplot3d import Axes3D
# import matplotlib.pyplot as plt
# import numpy as np

# # Fixing random state for reproducibility
# np.random.seed(19680801)


# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
# x = np.random.rand(1000) * 10
# y = np.random.rand(1000) * 10000
# hist, xedges, yedges = np.histogram2d(x, y, bins=[6,5], range=[[4, 28], [1000, 6000]])
# print(hist)

# # Construct arrays for the anchor positions of the 16 bars.
# # Note: np.meshgrid gives arrays in (ny, nx) so we use 'F' to flatten xpos,
# # ypos in column-major order. For numpy >= 1.7, we could instead call meshgrid
# # with indexing='ij'.
# xpos, ypos = np.meshgrid(xedges[:-1], yedges[:-1])
# xpos = xpos.flatten('F')
# ypos = ypos.flatten('F')
# zpos = np.zeros_like(xpos)

# # Construct arrays with the dimensions for the 16 bars.
# dx = 100 * np.ones_like(zpos)
# dy = dx.copy()
# dz = hist.flatten()

# ax.bar3d(xpos, ypos, zpos, dx, dy, dz, color='b', zsort='average')

# plt.show()

import random
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

from mpl_toolkits.mplot3d import Axes3D
from scipy.interpolate import interp2d

from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

xa=np.array([4,8,12,16,20,24,4,8,12,16,20,24,4,8,12,16,20,24,4,8,12,16,20,24,4,8,12,16,20,24])
ya=np.array([1000,2000,3000,4000,5000,1000,2000,3000,4000,5000,1000,2000,3000,4000,5000,1000,2000,3000,4000,5000,1000,2000,3000,4000,5000,1000,2000,3000,4000,5000])

za=np.array([50.8994,89.3436,128.105,163.03,197.934,
	59.5806,110.148,161.98,215.901,265.195,
	55.9876,97.9228,134.593,178.471,222.671,
	53.7638,102.987,153.361,204.243,253.531,
	54.896,105.712,151.616,195.332,245.903,
	54.4416,110.362,161.854,210.693,256.67])

resultnum=np.array([45.3344,92.1322,138.035,183.625,227.467,17.3176,33.896,51.3062,67.6294,87.5038,14.0438,30.1592,45.8658,59.8222,78.3522,17.4122,34.5946,49.8206,65.2026,80.1932,15.0436,29.2414,43.9496,57.8478,69.9508,14.178,27.3742,41.5224,54.4216,66.7712])

# for i in range(0,len(za)):
# 	za[i]=za[i]/resultnum[i]

func=interp2d(xa,ya,za,kind='linear')


# za=np.array([[50.8994,89.3436,128.105,163.03,197.934],
# 	[59.5806,110.148,161.98,215.901,265.195],
# 	[55.9876,97.9228,134.593,178.471,222.671],
# 	[53.7638,102.987,153.361,204.243,253.531],
# 	[54.896,105.712,151.616,195.332,245.903],
# 	[54.4416,110.362,161.854,210.693,256.67]])

# bottom = np.zeros_like(za)
# width = 2
# depth =200
# ax.bar3d(xa, ya, bottom, width, depth, za, shade=True)

X = np.arange(4, 24, 0.25)
Y = np.arange(1000, 5000, 10)
Z = func(X,Y)
X, Y = np.meshgrid(X, Y)

# Plot the surface.
surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)

plt.show()

# import numpy as np
# import matplotlib.pyplot as plt
# from mpl_toolkits.mplot3d import Axes3D


# # setup the figure and axes
# fig = plt.figure(figsize=(8, 3))
# ax1 = fig.add_subplot(121, projection='3d')
# ax2 = fig.add_subplot(122, projection='3d')

# # fake data
# _x = np.arange(4)
# _y = np.arange(5)
# _xx, _yy = np.meshgrid(_x, _y)
# x, y = _xx.ravel(), _yy.ravel()

# top = x + y
# bottom = np.zeros_like(top)
# width = depth = 1

# print(top)

# ax1.bar3d(x, y, bottom, width, depth, top, shade=True)
# ax1.set_title('Shaded')

# ax2.bar3d(x, y, bottom, width, depth, top, shade=False)
# ax2.set_title('Not Shaded')

# plt.show()
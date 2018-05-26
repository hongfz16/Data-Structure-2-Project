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

# za=np.array([50.8994,89.3436,128.105,163.03,197.934,
# 	59.5806,110.148,161.98,215.901,265.195,
# 	55.9876,97.9228,134.593,178.471,222.671,
# 	53.7638,102.987,153.361,204.243,253.531,
# 	54.896,105.712,151.616,195.332,245.903,
# 	54.4416,110.362,161.854,210.693,256.67])

# resultnum=np.array([45.3344,92.1322,138.035,183.625,227.467,17.3176,33.896,51.3062,67.6294,87.5038,14.0438,30.1592,45.8658,59.8222,78.3522,17.4122,34.5946,49.8206,65.2026,80.1932,15.0436,29.2414,43.9496,57.8478,69.9508,14.178,27.3742,41.5224,54.4216,66.7712])

za=np.array([50.49,85.2435,114.977,165.198,199.871,
	59.1887,116.041,165.847,224.604,273.634,
	59.8342,110.489,159.038,210.594,259.731,
	57.0751,123.583,167.027,219.494,271.669,
	57.3307,108.582,162.867,195.426,247.984,
	59.5356,116.32,167.781,219.5,260.945])
resultnum=np.array([46.6259,86.6478,129.251,189.161,228.816,15.2381,36.5191,56.0592,72.7929,89.3507,16.4097,32.9635,47.8014,63.7004,81.6049,13.1563,33.0984,47.0866,66.5358,78.0302,17.1853,28.0407,43.9288,61.6594,76.5869,15.5222,31.6872,44.9652,57.604,69.0181])
# for i in range(0,len(za)):
# 	za[i]=za[i]/resultnum[i] * 100

# ---> 3D bar chart
# bottom = np.zeros_like(za)
# width = 2
# depth =400
# ax.bar3d(xa, ya, bottom, width, depth, za, shade=True)
# ax.set_xlabel('Dimensions')
# ax.set_ylabel('Object Numbers')
# ax.set_zlabel('Disk Access Times')
# ---> end

# ---> interp2d
func=interp2d(xa,ya,za,kind='cubic')
X = np.arange(4, 24, 0.25)
Y = np.arange(1000, 5000, 10)
Z = func(X,Y)
X, Y = np.meshgrid(X, Y)
# Plot the surface.
surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)
fig.colorbar(surf, shrink=0.5, aspect=5)
ax.set_xlabel('Dimensions')
ax.set_ylabel('Object Numbers')
ax.set_zlabel('Disk Access Times')
# ---> end

my_x_ticks = np.arange(4, 25, 4)
my_y_ticks = np.arange(1000,5001,1000)
plt.xticks(my_x_ticks)
plt.yticks(my_y_ticks)

plt.show()
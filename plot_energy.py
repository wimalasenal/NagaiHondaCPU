from numpy import *
import matplotlib.pyplot as plt

x,y = loadtxt('energy.dat', unpack=1, usecols=[0,1])
my = min(y)
My = max(y)
buffer = My - my

fig = plt.figure(facecolor='dimgrey')
ax1 = fig.add_subplot(111)
ax1.set_title("Energy in the mesh", fontsize=14, fontweight='bold')    
ax1.set_xlabel('Time', fontsize=12, fontweight='bold')
ax1.set_ylabel('Energy', fontsize=12, fontweight='bold')
ax1.set_axis_bgcolor('gainsboro')
ax1.set_ylim([my-0.1*buffer, My])
ax1.plot(x,y, c='black', label='Energy (Unitless)')
plt.savefig('Images/energy.png')

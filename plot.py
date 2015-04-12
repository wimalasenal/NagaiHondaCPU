import matplotlib as mpl
import seaborn as sns
from numpy import *
import matplotlib.pyplot as plt

###############################################
########## Set up axis and figure #############
###############################################

fig, axarr = plt.subplots(2, 2, facecolor='white')
fig.subplots_adjust(hspace=0.7, wspace=0.5)

################################################
#########   Make the energy plot   #############
################################################

x,y = loadtxt('energy.dat', unpack=1, usecols=[0,1])
my = min(y)
My = max(y)
buffer = My - my

axarr[0, 0].plot(x,y, c='black', label='Energy (Unitless)')
axarr[0, 0].set_title("Energy in the mesh", fontsize=12, fontweight='bold')    
axarr[0, 0].set_xlabel('Time', fontsize=10)
axarr[0, 0].set_ylabel('Energy', fontsize=10)
axarr[0, 0].set_axis_bgcolor('gainsboro')
axarr[0, 0].set_ylim([my-0.1*buffer, My])
axarr[0,0].set_axis_bgcolor('white')

################################################
#########     Make the bargraph    #############
################################################

sides0, num_cells0 = loadtxt('hist_data0.txt', unpack=1, usecols=[0,1])
sides1, num_cells1 = loadtxt('hist_data1.txt', unpack=1, usecols=[0,1])

N = len(sides0)
ind = arange(N)    # the x locations for the groups
width = 0.35       # the width of the bars

rects1 = axarr[0, 1].bar(ind, num_cells0, width, color='orange')
rects2 = axarr[0, 1].bar(ind + width, num_cells1, width, color = 'b' )

axarr[0, 1].set_ylabel('Number of Cells', fontsize=10)
axarr[0, 1].set_xlabel('Sides', fontsize=10)
axarr[0, 1].set_title('Distribution of Cell Shapes',fontsize=12, fontweight='bold')
axarr[0, 1].set_xticks(ind+width)
sides0 = sides0.astype(int)
axarr[0, 1].set_xticklabels(sides0)
axarr[0, 1].legend( (rects1[0], rects2[0]), ('Before', 'After') )
axarr[0, 1].set_xlim(0, N-width)
axarr[0,1].set_axis_bgcolor('white')


################################################
#####     Make the area/perim graphs    ########
################################################

fin = open("parameters.hpp", 'r')
for i in range(3):
    junk = fin.readline()
beta = fin.readline().split()[4]
lam = fin.readline().split()[4]
junk = fin.readline()
area = fin.readline().split()[4]
gamma = fin.readline().split()[4]
fin.close()

# Make area plot
a = open('area.txt').read().splitlines()
data = map(float, a)
axarr[1,0].hist(data, 10)
Title = 'Equilibrium Area: beta = ' + beta + '\nlambda = ' + lam + ' gamma = ' + gamma
axarr[1,0].set_title(Title, fontsize=12, fontweight='bold')
axarr[1,0].set_xlabel('Area', fontsize=10)
axarr[1,0].set_ylabel('Number of Cells', fontsize = 10)
axarr[1,0].set_axis_bgcolor('white')
#axarr[1,0].grid(b=0)

p = open('perim.txt').read().splitlines()
data = map(float, p)
axarr[1,1].hist(data, 10)
Title = 'Equilibrium Perimeter: beta = ' + beta + '\nlambda = ' + lam + ' gamma = ' + gamma
axarr[1,1].set_title(Title, fontsize=12, fontweight='bold')
axarr[1,1].set_xlabel('Perimeter', fontsize=10)
axarr[1,1].set_ylabel('Number of Cells', fontsize = 10)
axarr[1,1].set_axis_bgcolor('white')
#axarr[1,1].grid(b=0)
Title = 'Target Area = ' + str(area)
plt.suptitle(Title, fontsize=16, fontweight='bold')
plt.savefig('Images/graphs.png')

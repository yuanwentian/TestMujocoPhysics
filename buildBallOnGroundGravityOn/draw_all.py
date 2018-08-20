import numpy as np
import pandas as pd
import matplotlib.pyplot as plt



# pos 读入的所有数据
# step 时间间隔
# i 第几列数据 (从1开始)
# name 图片名称，比如 acceleration.png
def picturePosition(data,step,i,name):
	i=i-1
	col = np.array([float(x.split(' ')[i]) for x in data])
	length = len(col)
	total = length*step
	xtic = np.arange(11)*(total/10)
	x = np.arange(length)*step
	plt.figure(figsize=(10,10))
	plt.plot(x,col)
	plt.xticks(xtic)
	plt.xlabel('time',fontsize=18)
	plt.ylabel('position',fontsize=18)
	plt.savefig(name)
	#plt.show()

def pictureVelocity(data,step,i,name):
	i=i-1
	col = np.array([float(x.split(' ')[i]) for x in data])
	length = len(col)
	total = length*step
	xtic = np.arange(11)*(total/10)
	x = np.arange(length)*step
	plt.figure(figsize=(10,10))
	plt.plot(x,col)
	plt.xticks(xtic)
	plt.xlabel('time',fontsize=18)
	plt.ylabel('velocity',fontsize=18)
	plt.savefig(name)
	#plt.show()

def pictureAcceleration(data,step,i,name):
	i=i-1
	col = np.array([float(x.split(' ')[i]) for x in data])
	length = len(col)
	total = length*step
	xtic = np.arange(11)*(total/10)
	x = np.arange(length)*step
	plt.figure(figsize=(10,10))
	plt.plot(x,col)
	plt.xticks(xtic)
	plt.xlabel('time',fontsize=18)
	plt.ylabel('acceleration',fontsize=18)
	plt.savefig(name)
	#plt.show()

def pictureContact(data,step,i,name):
	i=i-1
	col = np.array([float(x.split(' ')[i]) for x in data])
	length = len(col)
	total = length*step
	xtic = np.arange(11)*(total/10)
	x = np.arange(length)*step
	plt.figure(figsize=(10,10))
	plt.plot(x,col)
	plt.xticks(xtic)
	plt.xlabel('time',fontsize=18)
	plt.ylabel('contact force',fontsize=18)
	plt.savefig(name)
	#plt.show()

if __name__ == "__main__":
	with open('position.txt','r') as f:
		data1 = f.readlines()
	with open('velocity.txt','r') as f:
		data2 = f.readlines()
	with open('acceleration.txt','r') as f:
		data3 = f.readlines()
	with open('contact.txt','r') as f:
		data4 = f.readlines()

	picturePosition(data1,0.005,3,'position.png')
	pictureVelocity(data2,0.005,3,'velocity.png')
	pictureAcceleration(data3,0.005,3,'acceleration.png')
	pictureContact(data4,0.005,1,'contact.png')

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt



# pos 读入的所有数据
# step 时间间隔
# i 第几列数据 (从1开始)
# name 图片名称，比如 position.png
def picture(data,step,i,name):
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

if __name__ == "__main__":
	with open('velocity.txt','r') as f:
		data = f.readlines()

	picture(data,0.005,3,'velocity.png')

import matplotlib.pyplot as plt
KI = [1.0,4.0]
KM = [1.0,1.5]
KN = [0.5,1.0]
KF = [0.1,0.3]
PS = [0.01,0.0001]
i=0
	
for a1 in range(2):
	for a2 in range(2):
		for a3 in range(2):
			for a4 in range(2):
				for a5 in range(2):
					
					Ki = KI[a1]
					Km = KM[a2]
					Kn = KN[a3]
					Kf = KF[a4]
					Ps = PS[a5]
					file = open("outfile"+str(i)+".txt",'r')
					x=[]
					y=[]
					for each in file:
						y.append(float(each[:-1]))
					for j in range(2501):
						x.append(j)
					plt.plot(x, y)
					plt.xlabel('t[RTT]')
					plt.ylabel('cwnd[MSS]')
					plt.title("PLOT Number"+str(i+1)+": Ki = "+str(Ki)+", Km = "+str(Km)+", Kn = "+str(Kn)+", Kf = "+str(Kf)+", Ps = "+str(Ps)+";")
					plt.savefig("mygraph"+str(i)+".png")
					plt.clf()
					i=i+1

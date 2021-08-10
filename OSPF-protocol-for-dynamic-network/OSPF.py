import socket
import _thread
import time
import sys
import random

MAX_INT = sys.maxsize
n=0
localIP     = "127.0.0.1"
localPort   = 20001
bufferSize  = 1024
nodeid=-1
adj=[]
adjmat=[]
delay_hello = 0
delay_lsa = 0
delay_dijkstra = 0
outputfile = ""
# Create a datagram socket
lsalist=[]
lastupdatedseqnum=[]
class edge:
	friend=0
	minij=0
	maxij=10

#functions
delay = 2

def Hello(threadname):
	while 1:
		time.sleep(delay_hello)
		for i in adj[nodeid]:
			if i.friend>nodeid:
				msg="Hello"
				data = str.encode(msg)
				UDPServerSocket.sendto(data, ("127.0.0.1", localPort+i.friend))        
	return


def Scanner(threadname):
	while 1:
		msg,address = UDPServerSocket.recvfrom(bufferSize)  
		#print(str(msg)+" "+str(address[1]-localPort))
		if str(msg) == "b'Hello'":
			HelloReply(address[1])
		elif str(msg).split()[0] == "b'HelloReply":
			lsalist[int(str(msg).split()[1])] = int(str(msg).split()[3])
			adjmat[nodeid][address[1]-localPort] = int(str(msg).split()[3])
			adjmat[address[1]-localPort][nodeid] = int(str(msg).split()[3])
		elif str(msg).split()[0] == "b'LSA":
			src = int(str(msg).split()[1])
			seqnum = int(str(msg).split()[2])
			tot = int(str(msg).split()[3])
			if lastupdatedseqnum[src] < seqnum:
				lastupdatedseqnum[src] = seqnum
				for i in range(tot):
					vertex = int(str(msg).split()[4+2*i])
					edge = int(str(msg).split()[5+2*i])
					adjmat[src][vertex] = edge
					adjmat[vertex][src] = edge
				#update the graph
				LSAtransfer(msg)
	return 

def HelloReply(address):
	mini =0
	maxi=10
	for i in adj[nodeid]:
		if address-localPort == i.friend:
			mini = i.minij
			maxi = i.maxij
	randomnum = random.randint(mini,maxi)
	msg = "HelloReply "+str(nodeid)+" "+str(address-localPort)+" "+str(randomnum)+" "
	data = str.encode(msg)
	UDPServerSocket.sendto(data, ("127.0.0.1", address))	
	return

def LSAsend(threadname):
	count=0
	LSA_lock = _thread.allocate_lock()
	while 1:
		time.sleep(delay_lsa)
		LSA_lock.acquire()
		lsalistcpy = lsalist.copy()
		LSA_lock.release()
		s = "LSA "+str(nodeid)+" "+str(count)+" "
		count = count+1
		totalneighbors=0
		s2=""
		for i in range(len(lsalistcpy)):
			if lsalistcpy[i] != -1:
				totalneighbors = totalneighbors + 1	
				s2 += str(i)
				s2 += " "
				s2 += str(lsalistcpy[i])
				s2 += " "
		s += str(totalneighbors)
		s += " "
		s += s2
		s += " "
		data = str.encode(s)
		for i in adj[nodeid]:
			UDPServerSocket.sendto(data,(localIP , localPort+i.friend))
	return
def LSAtransfer(msg):
	#print("ha")

	for i in adj[nodeid]:
		UDPServerSocket.sendto(msg, (localIP, localPort+i.friend))
	return
def Dijkstra(threadname):
	count=0
	#print("hi")
	dijk_lock = _thread.allocate_lock()
	while 1:
		time.sleep(delay_dijkstra)
		dijk_lock.acquire()
		adjmatcpy = adjmat.copy()
		dijk_lock.release()
		n = len(adjmatcpy[0])
		count=count+1
		distances = []
		paths = []
		flag = []
		s = []
		for i in range(n):
			distances.append(100000)
			paths.append("")
			flag.append(0)
		distances[nodeid] = 0
		paths[nodeid] = str(nodeid)
		flag[nodeid] = 1
		init = str(nodeid)
		for i in range(n):
			if adjmatcpy[nodeid][i] <100000:
				paths[i] = init + str(i)
				distances[i] = adjmatcpy[nodeid][i]

		while len(s)<n:
			mini = MAX_INT
			minindex=0
			for i in range(n):
				if flag[i] == 1:
					continue
				if distances[i]<mini:
					mini = distances[i]
					minindex=i
			src = minindex
			flag[src] = 1
			init = paths[minindex]
			s.append(src)

			for i in range(n):
				if distances[src]+adjmatcpy[src][i]<distances[i]:
					paths[i] = init + str(i)
					distances[i] = distances[src] + adjmat[src][i]
		file2 = open(outputfile,"a")
		file2.write("==========================================\n")
		file2.write("Routing Table for Node number. "+str(nodeid)+" at Time "+str(count*delay_dijkstra)+"\n")
		file2.write("------------------------------------------\n")
		file2.write("|  Destination   |   Path   |   Cost      |\n")
		file2.write("------------------------------------------\n")
		for i in range(n):
			if i == nodeid:
				continue
			file2.write("|       "+str(i)+"        |   "+paths[i]+"    |    "+str(distances[i])+"         |\n")
	return
def writeoutput():
	return

#main
#step-1 open input fileand mark the neighbours of current node
#lets assume input file has been scanned,we need to store all entries 
#containing this particular node

nodeid = int(sys.argv[1])
inputfile = str(sys.argv[2])
outputfile = str(sys.argv[3])
delay_hello =int(sys.argv[4])
delay_lsa = int(sys.argv[5])
delay_dijkstra = int(sys.argv[6])

file = open(inputfile,"r")
lines = file.readlines()

n = int(lines[0].split()[0])
e = int(lines[0].split()[1])

for i in range(n):
	lastupdatedseqnum.append(-1)
	lsalist.append(-1)
	adj.append([])
for i in range(n):
	x=[]
	for i in range(n):
		x.append(100000)
	adjmat.append(x)

for i in range(e):
	item = edge()
	item.friend = int(lines[i+1].split()[1])
	item.minij = int(lines[i+1].split()[2])
	item.maxij = int(lines[i+1].split()[3])
	adj[int(lines[i+1].split()[0])].append(item)
for i in range(e):
	item = edge()
	item.friend = int(lines[i+1].split()[0])
	item.minij = int(lines[i+1].split()[2])
	item.maxij = int(lines[i+1].split()[3])
	adj[int(lines[i+1].split()[1])].append(item)
#step-2 store all neighbors or create graph


UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
# Bind to address and ip
UDPServerSocket.bind((localIP, localPort+nodeid))
print("Node:"+str(nodeid)+" is launched successfully")

time.sleep(4)

try:
	_thread.start_new_thread( Hello, ("Thread-1" , ) )
	_thread.start_new_thread( Scanner, ("Thread-2" , ) )
	_thread.start_new_thread( LSAsend, ("Thread-3" , ) )
#	_thread.start_new_thread( ScannerLSA, ("Thread-4" , ) )
	_thread.start_new_thread( Dijkstra, ("Thread-4" , ) )
except:
	print ("Error: unable to start thread")

while 1:
	pass

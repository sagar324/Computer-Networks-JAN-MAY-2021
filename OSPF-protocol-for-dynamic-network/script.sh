make
python3.6 OSPF.py 0 input1.txt output0.txt 1 5 20 &
python3.6 OSPF.py 1 input1.txt output1.txt 1 5 20 &
python3.6 OSPF.py 2 input1.txt output2.txt 1 5 20 &
python3.6 OSPF.py 3 input1.txt output3.txt 1 5 20 &
python3.6 OSPF.py 4 input1.txt output4.txt 1 5 20 &
python3.6 OSPF.py 5 input1.txt output5.txt 1 5 20 &
python3.6 OSPF.py 6 input1.txt output6.txt 1 5 20 &
python3.6 OSPF.py 7 input1.txt output7.txt 1 5 20 

sleep 5

make clean

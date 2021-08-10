#include <bits/stdc++.h>
#include <random>
using namespace std;
double Ki,Km,Kn,Kf,Ps;
double MSS=1;
double RWS = 1000;
double tot;
double itr=0;
string filename;
double cwnd;

double ssthresh=0;
int prob(){
	 std::random_device rd;
	 std::mt19937 generator(rd());
	 std::bernoulli_distribution distribution(Ps);

	 if(distribution(generator)){
		return 1;
	 }
	 return 0;
}
void write(string filename,double cwnd,int itr){
	ofstream fout;
	fout.open(filename+".txt",ios::app);
	string output;
	output = std::to_string(cwnd);
	fout<<output<<endl;
	fout.close();
}
int main(){
	string di,dm,dn,df,ds,dT,d0;
	//cin>>di>>Ki>>dm>>Km>>dn>>Kn>>df>>Kf>>ds>>Ps>>dT>>tot>>d0>>filename;
	tot = 2500;
	double KI[2] = {1.0,4.0};
	double KM[2] = {1.0,1.5};
	double KN[2] = {0.5,1.0};
	double KF[2] = {0.1,0.3};
	double PS[2] = {0.01,0.0001};
	int number = 0;
	for(int a1=0;a1<2;a1++){
		for(int a2=0;a2<2;a2++){
			for(int a3=0;a3<2;a3++){
				for(int a4=0;a4<2;a4++){
					for(int a5=0;a5<2;a5++){
						Ki=KI[a1];
						Km=KM[a2];
						Kn=KN[a3];
						Kf=KF[a4];
						Ps=PS[a5];
						filename = "outfile"+std::to_string(number);
						cwnd =  Ki*MSS;
						ssthresh=0;
						//write(filename,cwnd,itr);
						int flag=0;
						for(itr=0;itr<=tot;itr++){
							//check for timeout;
							int timeout = prob();
							if(timeout == 1){
								write(filename,cwnd,itr);
							    ssthresh = cwnd/2;
			                    cwnd = max(1.0,Kf*cwnd);
			                    flag++;
			                    continue;
		                    }
		                    write(filename,cwnd,itr);
		                    if(flag==0||cwnd<ssthresh){
			                   cwnd = min(cwnd + Km*MSS, RWS);
		                    }
		                    else{
			                   cwnd = min((cwnd + Kn*MSS*(MSS/cwnd)),RWS);
		                    }
	                    }
						number++;
					}
				}
			}
		}
	}
	//
   
	return 0;
}
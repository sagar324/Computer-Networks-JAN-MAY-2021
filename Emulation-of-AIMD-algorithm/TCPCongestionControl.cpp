#include <bits/stdc++.h>
#include <random>
#include <string>
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
	 random_device rd;
     mt19937 generator(rd());
     bernoulli_distribution distribution(Ps);

     if(distribution(generator)){
     	return 1;
     }
     return 0;
}
void write(string filename,double cwnd,int itr){
	ofstream fout;
	fout.open(filename+".txt",ios::app);
	string output;
	output = to_string(cwnd);
	fout<<output<<endl;
	fout.close();
}
int main(){
	string di,dm,dn,df,ds,dT,d0;
	cin>>di>>Ki>>dm>>Km>>dn>>Kn>>df>>Kf>>ds>>Ps>>dT>>tot>>d0>>filename;
    cwnd =  Ki*MSS;
    //write(filename,cwnd,itr);
    int flag=0;
    for(;itr<=tot;itr++){
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
	return 0;
}
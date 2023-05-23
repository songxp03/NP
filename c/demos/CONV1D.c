#include <stdio.h>
#include <math.h>

#define imax 50
#define xdim 10.0f

#define HybridScheme 0
#define FirstOrderUpwindScheme 1
#define PowerLawScheme 2
#define ExponentialScheme 3
#define CenteredDifferenceScheme 4

#define source 0.05f
#define U 1.0f

float fmax(float a , float b){
  if( a>b) return a;

  return b;
}

float AFunc(float peclet,int scheme){
	peclet=fabs(peclet);
	
	switch (scheme){
		case HybridScheme: return fmax(0,1-0.5*peclet);
		case FirstOrderUpwindScheme: return 1;
		case PowerLawScheme: return fmax(0,1-0.1*pow(peclet,5));
		case ExponentialScheme: return peclet/(exp(peclet)-1);		
		case CenteredDifferenceScheme: return 1-0.5*peclet;
		default:break;
	}
	return 0;
}

struct Node1D{
    float aE;float aW;float aP;float aP0;float b;
    float x; float T; float T0; float Vol;
    float Sc;float Sp;
};

struct Node1D nodes[imax+1];

int main() {
    float deltax=xdim/imax;
  	float rho=1,Cp=1,lmd=1;
  	
  	for (int i=0; i<=imax; i++) {//init
  	    nodes[i].x=i*deltax;
  		nodes[i].T=0;
  		nodes[i].T0=0;
  		nodes[i].Sc=source; 
  		nodes[i].Sp=0;
  	}
  	
  	for (int i=1; i<imax; i++) {
  	    nodes[i].Vol=deltax*1*1;
  	    
  		float Diff_e=lmd/deltax;
    	float Flux_e=rho*U;
    	float Peclet_e=Flux_e/Diff_e;
    	
    	nodes[i].aE=Diff_e*AFunc(Peclet_e,HybridScheme)+fmax(0,-Flux_e);
    	nodes[i].aE*=1*1;
    	
    	float Diff_w=lmd/deltax;
    	float Flux_w=rho*U;
    	float Peclet_w=Flux_w/Diff_w;
    	
    	nodes[i].aW=Diff_w*AFunc(Peclet_w,HybridScheme)+fmax(0,Flux_w);
    	nodes[i].aW*=1*1;
    
    	nodes[i].aP0=0;//steady State
    			
    	nodes[i].aP=nodes[i].aE+nodes[i].aW+nodes[i].aP0-nodes[i].Sp*nodes[i].Vol;
    	
    	nodes[i].b=nodes[i].Sc*nodes[i].Vol+nodes[i].aP0*nodes[i].T0;
  	}
    
    nodes[0].T=0;
    
    nodes[imax].T=1;

    for(int iter=0;iter<1000;iter++)
    for (int i=1; i<imax; i++) {
        nodes[i].T=(nodes[i].aW*nodes[i-1].T+nodes[i].aE*nodes[i+1].T+nodes[i].b)/nodes[i].aP;
    }

   for(int i=0;i<=imax;i++){
      printf("chart%d %d %f %f\n",1,1,nodes[i].x,nodes[i].T);//ID,style,data...
   }

   printf("Solution Done!\n");

   return 0;
}
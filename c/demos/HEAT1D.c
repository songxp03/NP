#include <stdio.h>

#define imax 10
#define xdim 1.0f

struct Node1D{
    float aE;float aW;float aP;float b;
    float x; float T;
};

struct Node1D nodes[imax+1];

int main() {
    float deltax=xdim/imax;
  	float rho=1,Cp=1,lmd=1;
  	
  	for (int i=0; i<=imax; i++) {
  	    nodes[i].x=i;
  		nodes[i].T=0;
  	    nodes[i].aE=1.0f*lmd/deltax;
  		nodes[i].aW=1.0f*lmd/deltax;
  		nodes[i].aP=nodes[i].aE+nodes[i].aW;
  		nodes[i].b=0.0f;
  	}
    
    nodes[0].T=1;
    nodes[0].aE=0.5f*lmd/deltax;
    
    nodes[imax].T=0;
    nodes[imax].aW=0.5f*lmd/deltax;

    for(int iter=0;iter<15;iter++)
    for (int i=1; i<imax; i++) {
        nodes[i].T=(nodes[i].aW*nodes[i-1].T+nodes[i].aE*nodes[i+1].T+nodes[i].b)/nodes[i].aP;
    }

   for(int i=0;i<=imax;i++){
      printf("chart%d %d %f %f\n",1,1,nodes[i].x,nodes[i].T);//ID,style,data...
   }

   printf("End!\n");

   return 0;
}
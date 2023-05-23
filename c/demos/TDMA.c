#include <stdio.h>
#include <stdlib.h>

#define idxA(i,j) (i*2+j-2)
#define idxL(i,j) (i+j-2)
#define idxU(i,j) (i-1)
#define idxb(i) (i-1)
#define idxz(i) (i-1)

void TDMASolver(int dim,float* A,float* b,float* root){
	int i=0;

	float* L=(float*) malloc((dim*2-1)*sizeof(float));
	float* U=(float*) malloc((dim-1)*sizeof(float));
	float* z=(float*) malloc(dim*sizeof(float));

	// Step 1
	L[idxL(1,1)]=A[idxA(1,1)];
	U[idxU(1,2)]=A[idxA(1,2)]/L[idxL(1,1)];
	z[idxz(1)]=b[idxb(1)]/L[idxL(1,1)];
	
	//Step 2
	for (i=2;i<dim;i++)	{
		L[idxL(i,i-1)]=A[idxA(i,i-1)];
		L[idxL(i,i)]=A[idxA(i,i)]-L[idxL(i,i-1)]*U[idxU(i-1,i)];
		U[idxU(i,i+1)]=A[idxA(i,i+1)]/L[idxL(i,i)];
		z[idxz(i)]=(b[idxb(i)]-L[idxL(i,i-1)]*z[idxz(i-1)])/L[idxL(i,i)];
	}
	
	//Step 3, Now i=n
	L[idxL(i,i-1)]=A[idxA(i,i-1)];
	L[idxL(i,i)]=A[idxA(i,i)]-L[idxL(i,i-1)]*U[idxU(i-1,i)];
	z[idxz(i)]=(b[idxb(i)]-L[idxL(i,i-1)]*z[idxb(i-1)])/L[idxL(i,i)];

	//Step 4
	root[dim-1]=z[dim-1];
	
	//Step 5
	for (i=dim-1;i>0;i--)	{
		root[i-1]=z[idxz(i)]-U[idxU(i,i+1)]*root[i];
	}

	free(L);
	free(U);
	free(z);

	L=NULL;
	U=NULL;
	z=NULL;
}

int main(){
	float A[12]={0,1,2,
			  2,3,4,
			  4,5,6,
			  6,7,8};

	float b[4]={3,9,15,13};

	float root[4];

	TDMASolver(4,A,b,root);

	for(int i=0;i<4;i++) printf("root[%d] = %f\n",i,root[i]);

	return 0;
}
#include <stdio.h>
#include <math.h>

#define imax 50
#define jmax 30
#define PI 3.14159265358979

float tmpr[imax+1][jmax+1];

int main() {
    for(int j=0;j<=jmax;j++){
        tmpr[0][j]=0.05*j*(30.0-j);//左侧
        tmpr[imax][j]=0;//右侧
    }

    for(int i=0;i<=imax;i++){//
        tmpr[i][0]=10*sin(PI*i/50.0);
        tmpr[i][jmax]=0;//
    }
    
    float err,iter=0,tmp;
    int N=imax*jmax;/*节点总数*/
    int maxIter=1000;/*最大迭代次数*/
    do{
        err=0;/*误差*/
        for(int i=1;i<imax;i++){
            for(int j=1;j<jmax;j++){
                tmp=0.25*(tmpr[i+1][j]+tmpr[i-1][j]+tmpr[i][j+1]+tmpr[i][j-1]);
                err+=(tmp-tmpr[i][j])*(tmp-tmpr[i][j]);
                tmpr[i][j]=tmp;
            }
        }
        iter++;//迭代次数
        err=sqrt(err/N);
    }while(err>1E-3);
    printf("迭代%d次后最终误差 %f\n",iter,err);

    for(int i=0;i<=imax;i++){
        for(int j=0;j<=jmax;j++){
            printf("chart%d %d %d %d %f\n",66,2,i*9,j*9,tmpr[i][j]);
        }
    }

    return 0;
}
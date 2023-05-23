#include <stdio.h>
#include <string.h>
#include <math.h>

#define iterMax 10

#define imax 20
#define jmax 20

float xmax=1,ymax=1,dx,dy;

float Re = 100;
float rho = 1;

int outeriter=10;//number of outer iterations

// set relaxation factors
float omegau=0.7;    //u-momentum underrelaxation
float omegav=0.7;    //v-momentum underrelaxation
float omegap=0.3;    //pressure update underrelaxation

float u[imax+2][jmax+2];//每回合初始猜的，和最终值
float uold[imax+2][jmax+2];//用于迭代

float v[imax+2][jmax+2];//每回合初始猜的，和最终值
float vold[imax+2][jmax+2];//用于迭代

float p[imax+2][jmax+2];//交错网格P
float pc[imax+2][jmax+2];//压力修正
float pp[imax+2][jmax+2];//导出P
float p_source[imax+2][jmax+2];// 其实这东西就是散度，用于解泊松压力方程

float Ae[imax+2][jmax+2];
float Aw[imax+2][jmax+2];
float An[imax+2][jmax+2];
float As[imax+2][jmax+2];
float Apu[imax+2][jmax+2];
float Apv[imax+2][jmax+2];
float App[imax+2][jmax+2];

float uu[imax+2][jmax+2];
float vv[imax+2][jmax+2];

float fmax(float a , float b){
  if( a>b) return a;

  return b;
}


void init(){
  memset(u,0, (imax+2)*(jmax+2)* sizeof(float) );
  memset(uold,0, (imax+2)*(jmax+2)* sizeof(float) );

  memset(v,0, (imax+2)*(jmax+2)* sizeof(float) );
  memset(vold,0, (imax+2)*(jmax+2)* sizeof(float) );

  memset(p,0, (imax+2)*(jmax+2)* sizeof(float) );
  memset(p_source,0, (imax+2)*(jmax+2)* sizeof(float) );

  xmax=1.0;//set domain size  (increase length if channel)
  ymax=1.0;

  dx=xmax/imax;//compute mesh spacing
  dy=ymax/jmax;

  for(int i=0;i<imax+2;i++){
    u[i][jmax+1]=1;//顶驱速度
  }
}

void resetA(){
  memset( Ae, 0, (imax+2)*(jmax+2)* sizeof(float) );
  memset( Aw, 0, (imax+2)*(jmax+2)* sizeof(float) );
  memset( An, 0, (imax+2)*(jmax+2)* sizeof(float) );
  memset( As, 0, (imax+2)*(jmax+2)* sizeof(float) );
}

void calcU(){
  resetA();

  for(int i=2;i<imax+1;i++){
    for(int j=1;j<jmax+1;j++){
      float Fe = 0.5 * (uold[i][j] + uold[i + 1][j]) * dy * rho;
      float Fw = 0.5 * (uold[i][j] + uold[i - 1][j]) * dy * rho;
      float Fn = 0.5 * (vold[i][j+1] + vold[i-1][j+1]) * dx * rho;
      float Fs = 0.5 * (vold[i][j] + vold[i-1][j]) * dx * rho;

      Ae[i][j] = fmax(0, -Fe) + dy / dx / Re;
      Aw[i][j] = fmax(0, Fw) + dy / dx / Re;
      An[i][j] = fmax(0, -Fn) + dx / dy / Re;
      As[i][j] = fmax(0, Fs) + dx / dy / Re;
      Apu[i][j] = Ae[i][j] + Aw[i][j] + An[i][j] + As[i][j];// + Fe - Fw + Fn - Fs;
    }
  }

  // 由于交错网格的设置，最后一格速度到墙壁只有半格的距离，也就是dx/2或dy/2
  for(int i=2;i<imax+1;i++){
    As[i][1] += dx / dy / Re;
    An[i][jmax] += dx / dy / Re;
    Apu[i][1] += dx / dy / Re;
    Apu[i][jmax] += dx / dy / Re;
  }

  for(int k=1;k<iterMax;k++){
    for(int i=2;i<imax+1;i++){
      for(int j=1;j<jmax+1;j++){
        float term = Ae[i][j] * uold[i+1][j] + Aw[i][j] * uold[i-1][j];
        term += As[i][j] * uold[i][j-1] + An[i][j] * uold[i][j+1];
        float pressure_term = (p[i-1][j] - p[i][j]) * dy;
        u[i][j] = (1.-omegau)*uold[i][j];
        u[i][j] += omegau * (term + pressure_term) / Apu[i][j];
      }
    }
  }
}

void calcV(){
  resetA();

  for(int i=1;i<imax+1;i++){
    for(int j=2;j<jmax+1;j++){
      float Fe = 0.5 * (uold[i+1][j] + uold[i+1][j-1]) * dy * rho;
      float Fw = 0.5 * (uold[i][j] + uold[i][j-1]) * dy * rho;
      float Fn = 0.5 * (vold[i][j] + vold[i][j+1]) * dx * rho;
      float Fs = 0.5 * (vold[i][j] + vold[i][j-1]) * dx * rho;

      Ae[i][j] = fmax(0, -Fe) + dy / dx / Re;
      Aw[i][j] = fmax(0, Fw) + dy / dx / Re;
      An[i][j] = fmax(0, -Fn) + dx / dy / Re;
      As[i][j] = fmax(0, Fs) + dx / dy / Re;

      Apv[i][j] = Ae[i][j] + Aw[i][j] + An[i][j] + As[i][j];// + Fe - Fw + Fn - Fs;
    }
  }

  // 由于交错网格的设置，最后一格速度到墙壁只有半格的距离，也就是dx/2或dy/2
  for(int j=2;j<jmax+1;j++){
    Ae[imax][j] += dy / dx / Re;
    Aw[1][j] += dy / dx / Re;
    Apv[imax][j] += dy / dx / Re;
    Apv[1][j] += dy / dx / Re;
  }

  for(int k=1;k<iterMax;k++){
    for(int i=1;i<imax+1;i++){
      for(int j=2;j<jmax+1;j++){
        float term = Ae[i][j] * vold[i+1][j] + Aw[i][j] * vold[i-1][j];
        term += As[i][j] * vold[i][j-1] + An[i][j] * vold[i][j+1];
        float pressure_term = (p[i][j-1] - p[i][j]) * dx;
        v[i][j] = (1.-omegav)*vold[i][j];
        v[i][j] += omegav * (term + pressure_term) / Apv[i][j];
      }
    }
  }
}

void calcP(){
  memset( p_source, 0, (imax+2)*(jmax+2)* sizeof(float) );

  for(int i=1;i<imax+1;i++){
    for(int j=1;j<jmax+1;j++){
      Ae[i][j] = rho * dy * dy / Apu[i+1][j];
      Aw[i][j] = rho * dy * dy / Apu[i][j];
      An[i][j] = rho * dx * dx / Apv[i][j+1];
      As[i][j] = rho * dx * dx / Apv[i][j];

      p_source[i][j] = rho*(u[i+1][j] - u[i][j])*dy + rho*(v[i][j+1]-v[i][j])*dx;
    }
  }

  for(int j=0;j<jmax+1;j++){
    Ae[imax][j]=0;
    Aw[1][j]=0;
  }

  for(int i=0;i<imax+1;i++){
    An[i][jmax]=0;
    As[i][1]=0;
  }

  for(int i=0;i<imax+1;i++){
    for(int j=0;j<jmax+1;j++){
      App[i][j] = Ae[i][j] + Aw[i][j] + An[i][j] + As[i][j];
    }
  }

  App[1][1] =1.E30;//set reference cell value for pressure

  memset( pc, 0, (imax+2)*(jmax+2)* sizeof(float) ); //initialize corrections to zero
  

  //SOR iterations to solve for pressure correction pc
  for(int k=1;k<100;k++){
    for(int j=1;j<jmax+1;j++){
      for(int i=1;i<imax+1;i++){
        float term = Ae[i][j] * pc[i+1][j] + Aw[i][j] * pc[i-1][j];
        term += An[i][j] * pc[i][j+1] + As[i][j] * pc[i][j-1];
        term = (term - p_source[i][j] -pc[i][j]* App[i][j]); 

        pc[i][j]+=1.7/App[i][j]*term;
      }
    }
  }
}

void correctP(){
  //校正压力
  for(int i=1;i<imax+1;i++){
    for(int j=1;j<jmax+1;j++){
      p[i][j] += omegap * pc[i][j];
    }
  }
}

void correctUV(){
  //校正速度U
  for(int i=2;i<imax+1;i++){
    for(int j=1;j<jmax+1;j++){
      u[i][j] += dy / Apu[i][j] * (pc[i-1][j] - pc[i][j]);
    }
  }
  //校正速度V
  for(int i=1;i<imax+1;i++){
    for(int j=2;j<jmax+1;j++){
      v[i][j] = v[i][j] + dx / Apv[i][j] * (pc[i][j-1] - pc[i][j]);
    }
  }
}

void calcRealUVP(){
  for (int i = 2; i < imax+1; i ++){
    for (int j = 2; j < jmax+1; j ++) {
        uu[i][j]=0.5*(u[i][j-1]+u[i][j]);
        vv[i][j]=0.5*(v[i-1][j]+v[i][j]);
        pp[i][j]=0.25*(p[i-1][j-1]+p[i-1][j]+p[i][j-1]+p[i][j]);
    }
  }
}

void correctUVP(){
  //east and west boundaries
  for (int j = 2; j < jmax+1; j ++) {
    uu[1][j]=0.5*(u[1][j-1]+u[1][j]);
    uu[imax+1][j]=0.5*(u[imax+1][j-1]+u[imax+1][j]);
    vv[1][j]=v[0][j];
    vv[imax+1][j]=v[imax+1][j];
    pp[1][j]=0.5*(p[1][j-1]+p[1][j]);
    pp[imax+1][j]=0.5*(p[imax][j-1]+p[imax][j]);
  }

  //north and south boundaries
  for (int i = 2; i < imax+1; i ++){
    uu[i][1]=u[i][0];
    uu[i][jmax+1]=u[i][jmax+1];
    vv[i][1]=0.5*(v[i-1][1]+v[i][1]);
    vv[i][jmax+1]=0.5*(v[i-1][jmax+1]+v[i][jmax+1]);
    pp[i][1]=0.5*(p[i-1][1]+p[i][1]);
    pp[i][jmax+1]=0.5*(p[i-1][jmax]+p[i][jmax]);
  }

  //southwest corner
  uu[1][1]=0;
  vv[1][1]=0;
  pp[1][1]=pp[2][2];

  //southeast corner
  uu[imax+1][1]=0;
  vv[imax+1][1]=0;
  pp[imax+1][1]=p[imax][1];

  //northeast corner
  uu[imax+1][jmax+1]=0 ;
  vv[imax+1][jmax+1]=0;
  pp[imax+1][jmax+1]=p[imax][jmax];

  //northwest corner
  uu[1][jmax+1]=0;
  vv[1][jmax+1]=0;
  pp[1][jmax+1]=p[1][jmax];
}

void export(char *tecfile){
  int cnt=0;
  for (int i = 1; i < imax+2; i++) {
    for (int j = 1; j <jmax+2; j++) {
      printf("chart%d %d %f %f %f %f\n",99,0,i*dx,j*dy,uu[i][j],vv[i][j]);//ID,style,data...
    }
  }
}

void step(){
  memcpy( uold,u,(imax+2)*(jmax+2)* sizeof(float) );
  memcpy( vold,v,(imax+2)*(jmax+2)* sizeof(float) );

  calcU();
  calcV();
  calcP();

  correctP();
  correctUV();
}

void runsim(){
  init();

  for(int iter=0;iter<outeriter;iter++){
    step();
    printf("iter %d\n",iter);
  }

  calcRealUVP();

  correctUVP();

  export("tec.dat");
}

int main() {
  runsim();
  
  printf("Compiled by X. P. 宋 [SongSoftStudio]\n");
  return (0);
}
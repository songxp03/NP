<template>
  <el-row class="mb-4">
    <canvas ref="myCanvas" width="500" height="350" />
  </el-row>
</template>

<script lang="ts" setup>
import {onMounted,ref} from 'vue';

const myCanvas=ref()

var VectorUtil={};
  
VectorUtil.NORMAL=function (vector){
  var result = 0.0,len=vector.length;
  for (var i=0;i<len;i++) result += vector[i]*vector[i];
  return Math.sqrt(result);
}

VectorUtil.ASSIGN=function (vector,value){
  var len=vector.length;
  for (var i=0;i<len;i++) vector[i]=value;
}
//行列的编号都是从1开始的，而不是0
function idxA(i,j) {return i*2+j-2;}
function idxL(i,j) {return i+j-2;}
function idxU(i,j) {return i-1;}
function idxb(i) {return i-1;}
function idxz(i) {return i-1;}

function SolveByTDMA(dim,A,b,root){
  /*
    TDMA method

    For Linear Equation Ax=b 
    =>  LUx=b
    =>  Lz=b  
    =>  Ux=z
  */
  
  var i=0;

  var L=new Array(dim*2-1);
  var U=new Array(dim-1);
  var z=new Array(dim);

  // Step 1
  L[idxL(1,1)]=A[idxA(1,1)];
  U[idxU(1,2)]=A[idxA(1,2)]/L[idxL(1,1)];
  z[idxz(1)]=b[idxb(1)]/L[idxL(1,1)];
  
  //Step 2
  for (i=2;i<dim;i++){
    L[idxL(i,i-1)]=A[idxA(i,i-1)];
    L[idxL(i,i)]=A[idxA(i,i)]-L[idxL(i,i-1)]*U[idxU(i-1,i)];
    U[idxU(i,i+1)]=A[idxA(i,i+1)]/L[idxL(i,i)];
    z[idxz(i)]=(b[idxb(i)]-L[idxL(i,i-1)]*z[idxz(i-1)])/L[idxL(i,i)];
  }
  
  //Step 3, Now i=n
  L[idxL(i,i-1)]=A[idxA(i,i-1)];
  L[idxL(i,i)]=A[idxA(i,i)]-L[idxL(i,i-1)]*U[idxU(i-1,i)];
  z[idxz(i)]=(b[idxb(i)]-L[idxL(i,i-1)]*z[idxz(i-1)])/L[idxL(i,i)];

  //Step 4
  root[dim-1]=z[dim-1];
  
  //Step 5
  for (i=dim-1;i>0;i--){
    root[i-1]=z[idxz(i)]-U[idxU(i,i+1)]*root[i];
  }

  return getResidualTDMA(dim,A,b,root);
}

function getResidualTDMA(dim,A,b,root){
  var resNum=new Array(dim),resDen=new Array(dim);
  VectorUtil.ASSIGN(resNum,0.0);
  for(var row=1;row<=dim;row++){
    for(var col=row-1;col<=row+1;col++){
      if(col<1) continue;
      if(col>dim) continue;
      resNum[row-1]+=A[idxA(row,col)]*root[col-1];
    }
    resNum[row-1]-=b[row-1];
    resDen[row-1]=A[idxA(row,row)]*root[row-1];
  }

  return VectorUtil.NORMAL(resNum)/VectorUtil.NORMAL(resDen);
}
  
function analyticalResult(x){
  if(x<12.5)  return x;
  else if(x<37.5) return 0.1*x+10.25;
  else return x-22.5;
}

onMounted(()=>{ 
  var myChart = echarts.init(myCanvas.value);
  
  function BC(type=1){
    this.type=type;
    this.value=0;
    this.alpha=1;
  }

  function SimpleMaterial(lmd,Cp,rho){
    this.lmd=lmd;
    this.Cp=Cp;
    this.rho=rho;
  }

  var Node1D=function (x){
    this.x=x;
    this.west=null;
    this.east=null;

    this.T=0;
    this.T0=0;
    this.Vol=0;
    this.lmd_w=0;
    this.lmd_e=0;
    this.Cp=0;
    this.rho=0;
    this.dx_w=0;
    this.dx_e=0;
    this.Se=1;
    this.Sw=1;
    this.aE=0;
    this.aW=0;
    this.aP=0;
    this.aP0=0;
    this.b=0;
    this.Sc=0;
    this.Sp=0;
    this.bcIndex=-1;

    this.ApplyBC=ApplyBC;
    this.CalcMatrics=CalcMatrics;
  };

  function ApplyBC(index,steadyState=flase){
    var bc=BCList[index];

    this.bcIndex=index;

    if(bc.type==1){
      this.aW=0;
      this.aE=0;
      this.aP=1;
      this.b=bc.value;
      this.T0=bc.value;
      this.T=bc.value;
      return;
    }else if(bc.type==2){
      this.aW=this.Sw*this.lmd_w/this.dx_w;
      this.aE=0;
      this.aP0=steadyState?0:this.rho*this.Cp*this.Vol/timeStep;
      this.Sc+=bc.value*this.Se/this.Vol;
      this.aP=this.aE+this.aW+this.aP0-this.Sp*this.Vol;
      this.b=this.Sc*this.Vol+this.aP0*this.T0;
      return;
    }else if(bc.type==3){
      this.aW=this.Sw*this.lmd_w/this.dx_w;
      this.aE=0;
      this.aP0=steadyState?0:this.rho*this.Cp*this.Vol/timeStep;
      this.Sc+=bc.value*bc.alpha*this.Se/this.Vol;
      this.Sp+=-bc.alpha*this.Se/this.Vol;
      this.aP=this.aE+this.aW+this.aP0-this.Sp*this.Vol;
      this.b=this.Sc*this.Vol+this.aP0*this.T0;
    }
  }

  function CalcMatrics(timeStep,steadyState=false){
    if(this.bcIndex>-1) return;

    this.aW=this.Sw*this.lmd_w/this.dx_w;
    this.aE=this.Se*this.lmd_e/this.dx_e;
    this.aP0=steadyState?0:this.rho*this.Cp*this.Vol/timeStep;
    this.aP=this.aE+this.aW+this.aP0-this.Sp*this.Vol;
    this.b=this.Sc*this.Vol+this.aP0*this.T0;
  }

  var Solution=function(nodes){
    if(nodes)
      this.nodes=nodes;
    else
      this.nodes=[];

    this.nx=10;
    this.dx=1;
    this.flowTime=0;

    this.SetUpGeometryAndMesh=SetUpGeometryAndMesh;
    this.ApplyMaterial=ApplyMaterial;
    this.SetUpBoundaryCondition=SetUpBoundaryCondition;
    this.Initialize=Initialize;
    this.CombineMatric=CombineMatric;
    this.GetLastError=GetLastError;
    this.Solve=Solve;
    this.UpdateOld=UpdateOld;
    this.ShowResults=ShowResults;
  };

  function SetUpGeometryAndMesh(nx,dx){
    this.nx=nx;
    this.dx=dx;

    for(var i=0;i<nx+3;i++){
      nodes[i]=new Node1D(i*dx);
    }

    for(var j=1;j<=nx+1;j++){
      nodes[j].west=nodes[j-1];
      nodes[j].east=nodes[j+1];
    }

    for(var k=1;k<=nx+1;k++){
      nodes[k].Vol=dx*1*1;
      nodes[k].Se=1;
      nodes[k].Sw=1;
      nodes[k].dx_w=dx;
      nodes[k].dx_e=dx;
    }

    nodes[1].Vol/=2;//index is 1 not 0
    nodes[nx+1].Vol/=2;
  }

  function ApplyMaterial(mtrl01,mtrl02){
    for(var j=1;j<=this.nx+1;j++){
      nodes[j].Cp=mtrl01.Cp;
      nodes[j].rho=mtrl01.rho;
      nodes[j].lmd_w=mtrl01.lmd;
      nodes[j].lmd_e=mtrl01.lmd;
    }

    for(var j=13;j<=37;j++){
      nodes[j].Cp=mtrl02.Cp;
      nodes[j].rho=mtrl02.rho;
      nodes[j].lmd_w=mtrl02.lmd;
      nodes[j].lmd_e=mtrl02.lmd;
    }

    var avg_lmd=2/(1/mtrl01.lmd+1/mtrl02.lmd);

    nodes[12].lmd_e=avg_lmd;
    nodes[13].lmd_w=avg_lmd;
    nodes[37].lmd_e=avg_lmd;
    nodes[38].lmd_w=avg_lmd;
  }

  function SetUpBoundaryCondition(){
    nodes[1].ApplyBC(0,true);
    nodes[this.nx+1].ApplyBC(1,true);
  }

  function Initialize(Tini,Tair,timeStep){
    for(var j=1;j<=this.nx+1;j++){
      nodes[j].T0=Tini;
      nodes[j].T=Tini;
    }
  }

  function CombineMatric(timeStep,AMatric,bRHS,steadyState){
    for(var i=1;i<=this.nx+1;i++){
      var node=nodes[i];
      node.CalcMatrics(timeStep,steadyState);
      var baseIndex=3*(i-1);
      AMatric[baseIndex+0]=-node.aW;
      AMatric[baseIndex+1]=node.aP;
      AMatric[baseIndex+2]=-node.aE;    
      bRHS[i-1]=node.b;
    }
  }

  function GetLastError(){
    var e1=0,e2=0;this;

    for(var i=1;i<=this.nx+1;i++){
      e1+=(nodes[i].T-nodes[i].T0)*(nodes[i].T-nodes[i].T0);
      e2+=nodes[i].T0*nodes[i].T0;
    }

    if(e2>0)  return e1/e2;
    return Number.NaN;
  }

  function Solve(iterCnt,timeStep,steadyState){
    var dim=this.nx+1;
    var AMatric=new Array(dim*3);
    var bRHS=new Array(dim);
    var root=new Array(dim);

    for (var iter = 0; iter < iterCnt; iter++) {
      this.UpdateOld();
      this.CombineMatric(timeStep,AMatric,bRHS,steadyState);
      SolveByTDMA(dim,AMatric,bRHS,root)
      console.log(iter,AMatric,bRHS,root);
      for(var j=1;j<=this.nx+1;j++){
        nodes[j].T=root[j-1];
      }

      if(steadyState) this.flowTime+=timeStep;

      var error=this.GetLastError();
      console.log(error);
      if(error<1E-6){
        console.log("Solution Done in ",iter," iterations with ",error);
        break;
      }   
    }
  }

  function UpdateOld(){
    for(var j=1;j<=this.nx+1;j++){
      nodes[j].T0=nodes[j].T;
    }
  }

  function ShowResults(){
    var x=[],y=[],realT=[];
    for(var i=1;i<=this.nx+1;i++){
      x[i-1]=nodes[i].x-1;
      y[i-1]=nodes[i].T;
      realT[i-1]=analyticalResult(x[i-1])
    }
    
    var option = {
      xAxis: {type: 'category',data: x},
      yAxis: {type: 'value'},
      legend: {data: ['数值解', '解析解'],top:40},
      series: [
        {data: y,type: 'line',name:'数值解'},
        {data: realT,type: 'line',name:'解析解'},
      ]
    };

    option && myChart.setOption(option);
  }

  var nodes=[];
  var BCList=[];

  function onSolve(){
    var solution=new Solution(nodes);

    var nx=50;
    var dx=1;
    solution.SetUpGeometryAndMesh(nx,dx);

    var mtrl01=new SimpleMaterial(1,1,1);
    var mtrl02=new SimpleMaterial(10,4,25);
    solution.ApplyMaterial(mtrl01,mtrl02);

    var Tini=0;
    var Tair=1; 
    var timeStep=0.1;
    console.log("timeStep is set to: "+timeStep);
    var iterations=3;

    var bc01=new BC(1),bc02=new BC(2);
    bc01.value=0;
    bc02.value=1;
    BCList.push(bc01,bc02);

    solution.SetUpBoundaryCondition();

    solution.Initialize(Tini,Tair,timeStep);

    var steadyState=true;
    solution.Solve(iterations,timeStep,steadyState);

    solution.ShowResults();
  }
  
  onSolve()
  
})
  
</script>

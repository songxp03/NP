<template>
  <el-row class="mb-4">
    <canvas ref="myCanvas" width="500" height="300" />
  </el-row>
  <el-row class="mb-4">
    <canvas ref="errCanvas" width="500" height="300" />
  </el-row>
</template>

<script lang="ts" setup>
import {getCurrentInstance,onMounted,ref} from 'vue';

//import * as echarts from 'echarts';
  
const { proxy } = getCurrentInstance();
  
onMounted(()=>{ 
  var dom =proxy.$refs.myCanvas;
  
  var myChart = echarts.init(dom, null, {
    renderer: 'canvas',
    useDirtyRect: false
  });
  
  var dom2 = proxy.$refs.errCanvas;
	var errChart = echarts.init(dom2);

  Array.create = function(dimension, initialValue){
    var arr = [];
    for(var i=0;i<dimension;i++){
      arr[i]=initialValue;
    }
    return arr;
  };

  Array.prototype.clone = function(){
    var arr=[];
    for(var i=0,len=this.length;i<len;i++) {
      arr.push(this[i]);
    }
    return arr;
  }

  var Solution=function(tmpr){
    this.tmpr=tmpr;

    this.SetUpGeometryAndMesh=SetUpGeometryAndMesh;
    this.SetUpBoundaryCondition=SetUpBoundaryCondition;
    this.Solve=Solve;
    this.ShowResults=ShowResults;
  };

  function SetUpGeometryAndMesh(dx){
    tmpr=Array.create(50+1,0);
    for(var i=0;i<50+1;i++){
      xs.push(i*dx);
    }
  }

  function SetUpBoundaryCondition(dx){
    tmpr[0]=1;//左侧
    tmpr[50]=0;//右侧
  }

  function Solve(timeStep,maxIter){
    for(var iter=1,N=51/*节点总数*/;iter<maxIter+1;iter++){
      for(var i=1,tmp,err=0;i<50;i++){
        tmp=0.5*(tmpr[i+1]+tmpr[i-1]);
        err+=(tmp-tmpr[i])*(tmp-tmpr[i]);
        tmpr[i]=tmp;
      }
      err=Math.sqrt(err/N);
      errors.push(err);iters.push(iter);
      if(iter==101) T100=tmpr.clone();
      if(iter==301) T300=tmpr.clone();
      if(iter==501) T500=tmpr.clone();
      if(err<1E-4) {
        console.info("收敛于第",iter,"次迭代",'最终误差：',err)
        break;
      }
    }
  }

  function ShowResults(errChart,tmprChart){
    //绘制残差
    var errOption = {
        title: {text: '残差变化趋势'},//图标题
        tooltip: {trigger: 'axis'},//提示信息
        xAxis: {type: 'category',data: iters,},yAxis: {type: 'value'},//x坐标和y坐标
        series: [{name:'残差',data: errors,type: 'line'},],
        legend: {data:['残差',]},//图例文字
        toolbox: {feature: {saveAsImage: {}}},//设置保存图片按钮
    };

    errChart.setOption(errOption, true);

    var tmprChart = echarts.init(dom);
    var tmprOption = {
        title: {text: '温度分布'},//图标题
        tooltip: {trigger: 'axis'},//提示信息
        xAxis: {type: 'category',data: xs,},yAxis: {type: 'value'},//x坐标和y坐标
        series: [{name:'数值解',data: tmpr,type: 'line'},{name:'iter=100',data: T100,type: 'line'},
        {name:'iter=300',data: T300,type: 'line'},{name:'iter=500',data: T500,type: 'line'},],
        legend: {data:['数值解','iter=100','iter=300','iter=500']},//图例文字
        toolbox: {feature: {saveAsImage: {}}},//设置保存图片按钮
    };

    tmprChart.setOption(tmprOption, true);
  }
  
  var tmpr=[],T100,T300,T500,xs=[],iters=[],errors=[],timeStep=1,iterations=800;
  var solution=new Solution(tmpr);
  var dx=1;//网格dx
  solution.SetUpGeometryAndMesh(dx);//网格
  solution.SetUpBoundaryCondition(dx);//设置边界条件
  solution.Solve(timeStep,iterations);//求解
  solution.ShowResults(errChart,myChart);//绘制流函数及速度云图
})
  
</script>

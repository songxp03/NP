<template>
  <el-row class="mb-4">
    <canvas ref="myCanvas" width="500" height="300" />
  </el-row>
</template>

<script lang="ts" setup>
import {onMounted,ref} from 'vue';

const myCanvas=ref();
  
onMounted(()=>{ 
  var myChart = echarts.init(myCanvas.value);
  var tmpr=[],T50s,T100s,T150s,xs=[],flowTime=0,timeStep=0.5,iterations=301;

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
      xs.push(i*spaceStep);
    }
  }

  function SetUpBoundaryCondition(dx){
    tmpr[0]=1;//左侧
    tmpr[50]=0;//右侧
  }

  function Solve(timeStep,iterations){
    var a=1;//热扩散系数
    for(var iter=1;iter<iterations+1;iter++){
      for(var i=1;i<50;i++){
        tmpr[i]=tmpr[i]+timeStep*a*(tmpr[i+1]+tmpr[i-1]-2*tmpr[i])/spaceStep/spaceStep;
      }
      if(iter==101) T50s=tmpr.clone();
      if(iter==201) T100s=tmpr.clone();
      if(iter==301) T150s=tmpr.clone();
    }
  }

  function ShowResults(){
    var option = {
        //title: {text: '温度分布'},//图标题
        tooltip: {trigger: 'axis'},//提示信息
        xAxis: {type: 'category',data: xs,},yAxis: {type: 'value'},//x坐标和y坐标
        series: [{name:'time=50s',data: T50s,type: 'line'},
            {name:'time=100s',data: T100s,type: 'line'},
            {name:'time=150s',data: T150s,type: 'line'},],
        legend: {data:['time=50s','time=100s','time=150s',]},//图例文字
        toolbox: {feature: {saveAsImage: {}}},//设置保存图片按钮
    };

    myChart.setOption(option);
  }
  
  var solution=new Solution(tmpr);
  var spaceStep=1;//网格dx
  solution.SetUpGeometryAndMesh(spaceStep);//网格
  solution.SetUpBoundaryCondition(spaceStep);//设置边界条件
  solution.Solve(timeStep,iterations);//求解
  solution.ShowResults();//绘制结果
  
})//end of onMounted
  
</script>

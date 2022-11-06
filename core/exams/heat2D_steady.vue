<template>
    <span>二维平板导热： {{`$\\frac{{\\partial ^2}T}{{\\partial}x^2}+\\frac{{\\partial ^2}T}{{\\partial}y^2}=0$`}} </span> <br/>
  <span>左侧边界条件： {{`$ T|_{x=0} = Ay(b - y)$`}} </span> <br/>
  <span>右侧边界条件： {{`$ T|_{x=a} = 0$`}} </span> <br/>
  <span>下侧边界条件： {{`$ T|_{y=0} = Bsin(\\pi x/a)$`}} </span> <br/>
  <span>上侧边界条件： {{`$ T|_{y=b} = 0$`}} </span> <br/>
  <el-row class="mb-4">
    <canvas ref="myCanvas" width="500" height="260" />
    <canvas ref="errCanvas" width="500" height="200" />
  </el-row>
</template>

<script lang="ts" setup>
import {getCurrentInstance,onMounted,ref} from 'vue';

const myCanvas=ref()
const errCanvas=ref()

onMounted(()=>{
  renderFormula()
  var myChart = echarts.init(myCanvas.value);
  
	var errChart = echarts.init(errCanvas.value);
  
  var tmpr = [], iters = [], errors = [],xs=[],ys=[],maxIter=100;

  Array.create = function (dimension, initialValue) {
  var arr = [];
  for (var i = 0; i < dimension; i++) {
    arr[i] = initialValue;
  }
  return arr;
};

var Solution = function (tmpr) {
  this.tmpr = tmpr;

  this.SetUpGeometryAndMesh = SetUpGeometryAndMesh;
  this.SetUpBoundaryCondition = SetUpBoundaryCondition;
  this.Solve = Solve;
};

function SetUpGeometryAndMesh(dx, dy) {
  for (var i = 0; i < 50 + 1; i++) {
    tmpr[i] = Array.create(30 + 1, 0);
  }
}

function SetUpBoundaryCondition(dx, dy,Tr) {
  for (var j = 0; j < 30 + 1; j++) {
    tmpr[0][j] = 0.05 * j * (30.0 - j);//左侧
    tmpr[50][j] = Tr;//右侧
  }

  for (var i = 0; i < 50 + 1; i++) {//
    tmpr[i][0] = 10 * Math.sin(Math.PI * i / 50.0)
    tmpr[i][30] = 0;//
  }
}

function Solve() {
  var err, iter = 0, tmp, N = 51 * 31/*节点总数*/;
  do {
    err = 0/*误差*/, maxIter = 1000/*最大迭代次数*/;
    for (var i = 1; i < 50; i++) {
      for (var j = 1; j < 30; j++) {
        tmp = 0.25 * (tmpr[i + 1][j] + tmpr[i - 1][j] + tmpr[i][j + 1] + tmpr[i][j - 1]);
        err += (tmp - tmpr[i][j]) * (tmp - tmpr[i][j]);
        tmpr[i][j] = tmp;
      }
    }
    iter++;//迭代次数
    err = Math.sqrt(err / N);
    errors.push(err); iters.push(iter)
  } while (err > 1E-4)
  console.info('最终误差：', err,iter);
}

function Calc2D(Tr,solveIt){
  var solution = new Solution(tmpr);
  var dx = 0.1; var dy = 0.1;//本特例网格dx与dy必须一致
  solution.SetUpGeometryAndMesh(dx, dy);//网格
  solution.SetUpBoundaryCondition(dx, dy,Tr);//设置边界条件
  if (solveIt) solution.Solve();//求解

  var results = [];
  [xs,ys]=[[],[]];
  for (var j = 0; j < 30 + 1; j++) {
    ys.push(j);
    for (var i = 0; i < 50 + 1; i++) {
      var arr = [i, j, tmpr[i][j]];
      results.push(arr);
      if(j==0) ys.push(i);
    }
  }
  return results;
}
  
  const data = Calc2D(0,true);

  const option = {
    title: {text: '温度云图',left:'left',textStyle:{ fontSize:12},top:40},//图标题
    tooltip: {},animation: false,
    xAxis: {type: 'category',xs},
    yAxis: {type: 'category',ys},
    grid: {containLabel: true,left:'15%',top:'5%',bottom:'5%'},
    visualMap: {
      min: 0,max: 10,calculable: true,orient: 'vertical',left: 'center',left: 0,top:80,
      inRange: { color: ['#0000FF', '#0065FF', '#00CBFF', '#00FFCB', '#00FF65', '#00FF00', '#65FF00', '#CBFF00', '#FFCB00', '#FF6500', '#FF0000'],}
    },
    series: [{name: '温度/°C',type: 'heatmap',data: data,
      itemStyle: {emphasis: {shadowBlur: 10,shadowColor: 'rgba(0, 0, 0, 0.5)'}}
    }]
  };

  myChart.setOption(option);
  
  //绘制残差
	var errOption = {
      title: {text: '残差变化趋势',left:'center',textStyle:{ fontSize:12},top:0},//图标题
    	grid: {containLabel: true,top:'5%'},
      tooltip: {trigger: 'axis'},//提示信息
      xAxis: {type: 'category',data: iters,},yAxis: {type: 'value'},//x坐标和y坐标
      series: [{name:'残差',data: errors,type: 'line'},],
      legend: {data:['残差',],left:'center',top:40},//图例文字
      toolbox: {feature: {saveAsImage: {}}}//设置保存图片按钮
    };
  errChart.setOption(errOption);
})
  
</script>

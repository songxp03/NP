<template>
  <el-row class="mb-4">
    <canvas ref="myCanvas" width="800" height="180" />
  </el-row>
</template>

<script lang="ts" setup>
  import { getCurrentInstance,onMounted,reactive, ref,toRaw } from 'vue'
  
  const myCanvas=ref()
  
  onMounted(()=>{
    var myChart = echarts.init(myCanvas.value)
		
    var xDim=15;
    var tmpr = [],xs=[],Ts=[];
    var dx = 1;//网格dx

    tmpr = Array(xDim + 1);
    for (var i = 0; i < xDim + 1; i++) {
      xs.push(i * dx);
      Ts[i]=0;
    }
    
    Ts[0]=100
    Ts[xDim]=0
    
    const data = xs.map((item,idx)=>[item,0,Ts[idx]])
    var option = {
      tooltip: {
        position: 'top'
      },
      grid: {
        height: '50%',
        top: '10%'
      },
      xAxis: {
        type: 'category',
        data: xs,
        splitArea: {
          show: true
        }
      },
      yAxis: {
        type: 'category',
        data: ['T'],
        splitArea: {
          show: true
        }
      },
      visualMap: {
        min: 0,
        max: 100,
        calculable: true,
        orient: 'horizontal',
        left: 'center',
        bottom: '15%'
      },
      series: [
        {
          name: 'Punch Card',
          type: 'heatmap',
          data: data,
          label: {
            show: true
          },
          emphasis: {
            itemStyle: {
              shadowBlur: 10,
              shadowColor: 'rgba(0, 0, 0, 0.5)'
            }
          }
        }
      ]
    };

    myChart.setOption(option)
  })
  
</script>

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
    var tmpr = [],xs=[];
    var dx = 1;//网格dx

    tmpr = Array(xDim + 1);
    for (var i = 0; i < xDim + 1; i++) {
      xs.push(i * dx);
      tmpr[i]=0;
    }
    
    tmpr[0]=100
    tmpr[xDim]=0
    
    for (var iter = 1; iter < 300; iter++) {
      for (var i = 1, tmp, err = 0; i < xDim; i++) {
        tmpr[i] = 0.5 * (tmpr[i + 1] + tmpr[i - 1]);
      }
    }
    
    const data = xs.map((item,idx)=>[item,0,tmpr[idx].toFixed(1)])
    
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

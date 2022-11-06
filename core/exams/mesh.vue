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

    var tmpr = [],xs=[],Ts=['index']
    var dx = 1;//网格dx

    tmpr = Array(15 + 1);
    for (var i = 0; i < 15 + 1; i++) {
      xs.push(i * dx);
    }
    
    const data = xs.map(item=>[item,0,item])
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
        data: Ts,
        splitArea: {
          show: true
        }
      },
      visualMap: {
        min: 0,
        max: 20,
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

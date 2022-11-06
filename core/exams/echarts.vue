<template>
  <el-row class="mb-4">
    <canvas ref="myCanvas" width="800" height="180" />
  </el-row>
  <el-row class="mb-4">
    <canvas ref="errCanvas" width="500" height="300" />
  </el-row>
</template>

<script lang="ts" setup>
  import { getCurrentInstance,onMounted,reactive, ref,toRaw } from 'vue'
  
  const myCanvas=ref()
  const errCanvas=ref()
  
  onMounted(()=>{
    var myChart = echarts.init(myCanvas.value)
		var errChart = echarts.init(errCanvas.value)
    
    var xDim=15,maxIter=1000;
    var tmpr = [],xs=[],errors=[],iters=[];
    var dx = 1;//网格dx

    tmpr = Array(xDim + 1);
    for (var i = 0; i < xDim + 1; i++) {
      xs.push(i * dx);
      tmpr[i]=0;
    }
    
    tmpr[0]=100
    tmpr[xDim]=0
    
    for (var iter = 1; iter < maxIter; iter++) {
      for (var i = 1, tmp, err = 0; i < xDim; i++) {
          tmp = 0.5 * (tmpr[i + 1] + tmpr[i - 1]);
          err += (tmp - tmpr[i]) * (tmp - tmpr[i]);
          tmpr[i] = tmp;
      }
      err = Math.sqrt(err / xDim);
      errors.push(err); iters.push(iter);
      if (err < 1E-4) {
        console.info("收敛于第", iter, "次迭代", '最终误差：', err)
        break;
      }
    }
    
    const data = xs.map((item,idx)=>[idx,0,tmpr[idx].toFixed(1)])
    
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

    myChart.setOption(option);
  
    //绘制残差
    var errOption = {
        title: {text: '残差变化趋势'},//图标题
        tooltip: {trigger: 'axis'},//提示信息
        xAxis: {type: 'category',data: iters,},yAxis: {type: 'value'},//x坐标和y坐标
        series: [{name:'残差',data: errors,type: 'line'},],
        legend: {data:['残差',]},//图例文字
        toolbox: {feature: {saveAsImage: {}}}//设置保存图片按钮
      };
    errChart.setOption(errOption);
  })
  
</script>

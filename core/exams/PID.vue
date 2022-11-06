<template>
  <el-row class="mb-4">
    <canvas ref="myCanvas" width="600" height="400" />
  </el-row>
  <el-row class="mb-4">
    <el-form :model="form" label-width="120px" style="width:600px">
      <el-form-item label="Kp">
        <el-slider v-model="pidOpt.Kp" show-input/>
      </el-form-item>
      <el-form-item label="Ki">
        <el-slider v-model="pidOpt.Ki" :max="1" :step="0.01" show-input/>
      </el-form-item>
      <el-form-item label="Kd">
        <el-slider v-model="pidOpt.Kd" show-input/>
      </el-form-item>
      <el-form-item label="设定温度">
        <el-slider v-model="pidOpt.setpoint" show-input/>
      </el-form-item>
      <el-form-item>
        <el-button type="primary">提交修改</el-button>
        <el-button>取消</el-button>
      </el-form-item>
    </el-form>
  </el-row>
</template>

<script lang="ts" setup>
  import { getCurrentInstance,onMounted,reactive, ref,toRaw } from 'vue'
  
  const myCanvas=ref()
  const pidOpt=reactive({
    Kp:30,Ki:0.01,Kd:80,setpoint:30
  });
  
  var chart;
  var data=[];
  var timmerID=null;
  var v = 0;

  var y=pidOpt.setpoint;
  var prevError = 0;
  var integral = 0;

  var windup = 20.0;

  var history = [[]];
  var dates=[];
  var lastError = 0;
  var lastLastY = y;
  var lastY = y;

  function pid() {
    var error = pidOpt.setpoint - lastLastY;
    integral += error;

    if (integral < -windup) {
      integral = -windup;
    } else if (integral > windup) {
      integral = windup;
    }

    var derivative = error - prevError;
    prevError = error;

    lastLastY = lastY;
    lastY = y;

    return 0.001 * (pidOpt.Kp * error + pidOpt.Ki * integral + pidOpt.Kd * derivative);
  }

  function update() {
    var date = new Date();
    var a = pid();
    var maxA = 3;
    a = Math.max(Math.min(a, maxA), -maxA);
    if (Math.abs(a) < 0.001) {
      a = 0;
    }

    v += a - v * 0.01;
    y += v;

    if (history.length > 100) {
      history.shift();
      dates.shift();
    }
    history.push([y, pidOpt.setpoint]);
    dates.push(date);

    for (var i = 1; i < history.length; i++) {
      data.push({ value: [dates[i], history[i][0]] });
    }

    if(chart) chart.setOption({ series: [{ data: data }] }),

    timmerID=setTimeout(update, 500);
  }

  function initChart() {
    chart = echarts.init(myCanvas.value);

    var option = {//图标属性配置
      title: { text: '实时温度' },//标题
      grid: {containLabel: true},
      xAxis: { type: 'time', splitLine: { show: true,lineStyle: {color: ['#aaa', '#ddd']}} },
      yAxis: { type: 'value', boundaryGap: [0, '100%'], splitLine: { show: true,lineStyle: {color: ['#aaa', '#ddd']} } },
      series: [{ name: '实时温度', type: 'line', showSymbol: false, hoverAnimation: false, data: data }]
    };

    chart.setOption(option);
    return chart;
  }
  
  onMounted(()=>{
		initChart();
    update()
  })
  
</script>

<template>
  <el-form
    ref="myFormRef"
    :model="myForm"
    status-icon
    label-width="120px"
    class="demo-myForm"
  >
    <el-form-item label="左侧温度" prop="Tleft">
      <el-input v-model.number="myForm.Tleft" />
    </el-form-item>
    <el-form-item label="右侧温度" prop="Tright">
      <el-input v-model.number="myForm.Tright" />
    </el-form-item>
    <el-form-item label="迭代次数" prop="iters">
      <el-input v-model.number="myForm.iters" />
    </el-form-item>
    <el-form-item>
      <el-button-group>
        <el-button type="primary" @click="onMesh(myFormRef)">剖分网格</el-button>
        <el-button type="primary" @click="onBC(myFormRef)">设置边界条件</el-button>
        <el-button type="primary" @click="submitForm(myFormRef)">迭代计算</el-button>
        <el-button type="primary" @click="onShow(myFormRef)">展示结果</el-button>
        <el-button @click="resetForm(myFormRef)">重置</el-button>
      </el-button-group>
    </el-form-item>
  </el-form>
  <el-row class="mb-4">
    <canvas ref="myCanvas" width="500" height="300" />
  </el-row>
</template>

<script lang="ts" setup>
  import { getCurrentInstance,onMounted,reactive, ref,toRaw } from 'vue'
  import type { FormInstance } from 'element-plus'


  var tmpr = [],xs = [], iters = [], errors = [], iterations = 800;
  var dx = 1;//网格dx
  var solution,myChart

  const myCanvas=ref()
  const myFormRef = ref<FormInstance>()
  const myForm = reactive({
    Tleft:1,Tright:0,
    iters: 100,
  })
  
  const submitForm = (formEl: FormInstance | undefined) => {
  	if (!formEl) return
    let {iters}=toRaw(myForm)

    solution.Solve(1, iters);//求解
  }

  const resetForm = (formEl: FormInstance | undefined) => {
    if (!formEl) return
    formEl.resetFields()
  }

  Array.create = function (dimension, initialValue) {
    var arr = [];
    for (var i = 0; i < dimension; i++) {
      arr[i] = initialValue;
    }
    return arr;
  }

  var Solution = function (tmpr) {
    this.tmpr = tmpr;

    this.SetUpGeometryAndMesh = SetUpGeometryAndMesh;
    this.SetUpBoundaryCondition = SetUpBoundaryCondition;
    this.Solve = Solve;
  }

  function SetUpGeometryAndMesh(dx) {
    tmpr = Array.create(50 + 1, 0);
    for (var i = 0; i < 50 + 1; i++) {
      xs.push(i * dx);
    }
  }

  function SetUpBoundaryCondition(Tl,Tr) {
    tmpr[0] = Tl;//左侧
    tmpr[50] = Tr;//右侧
  }

  function Solve(timeStep, maxIter) {
    for (var iter = 1, N = 51/*节点总数*/; iter < maxIter + 1; iter++) {
      for (var i = 1, tmp, err = 0; i < 50; i++) {
        tmp = 0.5 * (tmpr[i + 1] + tmpr[i - 1]);
        err += (tmp - tmpr[i]) * (tmp - tmpr[i]);
        tmpr[i] = tmp;
      }
      err = Math.sqrt(err / N);
      errors.push(err); iters.push(iter);
      if (err < 1E-4) {
        console.info("收敛于第", iter, "次迭代", '最终误差：', err)
        break;
      }
    }
  }

  const onMesh=()=>{
    solution.SetUpGeometryAndMesh(dx);//网格
  }

  const onBC=()=>{
    let {Tleft,Tright}=toRaw(myForm)
    solution.SetUpBoundaryCondition(Tleft,Tright);//设置边界条件
  }

  const onShow=()=>{
    let data=[]
    for (var i = 0; i < 50 + 1; i++) {
      data.push({ value: [i*dx,tmpr[i]] });
    }
    myChart.setOption({series:[{data}]});
  }

  onMounted(()=>{
    myChart = echarts.init(myCanvas.value, null, {
      renderer: 'canvas',
      useDirtyRect: false
    })

    var option = {
      title: {text: '1D稳态导热',left: 'left'},
      legend: {data: ['温度/°C',]},
      grid: {containLabel: true},
      xAxis: {type: 'category',},yAxis: {type: 'value'},
      series: [{name: '温度/°C',data:[],type: 'line'}]
    }

    myChart.setOption(option)

    solution = new Solution(tmpr);
  })
  
</script>

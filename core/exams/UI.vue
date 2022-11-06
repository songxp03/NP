<template>
  <el-row class="mb-4">
    
  </el-row>
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
      <el-button type="primary" @click="onShow(myFormRef)">展示参数</el-button>
    </el-form-item>
  </el-form>

</template>

<script lang="ts" setup>
  import { getCurrentInstance,onMounted,reactive, ref,toRaw } from 'vue'
  import { ElMessage, ElMessageBox} from 'element-plus'
  import type { Action,FormInstance } from 'element-plus'

  const { proxy } = getCurrentInstance()

  const myFormRef = ref<FormInstance>()
  const myForm = reactive({
    Tleft:1,
    Tright:0,
    iters: 100,
  })

  const onShow=()=>{
    let {Tleft,Tright,iters}=toRaw(myForm)
    console.log(Tleft,Tright,iters)
    ElMessageBox.alert(`迭代次数: ${myForm.iters}`,'消息')
  }

  onMounted(()=>{

  })
  
</script>

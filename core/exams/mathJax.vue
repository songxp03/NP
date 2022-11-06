<template>
  <span>二维平板导热： {{`$\\frac{{\\partial ^2}T}{{\\partial}x^2}+\\frac{{\\partial ^2}T}{{\\partial}y^2}=0$`}} </span> <br/>
  <span>左侧边界条件： {{`$ T|_{x=0} = Ay(b - y)$`}} </span> <br/>
  <span>右侧边界条件： {{`$ T|_{x=a} = 0$`}} </span> <br/>
  <span>下侧边界条件： {{`$ T|_{y=0} = Bsin(\\pi x/a)$`}} </span> <br/>
  <span>上侧边界条件： {{`$ T|_{y=b} = 0$`}} </span> <br/>
</template>

<script lang="ts" setup>
import {onMounted,ref} from 'vue';

onMounted(()=>{ 
  renderFormula()
})
  
</script>
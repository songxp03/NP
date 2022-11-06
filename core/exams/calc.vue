<template>
  <el-row class="mb-4">
    <el-table :data="tableData" stripe style="width: 100%">
      <el-table-column prop="name" label="变量名" width="180" />
      <el-table-column prop="v" label="值"/>
    </el-table>
  </el-row>
  <el-row class="mb-4">
    <el-button type="primary" @click="onShow()">也可以弹窗展示参数</el-button>
  </el-row>
</template>

<script setup>
  import { onMounted,reactive, ref,toRaw } from 'vue'
  import { ElMessageBox} from 'element-plus'
  //这个案例展示颗粒在流体中受拖曳力产生的加速度
  let list=[];
  const tableData = ref([]);
  
  const rhoP=1070,
        rhoF = 1.22,
        dp=2e-6,
        df=30E-6,
        mu=1.8e-5,
        U=5e-0,
        Uf=0,
        l = 120e-6, 
        h = 120e-6;
  
  var T = 300;
  var kB = 1.3806505e-23;
  var D = kB * T / (3 * Math.PI * mu * dp);
  
  let a=18*mu/dp/dp/rhoP*(U-Uf);
  //list.push({name:"a",v:a})
  
  var R = dp / df;
  list.push({name:"R",v:R})
  
  var Pe = U * df / D;
  list.push({name:"Pe",v:Pe})
  var Re = U * df * rhoF / mu;
  list.push({name:"Re",v:Re})
  var St = rhoP * dp * dp * U / (18 * mu * df);
  list.push({name:"St",v:St})
  var tau=18*mu/(rhoP*dp*dp);
  var Res=U * dp * rhoF / mu;
  list.push({name:"Re,sphere",v:Res})
  var Cd=24/Res*(1+Math.pow(Res,0.678)/6.0);
  var Cds=tau*Cd*Res/24;
  list.push({name:"Cd,sphere",v:Cds})
  
  var alpha = Math.PI * df * df / (4 * l * h);
  list.push({name:"alpha",v:alpha})
  var Ku = -0.5 * Math.log(alpha) - 0.75 + alpha - 0.25 * alpha * alpha;
  //list.push({name:"Ku",v:Ku})
  var effD = 2.9 * Math.pow(Ku, -1.0 / 3) * Math.pow(Pe, -2.0 / 3) + 0.62 / Pe;
  list.push({name:"effD.Stechkina",v:effD})
  var errD3=0.84* Math.pow(Pe, -0.43)
  list.push({name:"errD.Wang",v:errD3})
  var effR = (1 + R) / 2 / Ku * (2 * Math.log(1 + R) - 1 + alpha + 1 / (1 + R) / (1 + R) * (1 - alpha / 2) - alpha / 2 * (1 + R) * (1 + R));
  list.push({name:"effR.Liu",v:effR})
  var effR2=2.4*Math.pow(alpha,1/3.0)*Math.pow(R,1.75);
  list.push({name:"effR.Miecret",v:effR2})
  var Rp1=R+1;
  var effR3=(2*Rp1*Math.log(Rp1)-Rp1+1/Rp1)/2/(2-Math.log(Re))
  list.push({name:"effR.Langmuir",v:effR3})
  var effR4=0.6*(1-alpha)*R*R/Ku/(1+R);
  list.push({name:"effR.Lee",v:effR4})
  
  var J = (29.6 - 28 * Math.pow(alpha, 0.62)) * R * R - 27.5 * Math.pow(R, 2.8);
  var effI = St * J / 2 / Ku / Ku;
  list.push({name:"effI.Brown",v:effI});
  var effI2=Math.pow(2*St,3)/(Math.pow(2*St,3)+0.77*Math.pow(2*St,2)+0.22)
  list.push({name:"effI.Landhal",v:effI2})
  var effI3=Math.pow(1+(1.53-0.23*Math.log(Re)+0.0167*Math.pow(Math.log(Re),2))/2/St,-2);
  list.push({name:"effI.Suneja",v:effI3})
  list.push({name:"J",v:J})
  
  tableData.value=list;
  
  const onShow=()=>{
    ElMessageBox.alert(`alpha: ${alpha}`,'这是一个好消息哦')//弹窗
  }

  onMounted(()=>{
    //这里写一写初始化代码
  })
</script>
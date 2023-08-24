import{_ as z,i as D,j as C,r as N,o as P,a as s,k as I,w as o,l as L,m as U,n as q,q as R,v as T,x as j,y as F,z as G,d as H,b as e,u as f,g as y,A as M,B as O,C as Z,D as J}from"./index-45d8c746.js";const K={__name:"ble",setup(Q){D([L,U,q,R,T,j,F,G]);const g=C({title:{text:"HRV分布",left:"center"},xAxis:{},yAxis:{},tooltip:{trigger:"axis",axisPointer:{type:"cross",label:{backgroundColor:"#6a7985"}}},legend:{data:["个体","平均"],left:"left"},series:[{symbolSize:10,data:[[4.31,8.04],[8.07,6.95],[9.05,8.81],[10,6.33],[6.15,7.2],[3.03,4.23],[2.02,4.47],[1.05,3.33],[5.02,5.68]],type:"scatter",name:"个体"},{data:[{value:[1,5]},{value:[10,5]}],type:"line",name:"平均"}],toolbox:{feature:{dataZoom:{yAxisIndex:"none"},dataView:{readOnly:!1},magicType:{type:["line","bar"]},restore:{},saveAsImage:{}}}});var d=[];const b=N(""),k=window.speechSynthesis,l=new SpeechSynthesisUtterance;l.lang="zh-CN",l.volume=1,l.rate=1,l.pitch=1;const S=t=>{l.text=t,k.speak(l)},$=C({num:2}),A=async()=>{},x=()=>{d=[]},V=t=>{const a=t.target;console.log(`设备: ${a.name} 已经断开连接`)},B=async()=>{x();let t=await navigator.bluetooth.requestDevice({filters:[{name:"ESP32 test"}],optionalServices:["45c795f5-4da6-5007-9056-67eb78a98bd1"]});t.addEventListener("gattserverdisconnected",V);let a=await t.gatt.connect();console.log({device:t,server:a});let c=await a.getPrimaryServices(),u=c[0].uuid;console.log({services:c},u);let r=await a.getPrimaryService(u);console.log({service:r});let i=await r.getCharacteristics(),p=i[0].uuid,m=i[0].uuid;console.log({characteristics:i},p,m);let _=await r.getCharacteristic(p);console.log({characteristic:_}),_.startNotifications(),_.addEventListener("characteristicvaluechanged",h=>{var n=new Uint8Array(h.target.value.buffer,0);if(n.length==3){let w=(n[0]-48)*100+(n[1]-48)*10+n[2]-48;E(w),console.log("heart rate",w)}else console.warn("bad rate",n.length)});let v=await r.getCharacteristic(m);console.log({unCharacteristic:v})},E=t=>{let a=d.length;d.push([a,t]),g.series[0].data=d,g.series[1].data=[]};return P(()=>{console.log("App Fired!"),S("Ready Go!")}),(t,a)=>{const c=s("el-button"),u=s("el-button-group"),r=s("el-form-item"),i=s("el-form"),p=s("el-header"),m=s("el-main"),_=s("el-input"),v=s("el-footer"),h=s("el-container");return H(),I(h,{class:"wrapper"},{default:o(()=>[e(p,{class:"header"},{default:o(()=>[e(i,{ref:"form",inline:!0,model:$},{default:o(()=>[e(r,{label:"操作"},{default:o(()=>[e(u,{class:"ml-4"},{default:o(()=>[e(c,{type:"success",icon:f(M),onClick:B},{default:o(()=>[y(" 开始 ")]),_:1},8,["icon"]),e(c,{type:"primary",icon:f(O),onClick:A},{default:o(()=>[y(" 导出 ")]),_:1},8,["icon"]),e(c,{type:"danger",icon:f(Z),onClick:x},{default:o(()=>[y(" 清空数据 ")]),_:1},8,["icon"])]),_:1})]),_:1})]),_:1},8,["model"])]),_:1}),e(m,{class:"main"},{default:o(()=>[e(f(J),{option:g,autoresize:""},null,8,["option"])]),_:1}),e(v,{class:"footer"},{default:o(()=>[e(_,{modelValue:b.value,"onUpdate:modelValue":a[0]||(a[0]=n=>b.value=n),placeholder:"识别结果"},null,8,["modelValue"])]),_:1})]),_:1})}}},X=z(K,[["__scopeId","data-v-a9eb4784"]]);export{X as default};

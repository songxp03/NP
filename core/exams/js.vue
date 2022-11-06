<template>
  <el-row class="mb-4">
    <iframe id="mysite" src="https://songxp03.github.io/SimFun/"></iframe>
  </el-row>
</template>

<script setup>
/**
 * js语法与C/C++/java等C-Style类型语言语法相近，如基本数据类型可以使用关键字var声明，可以不赋值，不赋值时变量为undfeined类型：
 */
 var b=true;//定义布尔变量b，并赋值为true，js注释与C/C++完全相同，此处不详述
 var i=0;//定义整型变量i，并赋值为0
 var j=-100;//定义整型变量j，并赋值为-100
 var k=0.12345;//定义浮点数k，并赋值为0.12345
 var s ="Hello World";//定义字符串变量s，并幅值为"Hello World"
 var obj ={Name:"Song",Weight:80};//定义对象obj，设置其属性:Name为“Soong”，Weight为80
/**
 * js不需要像C语言一样显式的指定具体类型，如int，double等，统一使用var关键字声明变量。也可以使用关键字const或者let声明变量，const用于声明不变量且定义时赋值而且后期不允许修改变量值，let声明的变量只在当前代码块内有效，而且不支持重新定义。四则运算与C语言基本一致，如：
 */
 i -= 10;//等同于i=i-10
 i++;//等同于i=i+1
 j/= 10;//等同于j=j/10
 //console.log(abc);//输出undefined，abc变量已不在内存中
 //const foo;//错误必须定义时赋初值
 const bar=1;//定义常量，后期不允许修改
 //bar=2;//错误常量不允许修改
 var i=5.8;//允许重新定义变量，也可以改变类型
/**
 js中没有switch-case条件转移语法，但有if-else语法，使用方法与C/C++语言类似，例如判断一个整数是否为偶数：
*/
 var num =10;//定义整数
 if((num%2)==1){ //求num的余数，“%”为求余数运算符，”==”表示判断相等，与C语言一样
    console.log("奇数");// console.log()为主流浏览器内置的输出函数，类似于C语言中printf
 }else{
    console.log("偶数");
 }
/**
 * for循环同样与循环语句与C/C++语言类似，如求0到100之和：
 */
  var sum =0;
  for(var i =0;i<=100;i++){
    sum+=i;
  }
/**
 * js数组与C++标准库（STL）中vector类类似，用法如下：
 */
  var v= new Array(5) ;//定义一个长度为5的数组
  v[0]=1;v[3]=4; //下标从0开始，下标0表示第1个元素，下标3表示第4个元素
  var v2=[] ;//定义数组v2与v2= new Array();作用完全相同
  console.log(v.length)//输出数组v的长度
  var arr= new Array("one","two","three", 3 , 4 ,5); //js数组元素可以是如何类型
  arr.push(1.2 , 2.3 , 3.4);//使用push函数从尾部添加数据
  arr.pop();//从尾部删除一个数据
/**
 * js中函数的定义：如定义和调用求和函数（AddFun）：
 */
  function AddFun(a,b){
    var res;
    res = a + b;
    return res;
  }

  var sum=AddFun(1, 2);
/**
 * js内置了一些数学函数，如指数运算、开方、三角函数等，类似与C语言中math.h中的数学函数，js内置数学函数有：
 */
 var rnd=Math.random();//使用 random()返回 0到1之间的随机数
 var max=Math.max(2,5,8,1);//使用max()返回两个给定的数中的最大值
 var pi=Math.PI;//Math.PI表示圆周率
 var sqr=Math.sqrt(5);//求5的平方根
 var ii=Math.floor(3.4);//求不超过3.4的最大整数
 var e=Math.exp(2);//指数函数
 var s=Math.sin(Math.PI/6);//求30度角的正弦值

</script>

<style>
  #mysite{
    width: 100%;
    height: calc(100vh - 12px);
    frameborder:0;
  }
</style>
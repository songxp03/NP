<template></template>

<script lang="ts" setup>
import {onMounted,ref} from 'vue';
import * as THREE from 'three';
import { OrbitControls } from 'OrbitControls';
  
onMounted(()=>{ 
	const scene = new THREE.Scene();
	const camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 0.1, 1000 );

	const renderer = new THREE.WebGLRenderer();
	renderer.setSize( window.innerWidth, window.innerHeight );
  document.body.appendChild( renderer.domElement );

  var spotLight = new THREE.SpotLight(0xffffff);
	spotLight.position.set(-40, 60, -10);
	scene.add(spotLight);
  
	const boxGeo = new THREE.BoxGeometry( 1, 1, 1 );
	const greenMaterial = new THREE.MeshPhongMaterial( { color: 0x00ff00 } );
	const cube = new THREE.Mesh( boxGeo, greenMaterial );
	scene.add( cube );
	camera.position.z = 2;
  
  const sphereGeo = new THREE.SphereGeometry(0.3, 20, 20);
  var redMaterial = new THREE.MeshLambertMaterial({color: 0xff0000});
	var sphere = new THREE.Mesh(sphereGeo, redMaterial);
  scene.add( sphere );
  sphere.position.x=1
  
  const control=new OrbitControls(camera,renderer.domElement)

  function animate() {
    requestAnimationFrame( animate );

    cube.rotation.x += 0.01;
    cube.rotation.y += 0.01;

    renderer.render( scene, camera );
  };

	animate();
})
</script>
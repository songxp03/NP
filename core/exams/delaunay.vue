<template>
  <el-row class="mb-4">
    <canvas ref="myCanvas" width="600" height="400" @click="clickCanvas"/>
  </el-row>
</template>

<script lang="ts" setup>
  import { getCurrentInstance,onMounted,reactive, ref,toRaw } from 'vue'
  
  const EPSILON=0.000001;
  const myCanvas=ref()
  var ctx;
  
	const clickCanvas=()=>{
    alert(1)
  }
  
function XYZ(x, y, z) 
{
	this.x = x;
	this.y = y;
	this.z = z;	
}

function ITriangle(p1, p2, p3) 
{
	this.p1=p1;
	this.p2=p2;
	this.p3=p3;
}

function IEdge(p1, p2) 
{
	this.p1 = p1;
	this.p2 = p2;
}

function CircumCircle(xp,yp,x1,y1,x2,y2,x3,y3,circle) {
	var m1,m2,mx1,mx2,my1,my2;
	var dx,dy,rsqr,drsqr;
	var xc, yc, r;
	
	/* Check for coincident points */
	
	if ( Math.abs(y1-y2) < EPSILON && Math.abs(y2-y3) < EPSILON )
	{
		return 0;
	}
	
	if ( Math.abs(y2-y1) < EPSILON )
	{
		m2 = - (x3-x2) / (y3-y2);
		mx2 = (x2 + x3) / 2.0;
		my2 = (y2 + y3) / 2.0;
		xc = (x2 + x1) / 2.0;
		yc = m2 * (xc - mx2) + my2;
	}
	else if ( Math.abs(y3-y2) < EPSILON )
	{
		m1 = - (x2-x1) / (y2-y1);
		mx1 = (x1 + x2) / 2.0;
		my1 = (y1 + y2) / 2.0;
		xc = (x3 + x2) / 2.0;
		yc = m1 * (xc - mx1) + my1;	
	}
	else
	{
		m1 = - (x2-x1) / (y2-y1);
		m2 = - (x3-x2) / (y3-y2);
		mx1 = (x1 + x2) / 2.0;
		mx2 = (x2 + x3) / 2.0;
		my1 = (y1 + y2) / 2.0;
		my2 = (y2 + y3) / 2.0;
		xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
		yc = m1 * (xc - mx1) + my1;
	}
	
	dx = x2 - xc;
	dy = y2 - yc;
	rsqr = dx*dx + dy*dy;
	r = Math.sqrt(rsqr);
	
	dx = xp - xc;
	dy = yp - yc;
	drsqr = dx*dx + dy*dy;
	
	circle.x = xc;
	circle.y = yc;
	circle.z = r;
	
	return ( drsqr <= rsqr ? 1 : 0 );
}

var Delaunay= function () {};

Delaunay.triangulate=function (pxyz) {	
	var v=new Array();
	var nv = pxyz.length;
	for (i = 0; i < (nv * 3); i++) 
	{
		v[i]=new ITriangle();
	}
	
	// the points must be sorted on the x dimension for the rest to work
	pxyz.sort(function(a,b){return a.x-b.x;});
	var complete=[];
	var 	edges=[];
	var 	nedge 			= 0;
	var 	trimax, emax 	= 200;
	var 	status 			= 0;
	
	var	inside;
	var 	xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r;
	var 	xmin, xmax, ymin, ymax, xmid, ymid;
	var 	dx, dy, dmax;
	
	var		ntri = 0;
	
	/* Allocate memory for the completeness list, flag for each triangle */
	trimax = 4*nv;
	complete = new Array();
	for (var ic=0; ic<trimax; ic++) 
		complete[ic] = false;
	/* Allocate memory for the edge list */
	edges = new Array();
	for (var ie=0; ie<emax; ie++) 
		edges[ie] = new IEdge();
	/*
	Find the maximum and minimum vertex bounds.
	This is to allow calculation of the bounding triangle
	*/
	xmin = pxyz[0].x;
	ymin = pxyz[0].y;
	xmax = xmin;
	ymax = ymin;
	for (var i=1;i<nv;i++)
	{
		if (pxyz[i].x < xmin) xmin = pxyz[i].x;
		if (pxyz[i].x > xmax) xmax = pxyz[i].x;
		if (pxyz[i].y < ymin) ymin = pxyz[i].y;
		if (pxyz[i].y > ymax) ymax = pxyz[i].y;
	}
	dx = xmax - xmin;
	dy = ymax - ymin;
	dmax = (dx > dy) ? dx : dy;
	xmid = (xmax + xmin) / 2.0;
	ymid = (ymax + ymin) / 2.0;
	
	/*
		Set up the supertriangle
		This is a triangle which encompasses all the sample points.
		The supertriangle coordinates are added to the end of the
		vertex list. The supertriangle is the first triangle in
		the triangle list.
	*/
	pxyz[nv] = new XYZ();
	pxyz[nv+1] = new XYZ();
	pxyz[nv+2] = new XYZ();
	
	pxyz[nv+0].x = xmid - 2.0 * dmax;
	pxyz[nv+0].y = ymid - dmax;
	pxyz[nv+0].z = 0.0;
	pxyz[nv+1].x = xmid;
	pxyz[nv+1].y = ymid + 2.0 * dmax;
	pxyz[nv+1].z = 0.0;
	pxyz[nv+2].x = xmid + 2.0 * dmax;
	pxyz[nv+2].y = ymid - dmax;
	pxyz[nv+2].z = 0.0;
	
	v[0].p1 = nv;
	v[0].p2 = nv+1;
	v[0].p3 = nv + 2;
	complete[0] = false;
	ntri = 1;
	
	/*
		Include each point one at a time into the existing mesh
	*/
	for (i=0;i<nv;i++)
	 {	
		xp = pxyz[i].x;
		yp = pxyz[i].y;
		nedge = 0;
		
		/*
			Set up the edge buffer.
			If the point (xp,yp) lies inside the circumcircle then the
			three edges of that triangle are added to the edge buffer
			and that triangle is removed.
		*/
		var circle = new XYZ();
		for (var j=0;j<ntri;j++)
		{
			if (complete[j])
				continue;
			x1 = pxyz[v[j].p1].x;
			y1 = pxyz[v[j].p1].y;
			x2 = pxyz[v[j].p2].x;
			y2 = pxyz[v[j].p2].y;
			x3 = pxyz[v[j].p3].x;
			y3 = pxyz[v[j].p3].y;
			inside = CircumCircle( xp, yp,  x1, y1,  x2, y2,  x3, y3,  circle );
			xc = circle.x; 
			yc = circle.y; 
			r = circle.z;
			if (xc + r < xp) 
				complete[j] = true;
			if (inside)
			{
				/* Check that we haven't exceeded the edge list size */
				if (nedge+3 >= emax)
				{
					trace("crazy if statement");
					emax += 100;
					var edges_n = new Array();
					for (ie = 0; ie < emax; ie++) 
						edges_n[ie] = new IEdge();
					for (var zfj = 0; zfj < edges.length; zfj++) 
					{
						edges_n[zfj] = edges[zfj];
					}
					edges = edges_n;
				}
				edges[nedge+0].p1 = v[j].p1;
				edges[nedge+0].p2 = v[j].p2;
				edges[nedge+1].p1 = v[j].p2;
				edges[nedge+1].p2 = v[j].p3;
				edges[nedge+2].p1 = v[j].p3;
				edges[nedge+2].p2 = v[j].p1;
				nedge += 3;
				v[j].p1 = v[ntri-1].p1;
				v[j].p2 = v[ntri-1].p2;
				v[j].p3 = v[ntri-1].p3;
				complete[j] = complete[ntri-1];
				ntri--;
				j--;
			}
		}
		
		/*
			Tag multiple edges
			Note: if all triangles are specified anticlockwise then all
			interior edges are opposite pointing in direction.
		*/
		for (j=0;j<nedge-1;j++)
		{
				for (var k=j+1;k<nedge;k++)
				{
					if ((edges[j].p1 == edges[k].p2) && (edges[j].p2 == edges[k].p1))
					{
						edges[j].p1 = -1;
						edges[j].p2 = -1;
						edges[k].p1 = -1;
						edges[k].p2 = -1;
					}
					/* Shouldn't need the following, see note above */
					if ((edges[j].p1 == edges[k].p1) && (edges[j].p2 == edges[k].p2))
					{
						edges[j].p1 = -1;
						edges[j].p2 = -1;
						edges[k].p1 = -1;
						edges[k].p2 = -1;
					}
				}
		}
		
		/*
			Form new triangles for the current point
			Skipping over any tagged edges.
			All edges are arranged in clockwise order.
		*/
		for (j=0;j<nedge;j++)
		{
			if (edges[j].p1 == -1 || edges[j].p2 == -1)
				continue;
			if (ntri >= trimax) 
				return null;
			v[ntri].p1 = edges[j].p1;
			v[ntri].p2 = edges[j].p2;
			v[ntri].p3 = i;
			complete[ntri] = false;
			ntri++;
		}
	}
	
	/*
		Remove triangles with supertriangle vertices
		These are triangles which have a vertex number greater than nv
	*/
	for (i=0;i<ntri;i++)
	{
		if (v[i].p1 >= nv || v[i].p2 >= nv || v[i].p3 >= nv)
		{
			v[i] = v[ntri-1];
			ntri--;
			i--;
		}
	}
	
	v.length = ntri;
	pxyz.length -= 3;
	
	return v;
};
  
  onMounted(()=>{
    var theCanvas=myCanvas.value
    var context = theCanvas.getContext("2d");
		
    	//background
	context.fillStyle = "#ffffaa";
	context.fillRect(0, 0, 600, 400);		

	var points=new Array();

	for(var p=1;p<30;p++){
		var pnt=new XYZ(Math.random()*600,Math.random()*400,0);
		points.push(pnt);
	}

	var triangles = Delaunay.triangulate(points);

	triangles.forEach(function drawTriangle(tri){
		context.strokeStyle = "#FF0000";
		context.lineWidth=1;

		context.beginPath();
		context.moveTo(points[tri.p1].x, points[tri.p1].y);
		context.lineTo(points[tri.p2].x, points[tri.p2].y);
		context.lineTo(points[tri.p3].x, points[tri.p3].y);
		context.lineTo(points[tri.p1].x, points[tri.p1].y);
		context.stroke();
		context.closePath();
	});
    
  })
  
</script>

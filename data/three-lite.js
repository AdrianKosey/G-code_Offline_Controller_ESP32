// Minimal local WebGL module for the CNC viewer. It intentionally exposes only
// the Three-like pieces this page needs: two dynamic LineSegments buffers plus
// one static grid/axis buffer. No network dependency and no render loop.
export class CNCThreeLite {
  constructor(canvas) {
    this.canvas = canvas;
    const gl = canvas.getContext('webgl', { antialias: true, preserveDrawingBuffer: false });
    if (!gl) throw new Error('WebGL no está disponible en este navegador');
    this.gl = gl;
    const vs = `attribute vec3 p;uniform mat4 m;uniform vec4 c;varying vec4 v;void main(){gl_Position=m*vec4(p,1.);v=c;}`;
    const fs = `precision mediump float;varying vec4 v;void main(){gl_FragColor=v;}`;
    const compile = (type, source) => { const s=gl.createShader(type); gl.shaderSource(s,source); gl.compileShader(s); if(!gl.getShaderParameter(s,gl.COMPILE_STATUS)) throw new Error(gl.getShaderInfoLog(s)); return s; };
    this.program = gl.createProgram(); gl.attachShader(this.program,compile(gl.VERTEX_SHADER,vs)); gl.attachShader(this.program,compile(gl.FRAGMENT_SHADER,fs)); gl.linkProgram(this.program);
    if (!gl.getProgramParameter(this.program,gl.LINK_STATUS)) throw new Error(gl.getProgramInfoLog(this.program));
    this.p = gl.getAttribLocation(this.program,'p'); this.m = gl.getUniformLocation(this.program,'m'); this.c = gl.getUniformLocation(this.program,'c');
    this.grid = gl.createBuffer(); this.rapid = gl.createBuffer(); this.cut = gl.createBuffer();
    this.gridCount=this.rapidCount=this.cutCount=0;
    this.target=[0,0,0]; this.yaw=-0.75; this.pitch=0.72; this.radius=120; this.fov=Math.PI/4;
    gl.enable(gl.DEPTH_TEST); gl.enable(gl.BLEND); gl.blendFunc(gl.SRC_ALPHA,gl.ONE_MINUS_SRC_ALPHA);
    this.resize(); new ResizeObserver(()=>this.resize()).observe(canvas);
  }
  resize(){ const d=Math.min(devicePixelRatio||1,2), w=Math.max(1,this.canvas.clientWidth*d), h=Math.max(1,this.canvas.clientHeight*d); if(this.canvas.width!==w||this.canvas.height!==h){this.canvas.width=w;this.canvas.height=h;} this.render(); }
  upload(buffer,data){ const gl=this.gl; gl.bindBuffer(gl.ARRAY_BUFFER,buffer); gl.bufferData(gl.ARRAY_BUFFER,data,gl.STATIC_DRAW); return data.length/3; }
  setPaths(cut,rapid){ this.cutCount=this.upload(this.cut,cut); this.rapidCount=this.upload(this.rapid,rapid); this.render(); }
  frame(bounds, iso=true){ const dx=Math.max(1,bounds.maxX-bounds.minX),dy=Math.max(1,bounds.maxY-bounds.minY),dz=Math.max(1,bounds.maxZ-bounds.minZ); this.target=[(bounds.minX+bounds.maxX)/2,(bounds.minY+bounds.maxY)/2,(bounds.minZ+bounds.maxZ)/2]; this.radius=Math.max(dx,dy,dz)*2.05; if(iso){this.yaw=-0.75;this.pitch=0.72;} this.makeGrid(bounds); this.render(); }
  top(){this.yaw=0;this.pitch=1.555;this.render();}
  iso(){this.yaw=-0.75;this.pitch=0.72;this.render();}
  rotate(dx,dy){this.yaw+=dx*.008;this.pitch=Math.max(-1.52,Math.min(1.52,this.pitch+dy*.008));this.render();}
  zoom(factor){this.radius=Math.max(.5,Math.min(1e7,this.radius*factor));this.render();}
  pan(dx,dy){const s=this.radius*Math.tan(this.fov*.5)*2/Math.max(1,this.canvas.clientHeight); const r=[-Math.sin(this.yaw),Math.cos(this.yaw),0]; const u=[-Math.cos(this.yaw)*Math.sin(this.pitch),-Math.sin(this.yaw)*Math.sin(this.pitch),Math.cos(this.pitch)]; this.target[0]+=(-dx*r[0]+dy*u[0])*s;this.target[1]+=(-dx*r[1]+dy*u[1])*s;this.target[2]+=(-dx*r[2]+dy*u[2])*s;this.render();}
  makeGrid(b){ const minX=Math.floor(b.minX/10)*10,maxX=Math.ceil(b.maxX/10)*10,minY=Math.floor(b.minY/10)*10,maxY=Math.ceil(b.maxY/10)*10,z=Math.min(0,b.minZ); const a=[]; const span=Math.max(maxX-minX,maxY-minY); const step=Math.max(10,Math.ceil(span/400)*10); for(let x=minX;x<=maxX;x+=step)a.push(x,minY,z,x,maxY,z);for(let y=minY;y<=maxY;y+=step)a.push(minX,y,z,maxX,y,z); const l=Math.max(20,span*.16);a.push(0,0,z,l,0,z,0,0,z,0,l,z,0,0,z,0,0,z+l);this.gridCount=this.upload(this.grid,new Float32Array(a)); }
  mat(){const e=[this.target[0]+this.radius*Math.cos(this.pitch)*Math.cos(this.yaw),this.target[1]+this.radius*Math.cos(this.pitch)*Math.sin(this.yaw),this.target[2]+this.radius*Math.sin(this.pitch)],c=this.target; let z=[e[0]-c[0],e[1]-c[1],e[2]-c[2]],zl=Math.hypot(...z);z=z.map(v=>v/zl);let x=[-z[1],z[0],0],xl=Math.hypot(...x);if(xl<1e-4){x=[1,0,0];xl=1;}x=x.map(v=>v/xl);const y=[z[1]*x[2]-z[2]*x[1],z[2]*x[0]-z[0]*x[2],z[0]*x[1]-z[1]*x[0]],v=[x[0],y[0],z[0],0,x[1],y[1],z[1],0,x[2],y[2],z[2],0,-(x[0]*e[0]+x[1]*e[1]+x[2]*e[2]),-(y[0]*e[0]+y[1]*e[1]+y[2]*e[2]),-(z[0]*e[0]+z[1]*e[1]+z[2]*e[2]),1];const f=1/Math.tan(this.fov/2),a=this.canvas.width/this.canvas.height,n=.05,fa=1e7,p=[f/a,0,0,0,0,f,0,0,0,0,(fa+n)/(n-fa),-1,0,0,(2*fa*n)/(n-fa),0],o=new Float32Array(16);for(let col=0;col<4;col++)for(let row=0;row<4;row++)o[col*4+row]=p[row]*v[col*4]+p[4+row]*v[col*4+1]+p[8+row]*v[col*4+2]+p[12+row]*v[col*4+3];return o;}
  draw(buffer,count,color){if(!count)return;const g=this.gl;g.bindBuffer(g.ARRAY_BUFFER,buffer);g.vertexAttribPointer(this.p,3,g.FLOAT,false,0,0);g.enableVertexAttribArray(this.p);g.uniform4fv(this.c,color);g.drawArrays(g.LINES,0,count);}
  render(){if(!this.gl)return;const g=this.gl;g.viewport(0,0,this.canvas.width,this.canvas.height);g.clearColor(.025,.035,.055,1);g.clear(g.COLOR_BUFFER_BIT|g.DEPTH_BUFFER_BIT);g.useProgram(this.program);g.uniformMatrix4fv(this.m,false,this.mat());this.draw(this.grid,this.gridCount,[.27,.33,.43,.8]);this.draw(this.rapid,this.rapidCount,[.55,.59,.65,.48]);this.draw(this.cut,this.cutCount,[.25,.85,1,1]);}
}

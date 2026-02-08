# GLSL Shader Behaviors

```glsl
// gl_FragCoord - vec4, set to x, y, 0, 0
//vec2 v2 = gl_FragCoord; <- fail
vec4 v4xyzw = FC.rgba;
vec3 v3rgb = FC.rgb;
vec2 v2rg = FC.rg;
vec4 v4 = FC;
if(v4xyzw.r > 1000.)
  o = vec4(1,1,1,1);
//if(v4xyzw.g > 500.)
//  o = vec4(1,0,0,0);
//if(v4xyzw.b > 10.)
//  o = vec4(0,1,0,0);
// if(v4xyzw.w > 10.)
//  o = vec4(0,0,1,0);
```

```glsl
// r - vec2, set to resx, resy
//vec4 v4xyzw = r.rgba; <-fail
//vec3 v3rgb = r.rgb; <- fail
vec2 v2xy = r.xy;
vec2 v2 = r;
//vec4 v4 = r; <- fail
vec4 v4; v4.xy = r;

//if(v2xy.r > 1000.)
//  o = vec4(1,1,1,1);

//if(v2xy.g > 500.)
//  o = vec4(1,0,0,0);

//if(v2xy.b > 10.) <- fail
//  o = vec4(0,1,0,0);

//if(v2xy.a > 10.) <- fail
//  o = vec4(0,0,1,0);
```

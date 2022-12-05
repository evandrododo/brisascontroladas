//Specify compiler to use GLSL version 1.2
//Enable working with textures of any dimensions
//Declare texture texture0, which is linked when you use fbo.bind(), or any other texture.bind().

//#version 430 compatibility
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect texture0;     //it can be named in any way, GLSL just links it


#define N (256)
uniform float specArray[N];

//uniform float time;             //Uniform value "time"

//blurring
void main(){
  vec2 pos = gl_TexCoord[0].st;     

  // Imagem da camera
  vec4 color0 = texture2DRect(texture0, pos);

  vec4 color;

  color = color0;  
  color.a = 1 - (color.r + color.g + color.b) / 3;

  if(color.a > 0)
    color.a = color.a*2;

  if(color.a > 1)
    color.a = 1;


  gl_FragColor = color;
}
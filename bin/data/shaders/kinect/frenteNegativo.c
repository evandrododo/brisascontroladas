//Specify compiler to use GLSL version 1.2
//Enable working with textures of any dimensions
//Declare texture texture0, which is linked when you use fbo.bind(), or any other texture.bind().

//#version 430 compatibility
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect texture0;     //it can be named in any way, GLSL just links it

uniform sampler2DRect texture1;   //second image

#define N (256)
uniform float specArray[N];

//uniform float time;             //Uniform value "time"

//blurring
void main(){
  vec2 pos = gl_TexCoord[0].st;     
  
  // Imagem do video
  vec4 color0 = texture2DRect(texture0, pos);
  // Imagem do sensor de profundidade
  vec4 corDepth =  texture2DRect(texture1, pos);


  vec4 color;

  //aumenta contraste de profundidade
  corDepth.b = corDepth.b * 2;
  if(corDepth.b > 1)
    corDepth.b = 1;

  color = color0;  
  if(corDepth.b > 0.5) {
    color.r = corDepth.b - color.r;
    color.g = corDepth.b - color.g;
    color.b = corDepth.b - color.b;
  }
  

  gl_FragColor = color;
}
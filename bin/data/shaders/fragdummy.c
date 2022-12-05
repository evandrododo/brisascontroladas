//Specify compiler to use GLSL version 1.2
//Enable working with textures of any dimensions
//Declare texture texture0, which is linked when you use fbo.bind(), or any other texture.bind().

//#version 430 compatibility
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect texture0;     //it can be named in any way, GLSL just links it



//blurring
void main(){
  vec2 pos = gl_TexCoord[0].st;     
  
  // Imagem do video
  vec4 color0 = texture2DRect(texture0, pos);

  gl_FragColor = color0;
}
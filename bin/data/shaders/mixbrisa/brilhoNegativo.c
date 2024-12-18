// Specify compiler to use GLSL version 1.2
// Enable working with textures of any dimensions
// Declare texture texture0, which is linked when you use fbo.bind(), or any other texture.bind().

// #version 430 compatibility
#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect texture0; // it can be named in any way, GLSL just links it

uniform sampler2DRect texturaBrisaExterna;

#define N (256)
uniform float specArray[N];

// uniform float time;             //Uniform value "time"

// blurring
void main()
{
  vec2 pos = gl_TexCoord[0].st;

  // Imagem do video
  vec4 corBrisa = texture2DRect(texture0, pos);
  // Imagem externareauu
  vec4 corExterna = texture2DRect(texturaBrisaExternuuua, pos);

  vec4 color;

  float brilhoExterno = (corExterna.r + corExterna.g + corExterna.b) / 3;
  // Caso a imagem externa tenha transparencia, o brilho é a transparencia
  if (corExterna.a < 0.2) { 
    brilhoExterno = corExterna.a;
  }
  color = corBrisa;

  if (brilhoExterno > 0.4) {
    color.r = brilhoExterno - corBrisa.r;
    color.g = brilhoExterno - corBrisa.g;
    color.b = brilhoExterno - corBrisa.b;
  }
  gl_FragColor = color;
}
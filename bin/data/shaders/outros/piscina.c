#ifdef GL_ES
precision mediump float;
#endif

#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable
#extension GL_OES_standard_derivatives : enable

uniform sampler2DRect texture0;     //it can be named in any way, GLSL just links it
uniform float p1;
uniform float p2;
uniform float p3;
uniform vec2 mouse;
uniform vec2 resolution;

vec2 random(vec2 st) {
	st = vec2(dot(st, vec2(34.356, 21.53)), dot(st, vec2(-12.534, 74.342)));
	
	return -1.0 + 2.0 * fract(sin(st) * 53256.4932);
}

vec3 random3(vec3 st) {
	st = vec3(dot(st, vec3(23.534, -12.535, -22.113)), dot(st, vec3(9.124, 32.131, 17.854)), dot(st, vec3(-3.432, 35.21, 43.315)));

	return -1.0 + 2.0 * fract(sin(st) * 43245.548);
}


float noise(vec2 st) {
	vec2 i = floor(st);
	vec2 f = fract(st);
	
	vec2 v1 = random(i);
	vec2 v2 = random(i + vec2(1.0, 0.0));
	vec2 v3 = random(i + vec2(0.0, 1.0));
	vec2 v4 = random(i + vec2(1.0, 1.0));
	
	//vec2 u = f * f * (3.0 - 2.0 * f);
	
	return mix(mix(dot(v1, vec2(0.0, 0.0) - f), dot(v2, vec2(1.0, 0.0) - f), smoothstep(0.0, 1.0, f.x)),
		  mix(dot(v3, vec2(0.0, 1.0) - f), dot(v4, vec2(1.0, 1.0) - f), smoothstep(0.0, 1.0, f.x)), smoothstep(0.0, 1.0, f.y));
}

float noise3(vec3 st) {
	vec3 i = floor(st);
	vec3 f = fract(st);
	vec3 u = f * f * (3.0 - 2.0 * f);
	
	return mix(
		mix(
			mix(
				dot(random3(i + vec3(0.0, 0.0, 0.0)), vec3(0.0, 0.0, 0.0) - f),
				dot(random3(i + vec3(1.0, 0.0, 0.0)), vec3(1.0, 0.0, 0.0) - f),
				u.x
			),
			mix(
				dot(random3(i + vec3(0.0, 1.0, 0.0)), vec3(0.0, 1.0, 0.0) - f),
				dot(random3(i + vec3(1.0, 1.0, 0.0)), vec3(1.0, 1.0, 0.0) - f),
				u.x
			),
			u.y
		),
		mix(
			mix(
				dot(random3(i + vec3(0.0, 0.0, 1.0)), vec3(0.0, 0.0, 1.0) - f),
				dot(random3(i + vec3(1.0, 0.0, 1.0)), vec3(1.0, 0.0, 1.0) - f),
				u.x
			),
			mix(
				dot(random3(i + vec3(0.0, 1.0, 1.0)), vec3(0.0, 1.0, 1.0) - f),
				dot(random3(i + vec3(1.0, 1.0, 1.0)), vec3(1.0, 1.0, 1.0) - f),
				u.x
			),
			u.y
		),
		u.z
	);
}

float fbm(vec3 st) {

	float v = 0.0;
	const int octave = 5;
	float amp = 0.5;
	for(int i = 0; i < octave; i++) {
		v += amp * noise3(st);
		st *= 2.0;
		amp *= .5;
	}
	
	return v;
	
}

float turbulence(vec3 st) {

	float v = 0.0;
	const int octave = 5;
	float amp = 0.5;
	for(int i = 0; i < octave; i++) {
		v += amp * abs(noise3(st));
		st *= 2.0;
		amp *= .5;
	}
	
	return v;
}

void main( void ) {
    vec2 pos = gl_TexCoord[0].st;     

    // Imagem do video
    vec4 color_original = texture2DRect(texture0, pos );
    float posfinalx = pos.x+sin(pos.x*(p1*0.01))*cos(pos.y*p2*0.01)*p3;
    float posfinaly = pos.y+cos(pos.x*p1*0.01)*sin(pos.y*p2*0.01)*p3;

    posfinalx = pos.x+sin(posfinaly*(p1*0.01))*cos(posfinalx*p2*0.01)*p3;
    posfinaly = pos.y+cos(posfinaly*0.01)*sin(posfinalx*p2*0.01)*p3;
    vec4 color0 = texture2DRect(texture0, vec2(posfinalx, posfinaly ) );

    vec2 p = gl_FragCoord.xy / resolution.yy;

    float v = .5 + noise(p * 8.0 + p1) * .5;


    vec3 c = mix(vec3(0.9, 0.95, 0.85), vec3(0.05, 0.5, 0.9), v);
	float color = 1.;


    gl_FragColor = color0 ;
}

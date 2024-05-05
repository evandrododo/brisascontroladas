#version 120

uniform sampler2DRect texture0; // Textura de entrada
uniform vec2 resolution; // Passar o tamanho da textura como uniforme

vec2 clampCoord(vec2 coord) {
    return clamp(coord, vec2(0.0), resolution);
}

void main() {
    vec2 pos = gl_TexCoord[0].st;

    float step = 3.0;
    vec2 texOffset = step / resolution; // Tamanho do passo em relação ao tamanho da textura
    
    vec3 texColor = texture2DRect(texture0, pos).rgb;
    vec3 o = texture2DRect(texture0, clampCoord(pos + vec2(0,0))).rgb;
    vec3 n = texture2DRect(texture0, clampCoord(pos + vec2(0,step))).rgb;
    vec3 s = texture2DRect(texture0, clampCoord(pos + vec2(0,-step))).rgb;
    vec3 e = texture2DRect(texture0, clampCoord(pos + vec2(step,0))).rgb;
    vec3 w = texture2DRect(texture0, clampCoord(pos + vec2(-step,0))).rgb;


    vec3 dy = (n - s);
    vec3 dx = (e - w);

    vec3 edge = sqrt(dx*dx + dy*dy);
    vec3 angle = atan(dy, dx);
   
    vec3 laplacian = n + e + s + w - 4.0 * o;
    vec3 fragColor = abs(laplacian.xyz/o.xyz) * 1.0;
    gl_FragColor = vec4(fragColor, 1.0);
}

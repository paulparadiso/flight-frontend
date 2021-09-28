#version 150
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;
uniform float u_mosaic;
uniform float u_time_x_mult;
uniform float u_noise_x_mult;
uniform float u_noise_y_mult;
uniform float u_noise_threshold;
uniform float u_overcast;

out vec4 FragColor;

highp float rand(vec2 co)
{
	highp float a = 12.9898;
	highp float b = 78.233;
	highp float c = 43758.5453;
	highp float dt= dot(co.xy ,vec2(a,b));
	highp float sn= mod(dt,3.14);
	return fract(sin(sn) * c);
}

vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }

float snoise(vec2 v) {
	const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
						0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
						-0.577350269189626,  // -1.0 + 2.0 * C.x
						0.024390243902439); // 1.0 / 41.0
	// First corner
	vec2 i  = floor(v + dot(v, C.yy) );
	vec2 x0 = v -   i + dot(i, C.xx);

	// Other corners
	vec2 i1;
	i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
	vec4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;

	// Permutations
	i = mod289(i); // Avoid truncation effects in permutation
	vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
					  + i.x + vec3(0.0, i1.x, 1.0 ));

	vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
	m = m*m ;
	m = m*m ;

	// Gradients: 41 points uniformly over a line, mapped onto a diamond.
	// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

	vec3 x = 2.0 * fract(p * C.www) - 1.0;
	vec3 h = abs(x) - 0.5;
	vec3 ox = floor(x + 0.5);
	vec3 a0 = x - ox;

	// Normalise gradients implicitly by scaling m
	// Approximation of: m *= inversesqrt( a0*a0 + h*h );
	m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

	// Compute final noise value at P
	vec3 g;
	g.x  = a0.x  * x0.x  + h.x  * x0.y;
	g.yz = a0.yz * x12.xz + h.yz * x12.yw;
	return 130.0 * dot(m, g);
}	

mat2 rotate2d(float _angle){
	return mat2(cos(_angle),-sin(_angle),
				sin(_angle),cos(_angle));
}

void main(){
	vec2 st = gl_FragCoord.xy/u_resolution.xy;
	float st_x_next = (gl_FragCoord.x + 1.0)/u_resolution.x;
	float st_y_next = (gl_FragCoord.y + 1.0)/u_resolution.y;

	float time = u_time * u_time_x_mult;

	float x = st.x * u_noise_x_mult + time;
	float x_next = st_x_next * u_noise_x_mult + time;
	float y = (st.y - mod(st.y, u_mosaic / 1000.0)) * u_noise_y_mult;
	float y_next = (st_y_next - mod(st_y_next, u_mosaic / 1000.0)) * u_noise_y_mult;
	
	float noise = snoise(vec2(x, y));
	float hasCloud = step(1.0 - noise, u_noise_threshold);

	float noise_next_x = snoise(vec2(x_next, y));
	float hasCloud_next_x = step(1.0 - noise_next_x, u_noise_threshold);

	float noise_next_y = snoise(vec2(x, y_next));
	float hasCloud_next_y = step(1.0 - noise_next_y, u_noise_threshold);

	float cloudColor = 1.0;
	float opacity = 0.5;

	if (hasCloud_next_x != hasCloud || hasCloud_next_y != hasCloud) {
		cloudColor = 0.0;
		opacity = 1.0;
		hasCloud = 1.0;
	}
	
	if (u_overcast == 1.0) {
		hasCloud = 1.0;
		cloudColor = 1.0;
		opacity = 0.5;
	}
	
	FragColor = vec4(cloudColor, cloudColor, cloudColor, hasCloud * opacity);
}

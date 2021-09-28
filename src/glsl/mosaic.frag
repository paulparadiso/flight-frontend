uniform sampler2D tDiffuse;
uniform float size;

varying vec2 vUv;

void main() {
	vec2 uv = floor(vUv * size)/size;
	gl_FragColor = texture2D(tDiffuse, uv);
}

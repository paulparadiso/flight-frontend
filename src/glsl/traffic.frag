#version 150        

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;
uniform int u_num_cars;
uniform float u_car_speed;

float max_dist = 200.0;
int num_cars = 10;

out vec4 outputColor;

void main(){
    vec4 collectedColor = vec4(0.0,0.0,0.0,0.0);
    if(gl_FragCoord.y < 0){
        outputColor = collectedColor;
    } else {
        for(int i = 0; i < num_cars; i++){
            float mult = sin(u_time * (0.7 / i));   
            float lightX = u_resolution.x * mult;
            vec2 lightPos = vec2(lightX , -u_resolution.y / 2);
            vec2 redLightPos = lightPos;
    
            redLightPos.x -= 20.0;
    
            float d = abs(gl_FragCoord.x - lightPos.x);
            float dy = 0.0;
            float dr = abs(gl_FragCoord.x - redLightPos.x);

            if(d < max_dist){
                if(gl_FragCoord.y < (u_resolution.y / 4)){
                    dy = 1.0 - gl_FragCoord.y / (u_resolution.y / 4);
                }
                collectedColor += vec4(1.0, 241.0 / 255.0, 224.0 / 255.0,((0.5 - 0.75 * (d / max_dist)) * dy));
            } 
            if(dr < max_dist) {
                if(gl_FragCoord.y < (u_resolution.y / 4)){
                    dy = 1.0 - gl_FragCoord.y / (u_resolution.y / 4);
                }
                collectedColor += vec4(0.55, 0.0, 0.0,((0.5 - 0.75 * (dr / max_dist)) * dy));
                collectedColor /= 2;
            } 
        }
        outputColor = collectedColor;
    }
}

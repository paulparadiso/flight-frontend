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
            float mult = sin(u_time * (0.3 / i));   
            float lightX = u_resolution.x * mult;
            vec2 lightPos = vec2(lightX , -u_resolution.y / 2);
            vec2 redLightPos = lightPos;
    
            redLightPos.x -= 20.0;

            if(gl_FragCoord.x > lightPos.x && gl_FragCoord.x < lightPos.x + 1.0){
                collectedColor += vec4(1.0, 241.0 / 255.0, 224.0 / 255.0, 1.0);
            } 
            if(gl_FragCoord.x > redLightPos.x && gl_FragCoord.x < redLightPos.x + 1.0) {
                collectedColor += vec4(0.55, 0.0, 0.0, 1.0);
                collectedColor /= 2;
            } 
        }
        outputColor = collectedColor;
    }
}

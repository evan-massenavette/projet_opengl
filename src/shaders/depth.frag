#version 330 core

in vec3 gWorldPos;

uniform vec3 lightWorldPos;
uniform float farPlane;

void main() {
    // Distance between fragment and light source
    float lightDistance = length(gWorldPos - lightWorldPos);

    // Map to [0;1] range by dividing by farPlane
    lightDistance = lightDistance / farPlane;

    // Write this as modified depth
    gl_FragDepth = lightDistance;
    if(gWorldPos == vec3(0)) {
        gl_FragDepth = 5;
    }
}
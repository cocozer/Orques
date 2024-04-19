#version 330

//variables d'entrées
in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

vec2 fCorrectCoords;

out vec4 fFragColor;

uniform sampler2D uText;



void main() {    

    // Application de la texture
    vec4 texture = texture(uText, vTexCoords);
    
    fFragColor = texture;
}
#version 330

//on definie les attributs d'entree du vertex
layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;

//on declare les variavbles de sortie vers le fragment shader
out vec3 vPosition_vs;
out vec3 vNormal_vs;
out vec2 vTexCoords; 

//on declare les 3 matrices uniformes pour les transformations
uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

void main() {
    //on passe en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 1);

    //on transforme la position et la normale du vertex
    vPosition_vs = vec3(uMVMatrix * vertexPosition);
    vNormal_vs = vec3(uNormalMatrix * vertexNormal);

    //on passe les coordonnées de texture
    vTexCoords = aVertexTexCoords;

    //position finale du vertex
    gl_Position = uMVPMatrix * vertexPosition;
}
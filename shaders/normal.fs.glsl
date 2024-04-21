#version 330 core
//blalalalala
//variables d'entrées
in vec3 vNormal_vs;

out vec3 fFragColor;

void main() {
    vec3 normale = normalize(vNormal_vs);
    fFragColor = normale.xzy;
}
#version 330 core

uniform vec4 color;
out vec4 fragColor; // 최종 출력 색상

void main() {
  fragColor = color;
}
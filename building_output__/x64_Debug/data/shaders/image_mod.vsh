attribute vec3 vertexPosition;
attribute vec2 vertexUV;

varying vec2 uv;

uniform float tFloat;
uniform mat4 mvp;

void main()
{
    // gl_Position.xyz = vertexPosition;
    // gl_Position.x += tFloat;
    // gl_Position.w = 1.0;
    // gl_Position.xyz = vertexPosition;
    gl_Position = mvp * vec4(vertexPosition, 1.0);

    uv = vertexUV;
}

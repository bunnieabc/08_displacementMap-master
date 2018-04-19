#version 150

// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
uniform mat4 textureMatrix;

in vec4 position;
in vec2 texcoord;
in vec4 normal;
in vec4 color;
in vec4 vertex;

//out vec2 vertPos;
out vec2 uv;

void main(void)
{
    gl_Position = vec4( vertex.xy, 0.0, 1.0 );
    gl_Position = sign( gl_Position );
    uv = (vec2( gl_Position.x, - gl_Position.y ) + vec2(1.0) ) / vec2(2.0);
}

#version 150

// these are from the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
uniform float maxHeight;
//in vec4 position;
in vec2 texcoord;
in vec3 normal;
in vec4 vertex;


out vec3 position;
out vec3 vertNV;
out vec3 vertCol;

// this is how we receive the texture
uniform sampler2DRect tex0;
uniform sampler2DRect colormap;
//out vec2 vUv;
out vec2 texCoordVarying;




void main()
{


    vec4 bumpColor = texture(tex0, texcoord);
    
    float df = 0.30 * bumpColor.x + 0.59 * bumpColor.y + 0.11 * bumpColor.z;
    vec4 newVertexPos = vec4(normal * df * (200) , 0.0) + vertex;

    gl_Position = modelViewProjectionMatrix * newVertexPos;

    texCoordVarying = texcoord;
    vertNV = normal;


}
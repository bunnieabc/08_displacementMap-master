#version 150

uniform sampler2DRect tex0;
uniform sampler2DRect colormap;

in vec2 texCoordVarying;

//uniform vec2 iResolution;           // viewport resolution (in pixels)
//uniform float iTime;                 // shader playback time (in seconds)


out vec4 outputColor;
 


void main()
{
   
    outputColor = texture(colormap, texCoordVarying);
}

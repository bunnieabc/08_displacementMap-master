#version 150

uniform sampler2DRect screenshot;
uniform float blurAmnt;

in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{

    vec4 color;
	
    color += 1.0 * texture(screenshot, texCoordVarying + vec2(blurAmnt * -4.0, 0.0));
    color += 2.0 * texture(screenshot, texCoordVarying + vec2(blurAmnt * -3.0, 0.0));
    color += 3.0 * texture(screenshot, texCoordVarying + vec2(blurAmnt * -2.0, 0.0));
    color += 4.0 * texture(screenshot, texCoordVarying + vec2(blurAmnt * -1.0, 0.0));
    
    color += 5.0 * texture(screenshot, texCoordVarying + vec2(blurAmnt, 0));
	
    color += 4.0 * texture(screenshot, texCoordVarying + vec2(blurAmnt * 1.0, 0.0));
    color += 3.0 * texture(screenshot, texCoordVarying + vec2(blurAmnt * 2.0, 0.0));
    color += 2.0 * texture(screenshot, texCoordVarying + vec2(blurAmnt * 3.0, 0.0));
    color += 1.0 * texture(screenshot, texCoordVarying + vec2(blurAmnt * 4.0, 0.0));
    
    color /= 18.0;
    
    outputColor = color;
}
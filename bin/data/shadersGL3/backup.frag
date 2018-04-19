#version 150

in vec2 vertPos;
uniform sampler2DRect u_textureCol;
uniform vec2 u_textureSize;
uniform float u_sigma;
uniform float u_width;

out vec4 outputColor;

float CalcGauss( float x, float sigma ) 
{
    float coeff = 1.0 / (2.0 * 3.14157 * sigma);
    float expon = -(x*x) / (2.0 * sigma);
    return (coeff*exp(expon));
}

void main()
{
    vec2 texC = vertPos.st * 0.5 + 0.5;
    vec4 texCol = texture( u_textureCol, texC );
    vec4 gaussCol = vec4( texCol.rgb, 1.0 );
    vec2 step = 1.0 / u_textureSize;
    for ( int i = 1; i <= u_width; ++ i )
    {
        vec2 actStep = vec2( float(i) * step.x, 0.0 );   // this is for the X-axis
        // vec2 actStep = vec2( 0.0, float(i) * step.y );   this would be for the Y-axis

        float weight = CalcGauss( float(i) / float(u_width), u_sigma );
        texCol = texture2D( u_textureCol, texC + actStep );    
        gaussCol += vec4( texCol.rgb * weight, weight );
        texCol = texture2D( u_textureCol, texC - actStep );
        gaussCol += vec4( texCol.rgb * weight, weight );
    }
    gaussCol.rgb /= gaussCol.w;
    
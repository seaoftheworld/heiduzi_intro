varying vec2 uv;
// out vec4 color;

uniform sampler2D texSampler;
// uniform float blendAlpha = 1.0f;
uniform float blendAlpha;

void main()
{
    // color = texture(texSampler, uv);
    // color.a = blendAlpha;

    gl_FragColor = texture2D(texSampler, uv);
    gl_FragColor.a = blendAlpha;
}

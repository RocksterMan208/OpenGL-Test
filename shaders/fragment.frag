#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
    vec3 lightDir = normalize(vec3(-1.0, 1.0, -1.0));

    float brightness = dot(normalize(normal), lightDir);

    brightness = brightness * 0.5 + 0.5;

    if (normal.y > 0.5)
        brightness += 0.15;

    brightness = min(brightness, 1.0);
    brightness = max(brightness, 0.25);

    vec4 texColor = texture(tex0, texCoord);

    FragColor = vec4(texColor.rgb * brightness, texColor.a);
}
in vec4 f_vertColor;
in vec2 f_fragTextureCoordinates;

uniform int useTexture;
uniform sampler2D textureUnit;

out vec4 f_fragColor;

void main()
{
	if (useTexture == 1)
	{
    	f_fragColor = f_vertColor * texture(textureUnit, f_fragTextureCoordinates);
    }
    else
    {
    	f_fragColor = f_vertColor;
    }
}

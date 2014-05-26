in vec3 v_vertPosition;
in vec3 v_vertNormal;
in vec4 v_vertColor;
in vec2 v_vertTextureCoordinates;

uniform mat4 modelViewProjectionMatrix;

out vec4 f_vertColor;
out vec2 f_fragTextureCoordinates;

void main()
{
	f_vertColor = v_vertColor;
	f_fragTextureCoordinates = v_vertTextureCoordinates;
	gl_Position = modelViewProjectionMatrix * vec4(v_vertPosition, 1.0);
}

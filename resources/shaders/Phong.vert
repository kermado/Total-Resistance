// Vertex inputs.
in vec3 v_vertPosition;
in vec3 v_vertNormal;
in vec4 v_vertColor;
in vec3 v_vertTextureCoordinates;

// MVP matrices.
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

// Transformation matrix for the node.
uniform mat4 nodeTransformationMatrix;

// Values to pass to fragment shader.
out vec3 f_vertPosition;
out vec3 f_vertNormal;
out vec4 f_vertColor;
out vec3 f_vertTextureCoordinates;

void main()
{
	f_vertPosition = vec3(viewMatrix * modelMatrix * nodeTransformationMatrix * vec4(v_vertPosition, 1.0));
	f_vertNormal = normalize(normalMatrix * mat3(nodeTransformationMatrix) * v_vertNormal);
	f_vertColor = v_vertColor;
	f_vertTextureCoordinates = v_vertTextureCoordinates;

	gl_Position = projectionMatrix * vec4(f_vertPosition, 1.0);
}

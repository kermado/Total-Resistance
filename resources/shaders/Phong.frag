// Light definition.
struct Light
{
	vec3 position;
	vec3 color;
};

// Material definition.
struct Material
{
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;
	vec3 emissiveColor;
	float shininess;
};

// Vertex input values passed from the vertex shader.
in vec3 f_vertPosition;
in vec3 f_vertNormal;
in vec4 f_vertColor;
in vec3 f_vertTextureCoordinates;

// Current time (seconds).
uniform float currentTime;

// Material for the mesh.
uniform Material material;

// Directional light.
uniform Light light;

// Texture flag.
uniform int useTexture;

// Texture.
uniform sampler2D diffuseTextureUnit;

// Output for fragment color.
out vec4 f_fragColor;

// Computes the ADS shading model.
void phongModel(vec3 position, vec3 normal, out vec3 ambient, out vec3 diffuse, out vec3 specular)
{
	// Direction to the light.
	vec3 s = normalize(light.position);

	// Direction to the camera/eye.
	vec3 v = normalize(-position.xyz);

	// Direction of light reflected off the surface normal from the light source.
	vec3 r = reflect(-s, normal);

	// Ambient intensity.
	ambient = material.ambientColor;

	// Diffuse intensity.
	diffuse = material.diffuseColor * max(dot(s, normal), 0.0);

	// Specular intensity.
	specular = material.specularColor;
	if (material.shininess > 0)
	{
		specular *= pow(max(dot(r, v), 0.0), material.shininess);
	}
}

void main()
{
	// Compute ambient, diffuse and specular colors using the Phong ADS model.
	vec3 ambient, diffuse, specular;
	phongModel(f_vertPosition, f_vertNormal, ambient, diffuse, specular);

	// Compute the fragment color.
	if (useTexture == 1)
	{
		vec4 textureColor = texture(diffuseTextureUnit, vec2(f_vertTextureCoordinates));
		f_fragColor = vec4(light.color * ((ambient + diffuse) * vec3(textureColor) + specular), 1.0);
	}
	else
	{
		f_fragColor = vec4(light.color * (ambient + diffuse + specular), 1.0);
	}
}

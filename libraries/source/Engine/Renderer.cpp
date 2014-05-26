#include <Engine/Renderer.hpp>

#include <iostream>
#include <memory>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/OrthographicCamera.hpp>
#include <Engine/Attribute/Model.hpp>
#include <Engine/Attribute/DirectionalLight.hpp>

namespace Engine
{
	Renderer::Renderer(std::shared_ptr<ResourceManager> resourceManager)
	: m_resourceManager(resourceManager)
	, m_renderList()
	, m_currentShaderProgram(nullptr)
	, m_drawCount(0)
	{
		// Nothing to do.
	}

	Renderer::~Renderer()
	{
		// Nothing to do.
	}

	unsigned int Renderer::GetDrawCount() const
	{
		return m_drawCount;
	}

	void Renderer::Render(std::map<GameObject::ID, std::shared_ptr<GameObject>>& gameObjects,
		std::shared_ptr<GameObject> cameraGameObject)
	{
		// Reset the draw counter.
		m_drawCount = 0;

		// We cannot render without a valid camera.
		assert (!cameraGameObject->IsDead()
			&& cameraGameObject->HasAttribute<Attribute::Transform>()
			&& cameraGameObject->HasAttribute<Attribute::OrthographicCamera>());

		// Get the camera's transform attribute.
		std::shared_ptr<Attribute::Transform> cameraTransform =
			cameraGameObject->GetAttribute<Attribute::Transform>();

		// Get the camera's orthographic camera attribute.
		std::shared_ptr<Attribute::OrthographicCamera> cameraOrthoCamera =
			cameraGameObject->GetAttribute<Attribute::OrthographicCamera>();

		// Get the PROJECTION matrix.
		const glm::mat4& projectionMatrix = cameraOrthoCamera->GetProjectionMatrix();

		// Derermine the VIEW matrix.
		// This is simply the inverse of the camera's transformation matrix.
		const glm::mat4 viewMatrix = glm::inverse(cameraTransform->GetTransformationMatrix());

		// Construct a list of directional lights.
		std::vector<std::shared_ptr<GameObject>> directionalLights;
		for (auto iter = gameObjects.begin(); iter != gameObjects.end(); ++iter)
		{
			std::shared_ptr<GameObject> gameObject = iter->second;

			if (!gameObject->IsDead() &&
				gameObject->HasAttribute<Attribute::Transform>() &&
				gameObject->HasAttribute<Attribute::DirectionalLight>())
			{
				directionalLights.push_back(gameObject);
			}
		}

		// Clear the render list from the previous execution.
		m_renderList.clear();

		// Populate the render list.
		for (auto iter = gameObjects.begin(); iter != gameObjects.end(); ++iter)
		{
			std::shared_ptr<GameObject> gameObject = iter->second;
			PopulateRenderList(gameObject);
		}

		// Sort the render list by shader program.
		// Has complexity O(n*log(n)).
		std::sort(m_renderList.begin(), m_renderList.end(),
			[this] (std::shared_ptr<GameObject> one, std::shared_ptr<GameObject> two)
			{
				// Get the game objects' shader program attributes.
				std::shared_ptr<Attribute::ShaderProgram> oneShaderProgram =
					one->GetAttribute<Attribute::ShaderProgram>();
				std::shared_ptr<Attribute::ShaderProgram> twoShaderProgram =
					two->GetAttribute<Attribute::ShaderProgram>();

				// Get the corresponding shader program resources.
				std::shared_ptr<ShaderProgram> oneShaderProgramResource =
					m_resourceManager->GetShaderProgram(oneShaderProgram->GetVertexShaderPath(), oneShaderProgram->GetFragmentShaderPath());
				std::shared_ptr<ShaderProgram> twoShaderProgramResource =
					m_resourceManager->GetShaderProgram(twoShaderProgram->GetVertexShaderPath(), twoShaderProgram->GetFragmentShaderPath());

				// Compare the shader program resource IDs.
				if (oneShaderProgramResource && twoShaderProgramResource)
				{
					return oneShaderProgramResource->GetId() > twoShaderProgramResource->GetId();
				}
				else
				{
					return false;
				}
			}
		);

		// Render each game object in the render list.
		for (auto iter = m_renderList.begin(); iter != m_renderList.end(); ++iter)
		{
			std::shared_ptr<GameObject> gameObject = *iter;
			RenderGameObject(gameObject, projectionMatrix, viewMatrix, directionalLights);
		}

		// Reset our record of the shader currently in use.
		m_currentShaderProgram = nullptr;

		// Stop using the last shader.
		// Just a precaution so that we don't inadvertedly modify or use the
		// shader from the outside.
		glUseProgram(0);
	}

	void Renderer::PopulateRenderList(std::shared_ptr<GameObject> gameObject)
	{
		// Check whether the specified game object should be added to the render
		// list.
		if (!gameObject->IsDead() &&
			gameObject->HasAttribute<Attribute::Transform>() &&
			gameObject->HasAttribute<Attribute::Model>() &&
			gameObject->HasAttribute<Attribute::ShaderProgram>())
		{
			// Add the game object to the render list if the model is
			// visible.
			if (gameObject->GetAttribute<Attribute::Model>()->GetVisible())
			{
				m_renderList.push_back(gameObject);
			}			
		}

		// Recursively check any child game objects.
		const unsigned int childCount = gameObject->GetChildCount();
		for (unsigned int i = 0; i < childCount; ++i)
		{
			PopulateRenderList(gameObject->GetChild(i));
		}
	}

	void Renderer::RenderGameObject(std::shared_ptr<GameObject> gameObject,
		const glm::mat4& projectionMatrix,
		const glm::mat4& viewMatrix,
		const std::vector<std::shared_ptr<GameObject>>& directionalLights)
	{
		// Check that the Game Object can be rendered.
		if (!gameObject->IsDead() &&
			gameObject->HasAttribute<Attribute::Transform>() &&
			gameObject->HasAttribute<Attribute::Model>() &&
			gameObject->HasAttribute<Attribute::ShaderProgram>())
		{
			// Get the transform attribute.
			std::shared_ptr<Attribute::Transform> transformAttr = gameObject->GetAttribute<Attribute::Transform>();

			// Determine the MODEL matrix.
			const glm::mat4 modelMatrix = transformAttr->GetTransformationMatrix();

			// Determine the NORMAL matrix.
			const glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));

			// Get the shader program attribute.
			std::shared_ptr<Attribute::ShaderProgram> shaderProgAttr = gameObject->GetAttribute<Attribute::ShaderProgram>();

			// Get the shader resource.
			const std::string vertexShaderFilepath = shaderProgAttr->GetVertexShaderPath();
			const std::string fragmentShaderFilepath = shaderProgAttr->GetFragmentShaderPath();
			std::shared_ptr<ShaderProgram> shaderProgram = m_resourceManager->GetShaderProgram(vertexShaderFilepath, fragmentShaderFilepath);

			// We can only render a Game Object if it has a shader program.
			// The pointer to the shader program returned from the resource
			// manager may be NULL if the shader program hasn't been
			// completely loaded yet. We won't error out in this case.
			// Instead, we just won't render the model until the shader
			// program has been completely loaded.
			if (shaderProgram)
			{
				// If the game object's shader program is not already in use
				// (bound), then we better start using it.
				if (!m_currentShaderProgram || m_currentShaderProgram->GetId() != shaderProgram->GetId())
				{
					shaderProgram->Use();
					m_currentShaderProgram = shaderProgram;

					// Pass the current time to the shader.
					// We only need to do this once for each shader program
					// every frame.
					shaderProgram->SetUniform1f("currentTime", static_cast<float>(glfwGetTime()));
				}

				// Apply the unforms registered with the shader program attribute
				// to the shader program resource.
				shaderProgAttr->ApplyUniforms();

				// Get the model attribute.
				std::shared_ptr<Attribute::Model> modelAttr = gameObject->GetAttribute<Attribute::Model>();

				// Get the model resource.
				std::string modelFilepath = modelAttr->GetPath();
				std::shared_ptr<Model> modelResource = m_resourceManager->GetModel(modelFilepath);

				// Again, like with the shader program, the model resource
				// pointer returned by the resource manager may be NULL if
				// the model has not been completely loaded yet. As with
				// before, we will not error out but instead just skip
				// rendering the model.
				if (modelResource)
				{
					// Get the current animation time (in seconds).
					const double currentAnimationTime = modelAttr->GetCurrentAnimationTime();
					assert(currentAnimationTime >= 0.0);

					// Pass the projection matrix to the shader.
					shaderProgram->SetUniformMatrix4fv("projectionMatrix", projectionMatrix);

					// Pass the view matrix to the shader.
					shaderProgram->SetUniformMatrix4fv("viewMatrix", viewMatrix);

					// Pass the model matrix to the shader.
					shaderProgram->SetUniformMatrix4fv("modelMatrix", modelMatrix);

					// Pass the normal matrix to the shader.
					shaderProgram->SetUniformMatrix3fv("normalMatrix", normalMatrix);

					// Lighting.
					if (directionalLights.size() > 0)
					{
						// Get the light's transform attribute.
						std::shared_ptr<Attribute::Transform> lightTransformAttr =
							directionalLights[0]->GetAttribute<Attribute::Transform>();
						assert(lightTransformAttr);

						// Get the light's directional light attribute.
						std::shared_ptr<Attribute::DirectionalLight> lightDirectionalLightAttr =
							directionalLights[0]->GetAttribute<Attribute::DirectionalLight>();
						assert(lightDirectionalLightAttr);

						// Get the light's position, color and intensity.
						const glm::vec3& lightPosition = lightTransformAttr->GetPosition();
						const glm::vec3& lightColor = lightDirectionalLightAttr->GetColor();
						const float lightIntensity = lightDirectionalLightAttr->GetIntensity();

						// Calculate the light's color intensity.
						const glm::vec3 lightColorIntensity = lightColor * lightIntensity;

						// Pass the light position to the shader.
						shaderProgram->SetUniform3fv("light.position", lightPosition);

						// Pass the light color intensity to the shader.
						shaderProgram->SetUniform3fv("light.color", lightColorIntensity);
					}

					// Render the node tree.
					std::shared_ptr<Model::Node> rootNode = modelResource->GetRootNode();
					if (rootNode)
					{
						RenderNode(
							rootNode, // Node to render.
							glm::mat4(), // Accumulated transformation matrix.
							currentAnimationTime, // Animation time at which to render.
							shaderProgram // Shader program.
						);
					};
				}
			}
			else
			{
				std::cerr << "Render Error: Shader \""
					<< vertexShaderFilepath << ", " << fragmentShaderFilepath
					<< "\" was not loaded" << std::endl;
			}
		}
	}

	void Renderer::RenderNode(std::shared_ptr<Model::Node> node,
		const glm::mat4& parentAccumulatedTransformation,
		double animationTime,
		std::shared_ptr<ShaderProgram> shaderProgram)
	{
		// Get the node's local transformation matrix at the specified
		// animation time.
		const glm::mat4 localTransformationMatrix =
			node->GetLocalTransformationMatrix((double) animationTime);

		// Calculate the node's accumulated transformation matrix.
		// This is the transformation matrix that will map local node
		// coordinates to model coordinates.
		const glm::mat4 accTransformationMatrix = parentAccumulatedTransformation * localTransformationMatrix;

		// Pass the node's accumulated transformation matrix to the shader.
		shaderProgram->SetUniformMatrix4fv("nodeTransformationMatrix", accTransformationMatrix);

		// Get the number of meshes in the node.
		const unsigned int meshCount = node->GetMeshCount();

		// For each mesh...
		for (unsigned int m = 0; m < meshCount; ++m)
		{
			// Get a shared pointer to the mesh.
			std::shared_ptr<Model::Node::Mesh> mesh = node->GetMesh(m);

			// Get a shared pointer to the material that should be applied to
			// the mesh.
			std::shared_ptr<Model::Material> material = mesh->GetMaterial();

			// A mesh should always contain a material.
			// ASSIMP should generate one if the 3D modeling software did not
			// assign one.
			assert(material);

			// Pass the material properties to the shader.
			shaderProgram->SetUniform3fv("material.diffuseColor", material->GetDiffuseColor());
			shaderProgram->SetUniform3fv("material.specularColor", material->GetSpecularColor());
			shaderProgram->SetUniform3fv("material.ambientColor", material->GetAmbientColor());
			shaderProgram->SetUniform3fv("material.emissiveColor", material->GetEmissiveColor());
			shaderProgram->SetUniform1f("material.shininess", material->GetShininess());

			// If the material has a diffuse texture, pass it to the shader.
			const std::string diffuseTexturePath = material->GetDiffuseTexturePath();
			if (diffuseTexturePath.length() > 0)
			{
				// Get a shared pointer to the diffuse texture.
				std::shared_ptr<Texture> texture = m_resourceManager->GetTexture(diffuseTexturePath);

				// Set the uniform flag that specifies whether or not the texture should be used.
				shaderProgram->SetUniform1i("useTexture", (texture) ? 1 : 0);

				// Pass the texture to the shader if the shared pointer to the
				// texture is valid (not null). The pointer may be null if the
				// texture has not been completely loaded yet.
				if (texture)
				{
					// Activate a texture unit.
					glActiveTexture(GL_TEXTURE0 + 0);

					// Bind the shape's texture to the texture unit.
					glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());

					// Pass the texture unit to the shader attribute.
					shaderProgram->SetUniform1i("diffuseTextureUnit", 0);
				}
			}
			else
			{
				shaderProgram->SetUniform1i("useTexture", 0);
			}

			// Bind the VAO for the mesh.
			glBindVertexArray(mesh->GetVAO());

			// Draw the mesh.
			glDrawElements(
				GL_TRIANGLES,
				mesh->GetIndicesCount(),
				GL_UNSIGNED_INT,
				(void*)0
			);

			// Unbind the mesh VAO.
			glBindVertexArray(0);

			// Increment the draw counter.
			++m_drawCount;
		}

		// Get the number of child nodes.
		const unsigned int childNodeCount = node->GetChildNodeCount();

		// Recursively render child nodes.
		for (unsigned int c = 0; c < childNodeCount; ++c)
		{
			RenderNode(
				node->GetChildNode(c),
				accTransformationMatrix,
				animationTime,
				shaderProgram
			);
		}
	}
}

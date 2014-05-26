#include <Engine/Model.hpp>

#include <iostream>
#include <set>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine
{
	Model::Model()
	: m_name("")
	, m_rootNode(nullptr)
	, m_materials()
	, m_animationDuration(0.0)
	{
		// Nothing to do.
	}

	Model::~Model()
	{
		// Nothing to do.
	}

	const std::shared_ptr<Model::Node> Model::GetRootNode() const
	{
		return m_rootNode;
	}

	const double Model::GetAnimationDuration() const
	{
		return m_animationDuration;
	}

	bool Model::LoadFromFile(std::string filepath)
	{
		// Clear any previously loaded data.
		Clear();

		// Create an instance of the ASSIMP importer.
		Assimp::Importer importer;

		// Have ASSIMP read the file.
		const aiScene* assimpScene = importer.ReadFile(
			filepath.c_str(),
			aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials | aiProcess_FlipUVs | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes
		);

		// Check that the file was read successfully.
		if (assimpScene)
		{
			// Get the root node.
			const aiNode* assimpRootNode = assimpScene->mRootNode;

			if (assimpRootNode)
			{
				// Get the path to the parent directory.
				// This is used for determining the full path to textures.
				const std::string directoryPath = filepath.substr(0, filepath.find_last_of('/'));

				// Load the materials.
				LoadMaterials(assimpScene, directoryPath);

				// Load the nodes recursively.
				LoadNode(assimpRootNode, assimpScene, nullptr);

				// Load the node keyframes.
				LoadNodeKeyframes(assimpScene);

				// Success!
				m_name = filepath;
				return true;
			}
			else
			{
				std::cerr << "Failed loading model \"" << filepath
					<< "\" due to missing root node" << std::endl;
				return false;
			}
		}
		else
		{
			std::cerr << "Failed loading model: \"" << filepath
				<< "\" due to error: " << importer.GetErrorString()
				<< std::endl;

			return false;
		}
	}

	void Model::Transform(const glm::vec3& translation, const glm::quat& rotation,
		const glm::vec3& scale)
	{
		// Build the transformation matrix.
		glm::mat4 transformationMatrix = glm::translate(translation)
			* glm::toMat4(rotation)
			* glm::scale(scale);

		// Transform the root node.
		m_rootNode->Transform(transformationMatrix);
	}

	void Model::Clear()
	{
		// Reset the model name.
		m_name = "";

		// Clear the list of materials.
		m_materials.clear();

		// Remove the reference to the root node.
		m_rootNode = nullptr;

		// Reset animation duration.
		m_animationDuration = 0.0;
	}

	std::shared_ptr<Model::Node> Model::FindNodeByName(std::string name)
	{
		return m_rootNode->FindNodeByName(name);
	}

	void Model::LoadMaterials(const aiScene* assimpScene, std::string directoryPath)
	{
		// Get the number of materials in the model.
		const unsigned int numMaterials = assimpScene->mNumMaterials;

		// Resize the materials lost so that we don't waste time dynamically
		// resizing the vector.
		m_materials.resize(numMaterials);

		// For each material...
		for (unsigned int m = 0; m < numMaterials; ++m)
		{
			// Get the ASSIMP material.
			const aiMaterial* assimpMaterial = assimpScene->mMaterials[m];

			// Create our own new material.
			std::shared_ptr<Material> material = std::make_shared<Material>();

			// Populate the material's name.
			aiString name;
			if (assimpMaterial->Get(AI_MATKEY_NAME, name) == AI_SUCCESS)
			{
				material->SetName(std::string(name.data));
			}

			// Populate the material's diffuse color.
			aiColor3D diffuseColor;
			if (assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
			{
				material->SetDiffuseColor(glm::vec3(
					diffuseColor.r,
					diffuseColor.g,
					diffuseColor.b
				));
			}

			// Populate the material's specular color.
			aiColor3D specularColor;
			if (assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS)
			{
				material->SetSpecularColor(glm::vec3(
					specularColor.r,
					specularColor.g,
					specularColor.b
				));
			}

			// Populate the material's ambient color.
			aiColor3D ambientColor;
			if (assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS)
			{
				material->SetAmbientColor(glm::vec3(
					ambientColor.r,
					ambientColor.g,
					ambientColor.b
				));
			}

			// Populate the material's emissive color.
			aiColor3D emissiveColor;
			if (assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, emissiveColor) == AI_SUCCESS)
			{
				material->SetEmissiveColor(glm::vec3(
					emissiveColor.r,
					emissiveColor.g,
					emissiveColor.b
				));
			}

			// Populate the material's shininess.
			float shininess;
			if (assimpMaterial->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
			{
				material->SetShininess(shininess);
			}

			// Populate the material's diffuse texture path.
			// Note: Other textures (specular, emissive, etc) are ignored.
			aiString diffuseTexturePath;
			if (assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexturePath) == AI_SUCCESS)
			{
				std::string texturePath(diffuseTexturePath.data);
				if (texturePath.at(0) != '/')
				{
					texturePath = "/" + texturePath;
				}
				material->SetDiffuseTexturePath(directoryPath + texturePath);
			}

			// Add the material to the list.
			m_materials[m] = material;
		}
	}

	void Model::LoadNode(const aiNode* assimpNode, const aiScene* assimpScene, std::shared_ptr<Node> parentNode)
	{
		// Get the node's name.
		const std::string name(assimpNode->mName.data);

		// Calculate the local bind pose transformation matrix.
		const glm::mat4 localBindTransformationMatrix =
			glm::transpose(ConvertToMat4(assimpNode->mTransformation));

		// Create our own new node.
		// We will be populating this with the data retrieved from ASSIMP.
		std::shared_ptr<Node> node = std::make_shared<Node>(name, localBindTransformationMatrix);

		// Get the number of meshes in the ASSIMP node.
		const unsigned int numMeshesInNode = assimpNode->mNumMeshes;

		// For every mesh in the ASSIMP node...
		for (unsigned int m = 0; m < numMeshesInNode; ++m)
		{
			// Get the ASSIMP mesh.
			const unsigned int assimpMeshIndex = assimpNode->mMeshes[m];
			const aiMesh* assimpMesh = assimpScene->mMeshes[assimpMeshIndex];

			// Create our own new mesh.
			std::shared_ptr<Node::Mesh> mesh = std::make_shared<Node::Mesh>();

			// Get the number of vertices that compose the mesh.
			const unsigned int numVertices = assimpMesh->mNumVertices;

			// Set the material that is applied to the mesh.
			mesh->SetMaterial(m_materials[assimpMesh->mMaterialIndex]);

			// Populate the mesh's vertex positions, normals and texture
			// coordinates.
			for (unsigned int v = 0; v < numVertices; ++v)
			{
				mesh->AddVertexPosition(ConvertToVec3(assimpMesh->mVertices[v]));
				mesh->AddVertexNormal(ConvertToVec3(assimpMesh->mNormals[v]));
				mesh->AddVertexTextureCoordinates(ConvertToVec3(assimpMesh->mTextureCoords[0][v]));
			}

			// Get the number of faces and indices in the mesh.
			const unsigned int numFaces = assimpMesh->mNumFaces;

			// Populate the mesh's faces.
			for (unsigned int f = 0; f < numFaces; ++f)
			{
				// Get the ASSIMP face.
				const aiFace& assimpFace = assimpMesh->mFaces[f];

				// Ensure that we are dealing with triangular faces.
				if (assimpFace.mNumIndices == 3)
				{
					for (unsigned int i = 0; i < 3; ++i)
					{
						mesh->AddVertexIndex(static_cast<unsigned int>(assimpFace.mIndices[i]));
					}
				}
				else
				{
					std::cout << "WARNING: Face was not triangular" << std::endl;
				}
			}

			// Update the mesh's buffer objects.
			// This sends the mesh data over to the graphics card.
			mesh->UpdateBuffers();

			// Add the mesh to the node.
			node->AddMesh(mesh);
		}

		// Add the node to the model.
		if (!parentNode)
		{
			// The created node doesn't have a parent node. Therefore, it must
			// be the root node.
			m_rootNode = node;
		}
		else
		{
			// Add the created node as a child to its parent node.
			parentNode->AddChildNode(node);
		}

		// Get the number of ASSIMP child nodes.
		const unsigned int numChildNodes = assimpNode->mNumChildren;

		// Recursively load child nodes.
		for (unsigned int n = 0; n < numChildNodes; ++n)
		{
			LoadNode(assimpNode->mChildren[n], assimpScene, node);
		}
	}

	void Model::LoadNodeKeyframes(const aiScene* assimpScene)
	{
		// Get the number of animations.
		const unsigned int numAnimations = assimpScene->mNumAnimations;

		// Only proceed if there is an animation to load.
		if (numAnimations > 0)
		{
			// Check whether we will be ignoring any animations.
			// Only the first animation will be loaded. All other animations
			// will be ignored.
			//
			// TODO: Support multiple animations.
			if (numAnimations > 1)
			{
				std::cout << "WARNING: The model \"" << m_name
					<< "\" contains multiple animations. "
					<< "Only the first animation will be loaded." << std::endl;
			}

			// Get the first animation.
			const aiAnimation* assimpAnimation = assimpScene->mAnimations[0];
			assert(assimpAnimation);

			// Get the animation speed.
			const double speedInTicksPerSecond = assimpAnimation->mTicksPerSecond;

			// Record the animation duration.
			m_animationDuration = assimpAnimation->mDuration;

			// Get the number of ASSIMP animation channels.
			// Each animation channel describes the movement of a single node
			// over time.
			const unsigned int numAnimationChannels = assimpAnimation->mNumChannels;

			// For each ASSIMP animation channel...
			for (unsigned int c = 0; c < numAnimationChannels; ++c)
			{
				// Get the ASSIMP channel.
				const aiNodeAnim* channel = assimpAnimation->mChannels[c];

				// Get the ASSIMP node that the channel affects.
				const aiNode* targetNode = assimpScene->mRootNode->FindNode(channel->mNodeName);

				// Get the name for the node.
				const std::string targetNodeName(targetNode->mName.data);

				// Get the number of individual keyframes for the node.
				const unsigned int numPositionKeys = channel->mNumPositionKeys;

				// Determine the keyframe times in "ticks" for the node.
				// We will need to convert these times to seconds later by using
				// the animation speed provided by ASSIMP.
				std::set<double> keyframeTimes;
				for (unsigned int k = 0; k < numPositionKeys; ++k)
				{
					keyframeTimes.insert(channel->mPositionKeys[k].mTime);

					// TODO: Check whether it is necessary to insert the
					// keyframe times for other keys (rotation and scale).
				}

				// For each keyframe time...
				for (auto iter = keyframeTimes.begin(); iter != keyframeTimes.end(); ++iter)
				{
					// Get the keyframe time (in ticks).
					const double keyframeTimeInTicks = *iter;

					// Find our corresponding node.
					std::shared_ptr<Node> node = FindNodeByName(targetNodeName);

					// Get the local position for the node at the keyframe time.
					const glm::vec3 keyframePosition =
						DetermineNodeLocalPositionAtTime(channel, keyframeTimeInTicks);

					// Get the local rotation for the node at the keyframe time.
					const glm::quat keyframeRotation =
						DetermineNodeLocalRotationAtTime(channel, keyframeTimeInTicks);

					// Get the local scale for the node at the keyframe time.
					const glm::vec3 keyframeScale =
						DetermineNodeLocalScaleAtTime(channel, keyframeTimeInTicks);

					// Build the local transformation matrix for the node.
					const glm::mat4 localTransformationMatrix = glm::translate(keyframePosition)
					 	* glm::toMat4(keyframeRotation)
					 	* glm::scale(keyframeScale);

					// Create our new keyframe.
					std::shared_ptr<Node::Keyframe> keyframe = std::make_shared<Node::Keyframe>(
						keyframeTimeInTicks / speedInTicksPerSecond, // Convert ticks to seconds here!
						localTransformationMatrix
					);

					// Add the keyframe to the node.
					node->AddKeyframe(keyframe);
				}
			}
		}
	}

	glm::vec3 Model::DetermineNodeLocalPositionAtTime(const aiNodeAnim* channel,
		double timeInTicks)
	{
		// Get the number of position keyframes.
		const unsigned int numPositionKeys = channel->mNumPositionKeys;

		// Get the index for the position key immediately preceding or equal to
		// the keyframe time.
		size_t leftPositionKeyIndex = 0;
		while (leftPositionKeyIndex < numPositionKeys - 1)
		{
			// If the next key is in the future relative to the keyframe time,
			// then the current key either occurs exactly at the keyframe time
			// or immediately before the keyframe time.
			if (channel->mPositionKeys[leftPositionKeyIndex + 1].mTime > timeInTicks)
			{
				break;
			}

			// Move to the next index.
			++leftPositionKeyIndex;
		}

		// Check whether we need to perform linear interpolation. We don't need
		// to interpolate if the keyframe time specified is outside of the
		// animation timeframe or the left key index found has a time that is
		// equal to the specified keyframe time.
		if (leftPositionKeyIndex >= numPositionKeys - 1 ||
			channel->mPositionKeys[leftPositionKeyIndex].mTime == timeInTicks)
		{
			// No need to interpolate.
			// We just output the position for the keyframe to the left.
			return ConvertToVec3(channel->mPositionKeys[leftPositionKeyIndex].mValue);
		}
		else
		{
			// Determine the right position key index.
			const size_t rightPositionKeyIndex = leftPositionKeyIndex + 1;

			// Perform linear interpolation.
			const glm::vec3 leftValue = ConvertToVec3(channel->mPositionKeys[leftPositionKeyIndex].mValue);
			const double leftTime = channel->mPositionKeys[leftPositionKeyIndex].mTime;
			const glm::vec3 rightValue = ConvertToVec3(channel->mPositionKeys[rightPositionKeyIndex].mValue);
			const double rightTime = channel->mPositionKeys[rightPositionKeyIndex].mTime;
			return leftValue + (rightValue - leftValue) * static_cast<float>((timeInTicks - leftTime) / (rightTime - leftTime));
		}
	}

	glm::quat Model::DetermineNodeLocalRotationAtTime(const aiNodeAnim* channel, double timeInTicks)
	{
		// Get the number of rotation keyframes.
		const unsigned int numRotationKeys = channel->mNumRotationKeys;

		// Get the index for the rotation key immediately preceding or equal to
		// the keyframe time.
		size_t leftRotationKeyIndex = 0;
		while (leftRotationKeyIndex < numRotationKeys - 1)
		{
			// If the next key is in the future relative to the keyframe time,
			// then the current key either occurs exactly at the keyframe time
			// or immediately before the keyframe time.
			if (channel->mRotationKeys[leftRotationKeyIndex + 1].mTime > timeInTicks)
			{
				break;
			}

			// Move to the next index.
			++leftRotationKeyIndex;
		}

		// Check whether we need to perform linear interpolation. We don't need
		// to interpolate if the keyframe time specified is outside of the
		// animation timeframe or the left key index found has a time that is
		// equal to the specified keyframe time.
		if (leftRotationKeyIndex >= numRotationKeys - 1 ||
			channel->mRotationKeys[leftRotationKeyIndex].mTime == timeInTicks)
		{
			// No need to interpolate.
			// We just output the rotation for the keyframe to the left.
			return ConvertToQuat(channel->mRotationKeys[leftRotationKeyIndex].mValue);
		}
		else
		{
			// Determine the right rotation key index.
			const size_t rightRotationKeyIndex = leftRotationKeyIndex + 1;

			// Perform linear interpolation.
			const glm::quat leftValue = ConvertToQuat(channel->mRotationKeys[leftRotationKeyIndex].mValue);
			const double leftTime = channel->mRotationKeys[leftRotationKeyIndex].mTime;
			const glm::quat rightValue = ConvertToQuat(channel->mRotationKeys[rightRotationKeyIndex].mValue);
			const double rightTime = channel->mRotationKeys[rightRotationKeyIndex].mTime;
			const double interpolationFactor = (timeInTicks - leftTime) / (rightTime - leftTime);
			return glm::mix(leftValue, rightValue, (float) interpolationFactor);
		}
	}

	glm::vec3 Model::DetermineNodeLocalScaleAtTime(const aiNodeAnim* channel, double timeInTicks)
	{
		// Get the number of scaling keyframes.
		const unsigned int numScalingKeys = channel->mNumScalingKeys;

		// Get the index for the scaling key immediately preceding or equal to
		// the keyframe time.
		size_t leftScalingKeyIndex = 0;
		while (leftScalingKeyIndex < numScalingKeys - 1)
		{
			// If the next key is in the future relative to the keyframe time,
			// then the current key either occurs exactly at the keyframe time
			// or immediately before the keyframe time.
			if (channel->mScalingKeys[leftScalingKeyIndex + 1].mTime > timeInTicks)
			{
				break;
			}

			// Move to the next index.
			++leftScalingKeyIndex;
		}

		// Check whether we need to perform linear interpolation. We don't need
		// to interpolate if the keyframe time specified is outside of the
		// animation timeframe or the left key index found has a time that is
		// equal to the specified keyframe time.
		if (leftScalingKeyIndex >= numScalingKeys - 1 ||
			channel->mScalingKeys[leftScalingKeyIndex].mTime == timeInTicks)
		{
			// No need to interpolate.
			// We just output the scale for the keyframe to the left.
			return ConvertToVec3(channel->mScalingKeys[leftScalingKeyIndex].mValue);
		}
		else
		{
			// Determine the right position key index.
			const size_t rightScalingKeyIndex = leftScalingKeyIndex + 1;

			// Perform linear interpolation.
			const glm::vec3 leftValue = ConvertToVec3(channel->mScalingKeys[leftScalingKeyIndex].mValue);
			const double leftTime = channel->mScalingKeys[leftScalingKeyIndex].mTime;
			const glm::vec3 rightValue = ConvertToVec3(channel->mScalingKeys[rightScalingKeyIndex].mValue);
			const double rightTime = channel->mScalingKeys[rightScalingKeyIndex].mTime;
			return leftValue + (rightValue - leftValue) * static_cast<float>((timeInTicks - leftTime) / (rightTime - leftTime));
		}
	}

	glm::mat4 Model::ConvertToMat4(const aiMatrix4x4& assimpMatrix)
	{
		return glm::mat4(
			assimpMatrix.a1, assimpMatrix.a2, assimpMatrix.a3, assimpMatrix.a4,
			assimpMatrix.b1, assimpMatrix.b2, assimpMatrix.b3, assimpMatrix.b4,
			assimpMatrix.c1, assimpMatrix.c2, assimpMatrix.c3, assimpMatrix.c4,
			assimpMatrix.d1, assimpMatrix.d2, assimpMatrix.d3, assimpMatrix.d4
		);
	}

	glm::vec3 Model::ConvertToVec3(const aiVector3D& assimpVector)
	{
		return glm::vec3(
			assimpVector.x,
			assimpVector.y,
			assimpVector.z
		);
	}

	glm::quat Model::ConvertToQuat(const aiQuaternion& assimpQuaternion)
	{
		return glm::quat(
			assimpQuaternion.w,
			assimpQuaternion.x,
			assimpQuaternion.y,
			assimpQuaternion.z
		);
	}

	Model::Node::Node(std::string name, const glm::mat4& bindTransformationMatrix)
	: m_name(name)
	, m_bindTransformationMatrix(bindTransformationMatrix)
	, m_keyframes()
	, m_children()
	, m_meshes()
	{
		// Nothing to do.
	}

	Model::Node::~Node()
	{
		// Nothing to do.
	}

	std::string Model::Node::GetName() const
	{
		return m_name;
	}

	unsigned int Model::Node::GetChildNodeCount() const
	{
		return m_children.size();
	}

	unsigned int Model::Node::GetMeshCount() const
	{
		return m_meshes.size();
	}

	std::shared_ptr<Model::Node> Model::Node::FindNodeByName(std::string name)
	{
		if (m_name == name)
		{
			return shared_from_this();
		}
		else
		{
			for (std::shared_ptr<Node> node : m_children)
			{
				std::shared_ptr<Node> result = node->FindNodeByName(name);
				if (result) return result;
			}

			return nullptr;
		}
	}

	std::shared_ptr<Model::Node> Model::Node::GetChildNode(unsigned int index) const
	{
		assert(index < m_children.size());
		return m_children[index];
	}

	std::shared_ptr<Model::Node::Mesh> Model::Node::GetMesh(unsigned int index) const
	{
		assert(index < m_meshes.size());
		return m_meshes[index];
	}

	const glm::mat4& Model::Node::GetLocalBindTransformationMatrix() const
	{
		return m_bindTransformationMatrix;
	}

	// TODO: Perform linear interpolation
	const glm::mat4 Model::Node::GetLocalTransformationMatrix(double time) const
	{
		// If we have no animations, just return the local bind pose
		// transformation matrix.
		if (m_keyframes.size() <= 0)
		{
			return GetLocalBindTransformationMatrix();
		}
		else
		{
			// Find the nearest keyframe.
			for (unsigned int k = 0; k < m_keyframes.size() - 1; ++k)
			{
				if (m_keyframes[k + 1]->GetTime() > time)
				{
					// Return the local transformation matrix for the node
					// at this nearest keyframe.
					return m_keyframes[k]->GetLocalTransformationMatrix();
				}
			}

			// Out of range - return the transformation matrix for the last
			// keyframe.
			return m_keyframes[m_keyframes.size() - 1]->GetLocalTransformationMatrix();
		}
	}

	void Model::Node::Transform(const glm::mat4& transform)
	{
		// Transform the bind pose.
		m_bindTransformationMatrix = transform * m_bindTransformationMatrix;

		// Transform any keyframes.
		for (std::shared_ptr<Model::Node::Keyframe> keyframe : m_keyframes)
		{
			keyframe->Transform(transform);
		}
	}

	void Model::Node::AddKeyframe(std::shared_ptr<Model::Node::Keyframe> keyframe)
	{
		m_keyframes.push_back(keyframe);
	}

	void Model::Node::AddMesh(std::shared_ptr<Model::Node::Mesh> mesh)
	{
		m_meshes.push_back(mesh);
	}

	void Model::Node::AddChildNode(std::shared_ptr<Node> node)
	{
		m_children.push_back(node);
	}

	Model::Node::Keyframe::Keyframe(double time, const glm::mat4& transformationMatrix)
	: m_time(time)
	, m_transformationMatrix(transformationMatrix)
	{
		// Nothing to do.
	}

	Model::Node::Keyframe::~Keyframe()
	{
		// Nothing to do.
	}

	double Model::Node::Keyframe::GetTime() const
	{
		return m_time;
	}

	const glm::mat4& Model::Node::Keyframe::GetLocalTransformationMatrix() const
	{
		return m_transformationMatrix;
	}

	void Model::Node::Keyframe::Transform(const glm::mat4& transform)
	{
		m_transformationMatrix = transform * m_transformationMatrix;
	}

	Model::Node::Mesh::Mesh()
	: m_material(nullptr)
	, m_positions()
	, m_normals()
	, m_textureCoordinates()
	, m_indices()
	, m_positionVBO(0)
	, m_normalVBO(0)
	, m_textureCoordinatesVBO(0)
	, m_indexVBO(0)
	, m_VAO(0)
	{
		// Generate VBOs.
		glGenBuffers(1, &m_positionVBO);
		glGenBuffers(1, &m_normalVBO);
		glGenBuffers(1, &m_textureCoordinatesVBO);
		glGenBuffers(1, &m_indexVBO);

		// Check VBOs were generated successfully.
		assert(m_positionVBO > 0);
		assert(m_normalVBO > 0);
		assert(m_textureCoordinatesVBO > 0);
		assert(m_indexVBO > 0);
	}

	Model::Node::Mesh::~Mesh()
	{
		// Delete the VBOs.
		glDeleteBuffers(1, &m_positionVBO);
		glDeleteBuffers(1, &m_normalVBO);
		glDeleteBuffers(1, &m_textureCoordinatesVBO);
		glDeleteBuffers(1, &m_indexVBO);
	}

	void Model::Node::Mesh::UpdateBuffers() const
	{
		// Send the vertex position data to the corresponding VBO.
		glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(glm::vec3) * m_positions.size(),
			&m_positions[0],
			GL_STATIC_DRAW
		);

		// Set the vertex position data to be the first attribute
		// passed to the shader (index 0).
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Send the vertex normal data to the corresponding VBO.
		glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(glm::vec3) * m_normals.size(),
			&m_normals[0],
			GL_STATIC_DRAW
		);

		// Set the vertex normal data to be the second attribute
		// passed to the shader (index 1).
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Send the vertex texture coordinates data to the corresponding VBO.
		glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordinatesVBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(glm::vec3) * m_textureCoordinates.size(),
			&m_textureCoordinates[0],
			GL_STATIC_DRAW
		);

		// Set the vertex texture coordinates data to be the fourth attribute
		// passed to the shader (index 3).
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Send the vertex indices to the corresponding VBO.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * m_indices.size(),
			&m_indices[0],
			GL_STATIC_DRAW
		);

		// Unbind the last buffer.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	unsigned int Model::Node::Mesh::GetIndicesCount() const
	{
		return m_indices.size();
	}

	unsigned int Model::Node::Mesh::GetVerticesCount() const
	{
		return m_positions.size();
	}

	std::shared_ptr<Model::Material> Model::Node::Mesh::GetMaterial() const
	{
		return m_material;
	}

	void Model::Node::Mesh::SetMaterial(std::shared_ptr<Material> material)
	{
		m_material = material;
	}

	void Model::Node::Mesh::AddVertexPosition(const glm::vec3& position)
	{
		m_positions.push_back(position);
	}

	void Model::Node::Mesh::AddVertexNormal(const glm::vec3& normal)
	{
		m_normals.push_back(normal);
	}

	void Model::Node::Mesh::AddVertexTextureCoordinates(const glm::vec3& textureCoordinates)
	{
		m_textureCoordinates.push_back(textureCoordinates);
	}

	void Model::Node::Mesh::AddVertexIndex(unsigned int index)
	{
		m_indices.push_back(index);
	}

	const glm::vec3& Model::Node::Mesh::GetVertexPosition(unsigned int index) const
	{
		return m_positions[index];
	}

	GLuint Model::Node::Mesh::GetVAO()
	{
		// Check whether we need to create the VAO.
		if (m_VAO <= 0)
		{
			// Generate the VAO.
			glGenVertexArrays(1, &m_VAO);
			assert(m_VAO > 0);

			// Bind the VAO.
			glBindVertexArray(m_VAO);

			// Bind the positions VBO to the VAO.
			glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			// Bind the normals VBO to the VAO.
			glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

			// Bind the texture coordinates VBO to the VAO.
			glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordinatesVBO);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

			// Bind the indices VBO to the VAO.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);

			// Unbind the VAO.
			glBindVertexArray(0);
		}

		return m_VAO;
	}

	Model::Material::Material()
	: m_name("")
	, m_diffuseColor(glm::vec3(0.0f, 0.0f, 0.0f))
	, m_specularColor(glm::vec3(0.0f, 0.0f, 0.0f))
	, m_ambientColor(glm::vec3(0.0f, 0.0f, 0.0f))
	, m_emissiveColor(glm::vec3(0.0f, 0.0f, 0.0f))
	, m_shininess(0.0f)
	, m_diffuseTexturePath("")
	{
		// Nothing to do.
	}

	Model::Material::~Material()
	{
		// Nothing to do.
	}

	void Model::Material::SetName(const std::string name)
	{
		m_name = name;
	}

	const glm::vec3& Model::Material::GetDiffuseColor() const
	{
		return m_diffuseColor;
	}

	void Model::Material::SetDiffuseColor(const glm::vec3& diffuseColor)
	{
		m_diffuseColor = diffuseColor;
	}

	const glm::vec3& Model::Material::GetSpecularColor() const
	{
		return m_specularColor;
	}

	void Model::Material::SetSpecularColor(const glm::vec3& specularColor)
	{
		m_specularColor = specularColor;
	}

	const glm::vec3& Model::Material::GetAmbientColor() const
	{
		return m_ambientColor;
	}

	void Model::Material::SetAmbientColor(const glm::vec3& ambientColor)
	{
		m_ambientColor = ambientColor;
	}

	const glm::vec3& Model::Material::GetEmissiveColor() const
	{
		return m_emissiveColor;
	}

	void Model::Material::SetEmissiveColor(const glm::vec3& emissiveColor)
	{
		m_emissiveColor = emissiveColor;
	}

	float Model::Material::GetShininess() const
	{
		return m_shininess;
	}

	void Model::Material::SetShininess(float shininess)
	{
		m_shininess = shininess;
	}

	std::string Model::Material::GetDiffuseTexturePath() const
	{
		return m_diffuseTexturePath;
	}

	void Model::Material::SetDiffuseTexturePath(std::string diffuseTexturePath)
	{
		m_diffuseTexturePath = diffuseTexturePath;
	}
}

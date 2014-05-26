#ifndef MODEL_H
#define	MODEL_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Engine/NonCopyable.hpp>

namespace Engine
{
	class Model : private NonCopyable
	{
	public:
		class Material
		{
		public:
			/**
			 * Constructor.
			 */
			Material();

			/**
			 * Destructor.
			 */
			~Material();

			/**
			 * Sets the name for the material.
			 *
			 * @param name Material name.
			 */
			void SetName(const std::string name);

			/**
			 * Returns the diffuse color for the material.
			 *
			 * @return Diffuse color (RGB).
			 */
			const glm::vec3& GetDiffuseColor() const;

			/**
			 * Sets the diffuse color for the material.
			 *
			 * @param diffuseColor Diffuse color (RGB).
			 */
			void SetDiffuseColor(const glm::vec3& diffuseColor);

			/**
			 * Returns the specular color for the material.
			 *
			 * @return Specular color (RGB).
			 */
			const glm::vec3& GetSpecularColor() const;

			/**
			 * Sets the specular color for the material.
			 *
			 * @param specularColor Specular color (RGB).
			 */
			void SetSpecularColor(const glm::vec3& specularColor);

			/**
			 * Returns the ambient color for the material.
			 *
			 * @return Ambient color (RGB).
			 */
			const glm::vec3& GetAmbientColor() const;

			/**
			 * Sets the ambient color for the material.
			 *
			 * @param ambientColor Ambient color (RGB).
			 */
			void SetAmbientColor(const glm::vec3& ambientColor);

			/**
			 * Returns the emissive color for the material.
			 *
			 * @return Emissive color (RGB).
			 */
			const glm::vec3& GetEmissiveColor() const;

			/**
			 * Sets the emissive color for the material.
			 *
			 * @param emissiveColor Emissibe color (RGB).
			 */
			void SetEmissiveColor(const glm::vec3& emissiveColor);

			/**
			 * Returns the shininess of the material.
			 *
			 * @return Shininess.
			 */
			float GetShininess() const;

			/**
			 * Sets the shininess of the material.
			 *
			 * @param shininess Material shininess.
			 */
			void SetShininess(float shininess);

			/**
			 * Returns the path to the diffuse texture for the material.
			 *
			 * @return Path to the material's diffuse texture.
			 */
			std::string GetDiffuseTexturePath() const;

			/**
			 * Sets the path to the diffuse texture for the material.
			 *
			 * @param diffuseTexturePath Path to the diffuse texture.
			 */
			void SetDiffuseTexturePath(std::string diffuseTexturePath);

		private:
			/**
			 * Name (may not be unique).
			 */
			std::string m_name;

			/**
			 * Diffuse color.
			 */
			glm::vec3 m_diffuseColor;

			/**
			 * Specular color.
			 */
			glm::vec3 m_specularColor;

			/**
			 * Ambient color.
			 */
			glm::vec3 m_ambientColor;

			/**
			 * Emissive color.
			 */
			glm::vec3 m_emissiveColor;

			/**
			 * Shininess.
			 */
			float m_shininess;

			/**
			 * Path to the diffuse texture.
			 */
			std::string m_diffuseTexturePath;
		};

		/**
		 * Node.
		 *
		 * Nodes have a transformation matrix, which transforms
		 * vectors in the node space to the model space, and
		 * zero or more keyframes. They can also contain meshes.
		 */
		class Node : private NonCopyable, public std::enable_shared_from_this<Node>
		{
		public:
			/**
			 * Keyframe.
			 *
			 * Represents an animation keyframe. This contains a time
			 * at which the keyframe occurs after the start of the
			 * animation in addition to the node's transformation
			 * matrix to be used at that time.
			 */
			class Keyframe : private NonCopyable
			{
			public:
				/**
				 * Constructor.
				 *
				 * @param time Time at which the keyframe occurs in seconds
				 * after the start of the animation.
				 * @param transformationMatrix Transformation matrix for the
				 * node at the keyframe time.
				 */
				Keyframe(double time, const glm::mat4& transformationMatrix);

				/**
				 * Destructor.
				 */
				~Keyframe();

				/**
				 * Returns the time (in seconds) after the start of the
				 * animation at which the keyframe is scheduled.
				 *
				 * @return Scheduled time (in seconds).
				 */
				double GetTime() const;

				/**
				 * Returns the local transformation matrix for the node at the
				 * keyframe.
				 *
				 * @return Node's local transformation matrix at the keyframe.
				 */
				const glm::mat4& GetLocalTransformationMatrix() const;

				/**
				 * Transforms the keyframe by the specified transformation
				 * matrix.
				 *
				 * @param transform Transformation matrix to apply.
				 */
				void Transform(const glm::mat4& transform);

			private:
				/**
				 * Time at which the keyframe occurs (in seconds)
				 */
				double m_time;

				/**
				 * Local transformation matrix for the node.
				 */
				glm::mat4 m_transformationMatrix;
			};

			/**
			 * Mesh.
			 *
			 * Meshes contain vertices and faces.
			 */
			class Mesh : private NonCopyable
			{
			public:
				/**
				 * Constructor.
				 */
				Mesh();

				/**
				 * Destructor.
				 */
				~Mesh();

				/**
				 * Returns the number of indices in the mesh.
				 *
				 * @return Number of indices.
				 */
				unsigned int GetIndicesCount() const;

				/**
				 * Returns the number of vertices in the mesh.
				 *
				 * @return Number of vertices.
				 */
				unsigned int GetVerticesCount() const;

				/**
				 * Returns a shared pointer to the mesh's material.
				 *
				 * @return Shared pointer to the material that should be applied
				 * to the mesh.
				 */
				std::shared_ptr<Material> GetMaterial() const;

				/**
				 * Updates the VBOs in the mesh by sending over
				 * the vertex data.
				 */
				void UpdateBuffers() const;

				/**
				 * Sets the material for the mesh.
				 *
				 * @param material Shared pointer to the material.
				 */
				void SetMaterial(std::shared_ptr<Material> material);

				/**
				 * Adds a vertex position to the mesh.
				 *
				 * @param position Vertex position.
				 */
				void AddVertexPosition(const glm::vec3& position);

				/**
				 * Adds a vertex normal to the mesh.
				 *
				 * @param normal Vertex normal.
				 */
				void AddVertexNormal(const glm::vec3& normal);

				/**
				 * Adds a vertex texture coordinates to the mesh.
				 *
				 * @param textureCoordinates Vertex texture coordinates.
				 */
				void AddVertexTextureCoordinates(const glm::vec3& textureCoordinates);

				/**
				 * Adds a vertex index, that composes a face, to the
				 * mesh.
				 *
				 * @param index Vertex index for the face.
				 */
				void AddVertexIndex(unsigned int index);

				/**
				 * Returns a reference to the vertex positions specified by the
				 * supplied index.
				 *
				 * @param index Index for the vertex to return.
				 * @return Reference to the index-th vertex in the mesh.
				 */
				const glm::vec3& GetVertexPosition(unsigned int index) const;

				/**
				 * Returns the Vertex Array Object identifier for the mesh.
				 *
				 * @return Vertex Array Object identifier.
				 */
				GLuint GetVAO();

			private:
				/**
				 * Pointer to the material that should be applied to
				 * the mesh.
				 */
				std::shared_ptr<Material> m_material;

				/**
				 * Vertex positions.
				 */
				std::vector<glm::vec3> m_positions;

				/**
				 * Vertex normals.
				 */
				std::vector<glm::vec3> m_normals;

				/**
				 * Vertex texture coordinates.
				 */
				std::vector<glm::vec3> m_textureCoordinates;

				/**
				 * Vertex indices.
				 */
				std::vector<unsigned int> m_indices;

				/**
				 * VBO for vertex positions.
				 */
				GLuint m_positionVBO;

				/**
				 * VBO for vertex normals.
				 */
				GLuint m_normalVBO;

				/**
				 * VBO for the vertex texture coordinates.
				 */
				GLuint m_textureCoordinatesVBO;

				/**
				 * VBO for the faces.
				 */
				GLuint m_indexVBO;

				/**
				 * Vertex Array Object.
				 */
				GLuint m_VAO;
			};

			/**
			 * Constructor.
			 *
			 * @param name Name for the node.
			 * @param bindTransformationMatrix Local transformation matrix for
			 * the bind pose.
			 */
			Node(std::string name, const glm::mat4& bindTransformationMatrix);

			/**
			 * Destructor.
			 */
			~Node();

			/**
			 * Returns the name of the node.
			 *
			 * @return Node's name.
			 */
			std::string GetName() const;

			/**
			 * Searches through the node tree, starting at this node,
			 * and returns a shared pointer to the node with the specified name.
			 *
			 * @param name Name for the node to return.
			 * @return Shared pointer to the node with the specified name. A
			 * nullptr is returns if no nodes match the specified name.
			 */
			std::shared_ptr<Node> FindNodeByName(std::string name);

			/**
			 * Returns the number of child nodes.
			 *
			 * @return Number of child nodes.
			 */
			unsigned int GetChildNodeCount() const;

			/**
			 * Returns the number of meshes in the node.
			 *
			 * @return Number of meshes.
			 */
			unsigned int GetMeshCount() const;

			/**
			 * Returns a shared pointer to the child nodes specified by the
			 * provided index.
			 *
			 * @param index Index for the node to return.
			 * @return Shared pointer to the index-th child node.
			 */
			std::shared_ptr<Node> GetChildNode(unsigned int index) const;

			/**
			 * Returns a shared pointer to the mesh specified by the provided
			 * index.
			 *
			 * @param index Index for the node to return.
			 * @return Shared pointer to the index-th mesh.
			 */
			std::shared_ptr<Mesh> GetMesh(unsigned int index) const;

			/**
			 * Returns a reference to the node's bind pose transformation
			 * matrix.
			 *
			 * @return Bind pose transformation matrix.
			 */
			const glm::mat4& GetLocalBindTransformationMatrix() const;

			/**
			 * Calculates the node's local transformation matrix at the
			 * specified time.
			 *
			 * @param time Time for the transformation matrix (in seconds).
			 */
			const glm::mat4 GetLocalTransformationMatrix(double time) const;

			/**
			 * Transforms the node by the specfied transformation matrix. This
			 * transform is applied to the bind pose and any keyframes.
			 *
			 * @param transform Transformation matrix to apply.
			 */
			void Transform(const glm::mat4& transform);

			/**
			 * Adds a keyframe to the node.
			 *
			 * @param keyframe Shared pointer to the keyframe to add to the
			 * node.
			 */
			void AddKeyframe(std::shared_ptr<Keyframe> keyframe);

			/**
			 * Adds a mesh to the node.
			 *
			 * @param mesh Shared pointer to the mesh to add to the node.
			 */
			void AddMesh(std::shared_ptr<Mesh> mesh);

			/**
			 * Adds a child node to this node.
			 *
			 * @param node Shared pointer to the node to add as a child.
			 */
			void AddChildNode(std::shared_ptr<Node> node);

		private:
			/**
			 * Node name.
			 */
			std::string m_name;

			/**
			 * Node transformation matrix for the bind pose.
			 */
			glm::mat4 m_bindTransformationMatrix;

			/**
			 * Keyframes.
			 */
			std::vector<std::shared_ptr<Keyframe>> m_keyframes;

			/**
			 * Child nodes.
			 */
			 std::vector<std::shared_ptr<Node>> m_children;

			/**
			 * Meshes.
			 */
			std::vector<std::shared_ptr<Mesh>> m_meshes;
		};

		/**
		 * Constructor.
		 */
		Model();

		/**
		 * Destructor.
		 */
		virtual ~Model();

		/**
		 * Returns a shared pointer to the model's root node.
		 *
		 * @return Shared pointer to the root node.
		 */
		const std::shared_ptr<Node> GetRootNode() const;

		/**
		 * Returns the animation duration.
		 *
		 * @return Animation duration (in seconds).
		 */
		const double GetAnimationDuration() const;

		/**
		 * Loads the model from a file.
		 *
		 * @param filepath Path to the model file.
		 * @return True if the model was successfully loaded.
		 */
		bool LoadFromFile(std::string filepath);

		/**
		 * Transforms the model.
		 *
		 * @param translation Offset translation for the model.
		 * @param rotation Offset rotation for the model.
		 * @param scale Offset scale for the model.
		 */
		void Transform(const glm::vec3& translation, const glm::quat& rotation,
			const glm::vec3& scale);

	protected:
		/**
		 * Clears any previously loaded data, resetting the model.
		 */
		void Clear();

		/**
		 * Returns a shared pointer to the node with the specified name.
		 *
		 * @param name Name for the node to return.
		 * @return Shared pointer to the node with the specified name. A nullptr
		 * is returnd if no such node was found.
		 */
		std::shared_ptr<Node> FindNodeByName(std::string name);

		/**
		 * Loads all the materials.
		 *
		 * @param assimpScene Pointer to the ASSIMP scene in which the
		 * materials are contained.
		 * @param directoryPath Path to the directory where the model file is
		 * located.
		 */
		void LoadMaterials(const aiScene* assimpScene, std::string directoryPath);

		/**
		 * Loads all the meshes in the specified ASSIMP node.
		 *
		 * @param assimpNode Pointer to the ASSIMP node to load.
		 * @param assimpScene Pointer to the ASSIMP scene in which the node is
		 * contained.
		 * @parentNode Shared pointer to the parent node.
		 */
		void LoadNode(const aiNode* assimpNode, const aiScene* assimpScene, std::shared_ptr<Node> parentNode);

		/**
		 * Loads all the keyframes for all of the nodes in the model.
		 *
		 * @param assimpScene Pointer to the ASSIMP scene in which the nodes
		 * are contained.
		 */
		void LoadNodeKeyframes(const aiScene* assimpScene);

		/**
		 * Returns the local position for the node corresponding to the
		 * specified channel at the specified time.
		 *
		 * @param channel Pointer to the ASSIMP animation channel for
		 * the node.
		 * @param timeInTicks Time after the start of the animation,
		 * in ticks, for which to calculate the node's local position.
		 * @return Node's local position at the specified time.
		 */
		static glm::vec3 DetermineNodeLocalPositionAtTime(const aiNodeAnim* channel, double timeInTicks);

		/**
		 * Returns the local rotation for the node corresponding to the
		 * specified channel at the specified time.
		 *
		 * @param channel Pointer to the ASSIMP animation channel for
		 * the node.
		 * @param timeInTicks Time after the start of the animation,
		 * in ticks, for which to calculate the node's local rotation.
		 * @return Node's local rotation at the specified time.
		 */
		static glm::quat DetermineNodeLocalRotationAtTime(const aiNodeAnim* channel, double timeInTicks);

		/**
		 * Returns the local scale for the node corresponding to the
		 * specified channel at the specified time.
		 *
		 * @param channel Pointer to the ASSIMP animation channel for
		 * the node.
		 * @param timeInTicks Time after the start of the animation,
		 * in ticks, for which to calculate the node's local scale.
		 * @return Node's local scale at the specified time.
		 */
		static glm::vec3 DetermineNodeLocalScaleAtTime(const aiNodeAnim* channel, double timeInTicks);

		/**
		 * Converts an ASSIMP aiMatrix4x4 to a glm::mat4.
		 *
		 * @param assimpMatrix ASSIMP 4x4 matrix to convert.
		 * @return Corresponding glm::mat4.
		 */
		static glm::mat4 ConvertToMat4(const aiMatrix4x4& assimpMatrix);

		/**
		 * Converts an ASSIMP aiVector3D to a glm::vec3.
		 *
		 * @param assimpVector ASSIMP 3D vector to convert.
		 * @return Corresponding glm::vec3.
		 */
		static glm::vec3 ConvertToVec3(const aiVector3D& assimpVector);

		/**
		 * Converts an ASSIMP aiQuaternion to a glm::quat.
		 *
		 * @param assimpQuaternion ASSIMP quaternion to convert.
		 * @return Corresponding glm::quat.
		 */
		static glm::quat ConvertToQuat(const aiQuaternion& assimpQuaternion);

	private:
		/**
		 * Name of the model currently loaded.
		 */
		std::string m_name;

		/**
		 * Root node.
		 */
		std::shared_ptr<Node> m_rootNode;

		/**
		 * Materials.
		 */
		std::vector<std::shared_ptr<Material>> m_materials;

		/**
		 * Animation
		 */
		double m_animationDuration;
	};
}

#endif

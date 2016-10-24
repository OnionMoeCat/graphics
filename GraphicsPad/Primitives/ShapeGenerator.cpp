#include "ShapeGenerator.h"
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <Primitives\Vertex.h>
#define PI 3.14159265359
using glm::vec2;
using glm::vec3;
using glm::mat4;
using glm::mat3;
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

namespace {
	struct VertexWithoutTangent {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 normal;
		glm::vec2 uv;
	};
	Vertex* generateTangent(VertexWithoutTangent* vertex, const size_t numVertex, unsigned short* index, const size_t numIndex);
	glm::vec3 getTangent(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2,
		const glm::vec2& UV0, const glm::vec2& UV1, const glm::vec2& UV2);
}

glm::vec3 randomColor()
{
	glm::vec3 ret;
	ret.x = rand() / (float)RAND_MAX;
	ret.y = rand() / (float)RAND_MAX;
	ret.z = rand() / (float)RAND_MAX;
	return ret;
}

ShapeData ShapeGenerator::makeCube() {
	ShapeData ret;
	VertexWithoutTangent stackVertsWithoutTangent[] =
	{
		vec3(-1.0f, +1.0f, +1.0f), // 0
		vec3(+1.0f, +0.0f, +0.0f), // Color
		vec3(+0.0f, +1.0f, +0.0f), // Normal
		vec2(0.0f, 0.0f),
		vec3(+1.0f, +1.0f, +1.0f), // 1
		vec3(+0.0f, +1.0f, +0.0f), // Color
		vec3(+0.0f, +1.0f, +0.0f), // Normal
		vec2(1.0f, 0.0f),
		vec3(+1.0f, +1.0f, -1.0f), // 2
		vec3(+0.0f, +0.0f, +1.0f), // Color
		vec3(+0.0f, +1.0f, +0.0f), // Normal
		vec2(1.0f, 1.0f),
		vec3(-1.0f, +1.0f, -1.0f), // 3
		vec3(+1.0f, +1.0f, +1.0f), // Color
		vec3(+0.0f, +1.0f, +0.0f), // Normal
		vec2(0.0f, 1.0f),

		vec3(-1.0f, +1.0f, -1.0f), // 4
		vec3(+1.0f, +0.0f, +1.0f), // Color
		vec3(+0.0f, +0.0f, -1.0f), // Normal
		vec2(0.0f, 1.0f),
		vec3(+1.0f, +1.0f, -1.0f), // 5
		vec3(+0.0f, +0.5f, +0.2f), // Color
		vec3(+0.0f, +0.0f, -1.0f), // Normal
		vec2(1.0f, 1.0f),
		vec3(+1.0f, -1.0f, -1.0f), // 6
		vec3(+0.8f, +0.6f, +0.4f), // Color
		vec3(+0.0f, +0.0f, -1.0f), // Normal
		vec2(1.0f, 0.0f),
		vec3(-1.0f, -1.0f, -1.0f), // 7
		vec3(+0.3f, +1.0f, +0.5f), // Color
		vec3(+0.0f, +0.0f, -1.0f), // Normal
		vec2(0.0f, 0.0f),

		vec3(+1.0f, +1.0f, -1.0f), // 8
		vec3(+0.2f, +0.5f, +0.2f), // Color
		vec3(+1.0f, +0.0f, +0.0f), // Normal
		vec2(1.0f, 1.0f),
		vec3(+1.0f, +1.0f, +1.0f), // 9
		vec3(+0.9f, +0.3f, +0.7f), // Color
		vec3(+1.0f, +0.0f, +0.0f), // Normal
		vec2(0.0f, 1.0f),
		vec3(+1.0f, -1.0f, +1.0f), // 10
		vec3(+0.3f, +0.7f, +0.5f), // Color
		vec3(+1.0f, +0.0f, +0.0f), // Normal
		vec2(0.0f, 0.0f),
		vec3(+1.0f, -1.0f, -1.0f), // 11
		vec3(+0.5f, +0.7f, +0.5f), // Color
		vec3(+1.0f, +0.0f, +0.0f), // Normal
		vec2(1.0f, 0.0f),

		vec3(-1.0f, +1.0f, +1.0f), // 12
		vec3(+0.7f, +0.8f, +0.2f), // Color
		vec3(-1.0f, +0.0f, +0.0f), // Normal
		vec2(1.0f, 1.0f),
		vec3(-1.0f, +1.0f, -1.0f), // 13
		vec3(+0.5f, +0.7f, +0.3f), // Color
		vec3(-1.0f, +0.0f, +0.0f), // Normal
		vec2(0.0f, 1.0f),
		vec3(-1.0f, -1.0f, -1.0f), // 14
		vec3(+0.4f, +0.7f, +0.7f), // Color
		vec3(-1.0f, +0.0f, +0.0f), // Normal
		vec2(0.0f, 0.0f),
		vec3(-1.0f, -1.0f, +1.0f), // 15
		vec3(+0.2f, +0.5f, +1.0f), // Color
		vec3(-1.0f, +0.0f, +0.0f), // Normal
		vec2(1.0f, 0.0f),

		vec3(+1.0f, +1.0f, +1.0f), // 16
		vec3(+0.6f, +1.0f, +0.7f), // Color
		vec3(+0.0f, +0.0f, +1.0f), // Normal
		vec2(1.0f, 1.0f),
		vec3(-1.0f, +1.0f, +1.0f), // 17
		vec3(+0.6f, +0.4f, +0.8f), // Color
		vec3(+0.0f, +0.0f, +1.0f), // Normal
		vec2(0.0f, 1.0f),
		vec3(-1.0f, -1.0f, +1.0f), // 18
		vec3(+0.2f, +0.8f, +0.7f), // Color
		vec3(+0.0f, +0.0f, +1.0f), // Normal
		vec2(0.0f, 0.0f),
		vec3(+1.0f, -1.0f, +1.0f), // 19
		vec3(+0.2f, +0.7f, +1.0f), // Color
		vec3(+0.0f, +0.0f, +1.0f), // Normal
		vec2(1.0f, 0.0f),

		vec3(+1.0f, -1.0f, -1.0f), // 20
		vec3(+0.8f, +0.3f, +0.7f), // Color
		vec3(+0.0f, -1.0f, +0.0f), // Normal
		vec2(0.0f, 1.0f),
		vec3(-1.0f, -1.0f, -1.0f), // 21
		vec3(+0.8f, +0.9f, +0.5f), // Color
		vec3(+0.0f, -1.0f, +0.0f), // Normal
		vec2(1.0f, 1.0f),
		vec3(-1.0f, -1.0f, +1.0f), // 22
		vec3(+0.5f, +0.8f, +0.5f), // Color
		vec3(+0.0f, -1.0f, +0.0f), // Normal
		vec2(1.0f, 0.0f),
		vec3(+1.0f, -1.0f, +1.0f), // 23
		vec3(+0.9f, +1.0f, +0.2f), // Color
		vec3(+0.0f, -1.0f, +0.0f), // Normal
		vec2(0.0f, 0.0f),
	};

	unsigned short stackIndices[] = {
		0, 1, 2, 0, 2, 3, // Top
		4, 5, 6, 4, 6, 7, // Front
		8, 9, 10, 8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(stackVertsWithoutTangent);
	ret.numIndices = NUM_ARRAY_ELEMENTS(stackIndices);
	Vertex* stackVerts = generateTangent(stackVertsWithoutTangent, ret.numVertices, stackIndices, ret.numIndices);
	ret.vertices = stackVerts;

	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));

	return ret;
}

ShapeData ShapeGenerator::generateNormals(const ShapeData& data)
{
	ShapeData ret;
	ret.numVertices = data.numVertices * 2;
	ret.vertices = new Vertex[ret.numVertices];
	glm::vec3 white(1.0f, 1.0f, 1.0f);
	for (size_t i = 0; i < data.numVertices; i++)
	{
		uint vertIndex = i * 2;
		Vertex& v1 = ret.vertices[vertIndex];
		Vertex& v2 = ret.vertices[vertIndex + 1];
		const Vertex& sourceVertex = data.vertices[i];
		v1.position = sourceVertex.position;
		v2.position = sourceVertex.position + sourceVertex.normal;
		v1.color = v2.color = white;
	}

	ret.numIndices = ret.numVertices;
	ret.indices = new GLushort[ret.numIndices];
	for (GLushort i = 0; i < ret.numIndices; i++)
		ret.indices[i] = i;
	return ret;
}

namespace {
	Vertex* generateTangent(VertexWithoutTangent* vertex, const size_t numVertex, unsigned short* index,  const size_t numIndex) {
		Vertex* ret = new Vertex[numVertex];
		std::vector<size_t> nums(numVertex);
		for (size_t i = 0; i < numVertex; i++) {
			ret[i].position = vertex[i].position;
			ret[i].color = vertex[i].color;
			ret[i].normal = vertex[i].normal;
			ret[i].uv = vertex[i].uv;
			ret[i].tangent = glm::vec3();
		}
		for (size_t i = 0; i < numIndex; i += 3) {
			glm::vec3 tangent = getTangent(vertex[index[i]].position, vertex[index[i + 1]].position, vertex[index[i + 2]].position, vertex[index[i]].uv, vertex[index[i + 1]].uv, vertex[index[i + 2]].uv);
			ret[index[i]].tangent += tangent;
			ret[index[i + 1]].tangent += tangent;
			ret[index[i + 2]].tangent += tangent;
			nums[index[i]] ++;
			nums[index[i + 1]] ++;
			nums[index[i + 2]] ++;
		}
		for (size_t i = 0; i < numVertex; i++) {
			if (nums[i] > 0) {
				ret[i].tangent /= nums[i];
			}
		}
		return ret;
	}
	glm::vec3 getTangent(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2,
		const glm::vec2& UV0, const glm::vec2& UV1, const glm::vec2& UV2)
	{
		//let P = v1 - v0
		glm::vec3 P = P1 - P0;
		//let Q = v2 - v0
		glm::vec3 Q = P2 - P0;
		//using Eric Lengyel's approach with a few modifications
		//from Mathematics for 3D Game Programmming and Computer Graphics
		// want to be able to trasform a vector in Object Space to Tangent Space
		// such that the x-axis cooresponds to the 's' direction and the
		// y-axis corresponds to the 't' direction, and the z-axis corresponds
		// to <0,0,1>, straight up out of the texture map

		float s1 = UV1.x - UV0.x;
		float t1 = UV1.y - UV0.y;
		float s2 = UV2.x - UV0.x;
		float t2 = UV2.y - UV0.y;


		//we need to solve the equation
		// P = s1*T + t1*B
		// Q = s2*T + t2*B
		// for T and B


		//this is a linear system with six unknowns and six equatinos, for TxTyTz BxByBz
		//[px,py,pz] = [s1,t1] * [Tx,Ty,Tz]
		// qx,qy,qz     s2,t2     Bx,By,Bz

		//multiplying both sides by the inverse of the s,t matrix gives
		//[Tx,Ty,Tz] = 1/(s1t2-s2t1) *  [t2,-t1] * [px,py,pz]
		// Bx,By,Bz                      -s2,s1	    qx,qy,qz  

		//solve this for the unormalized T and B to get from tangent to object space

		glm::vec3 tangent = glm::normalize(glm::vec3(t2*P.x - t1*Q.x, t2*P.y - t1*Q.y, t2*P.z - t1*Q.z));

		return tangent;
	}
}

#pragma once

//void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
//	unsigned int vLength, unsigned int normalOffset)
//{
//	for (size_t i = 0; i < indiceCount; i += 3)
//	{
//		unsigned int in0 = indices[i] * vLength;
//		unsigned int in1 = indices[i + 1] * vLength;
//		unsigned int in2 = indices[i + 2] * vLength;
//		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
//		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
//		glm::vec3 normal = glm::cross(v1, v2);
//		normal = glm::normalize(normal);
//
//		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
//		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
//		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
//		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
//	}
//
//	for (size_t i = 0; i < verticeCount / vLength; i++)
//	{
//		unsigned int nOffset = i * vLength + normalOffset;
//		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
//		vec = glm::normalize(vec);
//		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
//	}
//}

constexpr float PlaneData[] = {
	-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	 // bottom left
	 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,	// bottom right
	 -1.0f, 1.0f, 0.0f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	// top left
	 1.0f,  1.0f, 0.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f	 // top right
};

constexpr unsigned int PlaneIndices[] = {
	0, 1, 2, // bottom left, bottom right, top right
	1, 3, 2 // bottom right, top right, top left
};

constexpr float CubeData[] = {
	// Front Face
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,		// bottom left
	 0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,		// bottom right
	 -0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,		//  top left
	 0.5f,  0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,		//  top right
												
	 // Back Face							
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,		// bottom left
	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,		0.0f, 0.0f, 0.0f,		// bottom right
	 -0.5f, 0.5f, -0.5f,	0.0f, 1.0f,		0.0f, 0.0f, 0.0f,		//  top left
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,		0.0f, 0.0f, 0.0f	//  top right
};

constexpr unsigned int CubeIndices[] = {
	// Front Face
	0, 1, 2,
	1, 3, 2,
	
	// Back Face
	4, 5, 6, //
	5, 7, 6,

	// Top Face
	2, 7, 6,
	2, 3, 7,

	// Bottom Face
	0, 5, 1,
	0, 4, 5,

	// Left Face
	4, 0, 6,
	0, 2, 6,

	// Right Face
	1 ,5, 7,
	1 ,7, 3
};
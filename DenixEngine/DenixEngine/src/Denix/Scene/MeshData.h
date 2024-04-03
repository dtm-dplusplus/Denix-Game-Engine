#pragma once

inline void CalculateNormals(const unsigned int* _indices, const unsigned int _indicesCount, float* _vertices, const unsigned int _verticeCount, const unsigned int _vertexLength, const unsigned int _nornmalOffset)
{
	for (size_t i = 0; i < _indicesCount; i += 3)
	{
		unsigned int i0 = _indices[i] * _vertexLength;
		unsigned int i1 = _indices[i + 1] * _vertexLength;
		unsigned int i2 = _indices[i + 2] * _vertexLength;
		glm::vec3 v1(_vertices[i1] - _vertices[i0], _vertices[i1 + 1] - _vertices[i0 + 1], _vertices[i1 + 2] - _vertices[i0 + 2]);
		glm::vec3 v2(_vertices[i2] - _vertices[i0], _vertices[i2 + 1] - _vertices[i0 + 1], _vertices[i2 + 2] - _vertices[i0 + 2]);
		const glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		i0 += _nornmalOffset;
		i1 += _nornmalOffset;
		i2 += _nornmalOffset;

		_vertices[i0] += normal.x;
		_vertices[i0 + 1] += normal.y;
		_vertices[i0 + 2] += normal.z;

		_vertices[i1] += normal.x;
		_vertices[i1 + 1] += normal.y;
		_vertices[i1 + 2] += normal.z;

		_vertices[i2] += normal.x;
		_vertices[i2 + 1] += normal.y;
		_vertices[i2 + 2] += normal.z;
	}

	for (size_t i = 0; i < _verticeCount; i += _vertexLength)
	{
		glm::vec3 normal(_vertices[i + _nornmalOffset], _vertices[i + _nornmalOffset + 1], _vertices[i + _nornmalOffset + 2]);
		normal = glm::normalize(normal);

		_vertices[i + _nornmalOffset] = normal.x;
		_vertices[i + _nornmalOffset + 1] = normal.y;
		_vertices[i + _nornmalOffset + 2] = normal.z;
	}
}

constexpr float TexPlaneData[] = {
	-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		 // bottom left
	 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		// bottom right
	 -1.0f, 1.0f, 0.0f,		0.0f, 1.0f,		//  top left
	 1.0f,  1.0f, 0.0f,		1.0f, 1.0f		 //  top right
};

constexpr unsigned int PlaneIndices[] = {
	0, 1, 2, // bottom left, bottom right, top right
	1, 3, 2 // bottom right, top right, top left
};

constexpr float CubeData[] = {
	// Front Face
	-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,		// bottom left
	 0.5f, -0.5f, 0.5f,  1.0f, 0.0f,		// bottom right
	 -0.5f, 0.5f, 0.5f,  0.0f, 1.0f,		//  top left
	 0.5f,  0.5f, 0.5f,  1.0f, 1.0f,		//  top right

	 // Back Face
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		// bottom left
	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,		// bottom right
	 -0.5f, 0.5f, -0.5f,	0.0f, 1.0f,		//  top left
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f		//  top right
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
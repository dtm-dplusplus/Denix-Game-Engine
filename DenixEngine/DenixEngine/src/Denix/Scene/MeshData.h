#pragma once

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
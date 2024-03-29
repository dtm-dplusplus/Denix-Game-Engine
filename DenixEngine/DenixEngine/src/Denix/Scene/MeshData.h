#pragma once

// Raw data for primitive shapes
//  Ensure shape definitions are anti-clockwise


constexpr float EqualTriangleData[] = {
	-0.5f, -0.5f, 0.0f, // bottom left
	 0.5f, -0.5f, 0.0f, // bottom right
	 0.0f,  0.5f, 0.0f //  top
};

constexpr unsigned int EqualTriangleIndices[] = {
	0, 1, 2 // left, right, top
};

constexpr float RightTriangleData[] = {
	-0.5f, -0.5f, 0.0f, // bottom left
	 0.5f, -0.5f, 0.0f, // bottom right
	 -0.5f,  0.5f, 0.0f //  top (left)
};

constexpr unsigned int RightTriangleIndices[] = {
	0, 1, 2 // left, right, top (left)
};

constexpr float PlaneData[] = {
	-1.0f, -1.0f, 0.0f,	 // bottom left
	 1.0f, -1.0f, 0.0f,	// bottom right
	 -1.0f, 1.0f, 0.0f,	//  top left
	 1.0f,  1.0f, 0.0f	 //  top right
};

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
	-0.5f, -0.5f, 0.5f, // bottom left
	 0.5f, -0.5f, 0.5f, // bottom right
	 -0.5f,  0.5f, 0.5f, //  top left
	 0.5f,  0.5f, 0.5f, //  top right

	 // Back Face
	-0.5f, -0.5f, -0.5f, // bottom left
	 0.5f, -0.5f, -0.5f, // bottom right
	 -0.5f,  0.5f, -0.5f, //  top left
	 0.5f,  0.5f, -0.5f //  top right
};

constexpr unsigned int CubeIndices[] = {
	// Front Face
	0, 1, 2, // bottom left, bottom right, top right
	1, 3, 2, // bottom left, top right, top left
	
	// Back Face
	4,5,6, //
	5,7,6,

	// Top Face
	2, 7, 6,
	2, 3, 7,

	// Bottom Face
	0,5,1,
	0,4,5,

	// Left Face
	4,0,6,
	0,2,6,

	// Right Face
	1,5,7,
	1,7,3
};

//constexpr float CubeData[]{
//	  -1.0f,-1.0f,-1.0f, // triangle 1 : begin
//	-1.0f,-1.0f, 1.0f,
//	-1.0f, 1.0f, 1.0f, // triangle 1 : end
//	1.0f, 1.0f,-1.0f, // triangle 2 : begin
//	-1.0f,-1.0f,-1.0f,
//	-1.0f, 1.0f,-1.0f, // triangle 2 : end
//	1.0f,-1.0f, 1.0f,
//	-1.0f,-1.0f,-1.0f,
//	1.0f,-1.0f,-1.0f,
//	1.0f, 1.0f,-1.0f,
//	1.0f,-1.0f,-1.0f,
//	-1.0f,-1.0f,-1.0f,
//	-1.0f,-1.0f,-1.0f,
//	-1.0f, 1.0f, 1.0f,
//	-1.0f, 1.0f,-1.0f,
//	1.0f,-1.0f, 1.0f,
//	-1.0f,-1.0f, 1.0f,
//	-1.0f,-1.0f,-1.0f,
//	-1.0f, 1.0f, 1.0f,
//	-1.0f,-1.0f, 1.0f,
//	1.0f,-1.0f, 1.0f,
//	1.0f, 1.0f, 1.0f,
//	1.0f,-1.0f,-1.0f,
//	1.0f, 1.0f,-1.0f,
//	1.0f,-1.0f,-1.0f,
//	1.0f, 1.0f, 1.0f,
//	1.0f,-1.0f, 1.0f,
//	1.0f, 1.0f, 1.0f,
//	1.0f, 1.0f,-1.0f,
//	-1.0f, 1.0f,-1.0f,
//	1.0f, 1.0f, 1.0f,
//	-1.0f, 1.0f,-1.0f,
//	-1.0f, 1.0f, 1.0f,
//	1.0f, 1.0f, 1.0f,
//	-1.0f, 1.0f, 1.0f,
//	1.0f,-1.0f, 1.0f
//};
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
	-0.5f, -0.5f, 0.0f, // bottom left
	 0.5f, -0.5f, 0.0f, // bottom right
	 0.5f,  0.5f, 0.0f, //  top right
	 -0.5f,  0.5f, 0.0f //  top left
};

constexpr unsigned int PlaneIndices[] = {
	0, 1, 2, // bottom left, bottom right, top right
	0, 2, 3 // bottom left, top right, top left
};

constexpr float CubeData[] = {
	-0.5f, -0.5f, 0.0f, // Tri 1 bottom left
	 0.5f, -0.5f, 0.0f, // Tri 1 bottom right
	 0.5f,  0.5f, 0.0f, // Tri 1 top right
	 -0.5f,  0.5f, 0.0f, // Tri 1 top left


};

constexpr unsigned int CubeIndices[] = {
	0, 1, 2, // bottom left, bottom right, top right
	0, 2, 3 // bottom left, top right, top left
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
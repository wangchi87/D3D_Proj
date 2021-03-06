//***************************************************************************************
// GeometryGenerator.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines a static class for procedurally generating the geometry of 
// common mathematical objects.
//
// All triangles are generated "outward" facing.  If you want "inward" 
// facing triangles (for example, if you want to place the camera inside
// a sphere to simulate a sky), you will need to:
//   1. Change the Direct3D cull mode or manually reverse the winding order.
//   2. Invert the normal.
//   3. Update the texture coordinates and tangent vectors.
//***************************************************************************************



// ****************** modified by Chi Wang for Netease -1 homework *******************


#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H

#include "ProjectHeader.h"

#include "tinyply\tinyply.h"
using namespace tinyply;


inline void ScaleMesh ( MeshData &md, float scaleFactor )
{
	for (UINT i = 0; i < md.Vertices.size (); i++)
	{
		md.Vertices[ i ].Position.x *= scaleFactor;
		md.Vertices[ i ].Position.y *= scaleFactor;
		md.Vertices[ i ].Position.z *= scaleFactor;
	}
}

class GeometryGenerator
{
public:

	//Creates a box centered at the origin with the given dimensions.
	void CreateBox(float width, float height, float depth, MeshData& meshData);

	// Creates a sphere centered at the origin with the given radius.  The
	//slices and stacks parameters control the degree of tessellation.
	void CreateSphere(float radius, WORD sliceCount, WORD stackCount, MeshData& meshData);

	// Creates a geosphere centered at the origin with the given radius.  The
	// depth controls the level of tessellation.
	void CreateGeosphere(float radius, WORD numSubdivisions, MeshData& meshData);

	// Creates a cylinder parallel to the y-axis, and centered about the origin.  
	// The bottom and top radius can vary to form various cone shapes rather than true
	// cylinders.  The slices and stacks parameters control the degree of tessellation.
	void CreateCylinder(float bottomRadius, float topRadius, float height, WORD sliceCount, WORD stackCount, MeshData& meshData);

	// Creates an mxn grid in the xz-plane with m rows and n columns, centered
	// at the origin with the specified width and depth.
	void CreateGrid(float width, float depth, WORD m, WORD n, MeshData& meshData);

	// Creates a quad covering the screen in NDC coordinates.  This is useful for
	// postprocessing effects.
	void CreateFullscreenQuad(MeshData& meshData);

	// load a mesh from external PLY file
	void CreateMeshFromPLY ( LPCWSTR fileName , MeshData& meshData );

	~GeometryGenerator ()
	{
		
	}
private:
	void Subdivide(MeshData& meshData);
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, WORD sliceCount, WORD stackCount, MeshData& meshData);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, WORD sliceCount, WORD stackCount, MeshData& meshData);
};

inline float AngleFromXY ( float x , float y )
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf ( y / x ); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f*XM_PI; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf ( y / x ) + XM_PI; // in [0, 2*pi).

	return theta;
}

#endif // GEOMETRYGENERATOR_H
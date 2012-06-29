#include "QTTerrain.h"
#include <SEScene/Frustum.h>
#include <SEScene/QuadTree.h>
//#include "Terrain/s.h"
#include <DX/DX9.h>
#include <SEMath/Vector4.h>
#include <SEMath/Matrix4x4.h>
static	int		flushTextures = 1;
QTTerrain::QTTerrain(void)
{
}


QTTerrain::~QTTerrain(void)
{
}
//‰÷»æµÿ–Œ
void QTTerrain:: render()
{
	// take another surface description ?
	int surfaceSelect = 0;
	int  qualitySelect = 5;
	static int lastSurfaceType = 0;
	static int lastQuality     = 5;
	
	if ( lastSurfaceType != surfaceSelect || lastQuality != qualitySelect )
	{
		lastSurfaceType = surfaceSelect;
		lastQuality     = qualitySelect;
		flushTextures = 1;
		//createSurfaceNodes();
		//rootSurfaceNode = surfaceTypes[ surfaceSelect ];
	}
	

}
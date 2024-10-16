#include "cgCube.h"
#include <math.h>
#include <GL/glew.h>//OpenGL库

cgCube::cgCube(void)
{
}


cgCube::~cgCube(void)
{
}

void cgCube::SetPos(cgPoint3D pos)
{
	ptPos = pos;
}

void cgCube::InitData(float length)
{
	fLength = length;

	vTris.clear();
	float l = fLength/2;
	float pt[8][3];
	float nor[8][3];
	float text[8][2];

	pt[0][0] = -l;pt[0][1] = -l;pt[0][2] = l;	
	pt[1][0] =  l;pt[1][1] = -l;pt[1][2] = l;
	pt[2][0] =  l;pt[2][1] =  l;pt[2][2] = l;
	pt[3][0] = -l;pt[3][1] =  l;pt[3][2] = l;

	pt[4][0] = -l;pt[4][1] = -l;pt[4][2] = -l;
	pt[5][0] =  l;pt[5][1] = -l;pt[5][2] = -l;
	pt[6][0] =  l;pt[6][1] =  l;pt[6][2] = -l;
	pt[7][0] = -l;pt[7][1] =  l;pt[7][2] = -l;

	//构建三角形
	//前面
	cgTriangle tri;
	tri.Pt[0] = cgPoint3D(pt[0][0],pt[0][1],pt[0][2]);
	tri.Pt[1] = cgPoint3D(pt[1][0],pt[1][1],pt[1][2]);
	tri.Pt[2] = cgPoint3D(pt[3][0],pt[3][1],pt[3][2]);
	vTris.push_back(tri);

	tri.Pt[0] = cgPoint3D(pt[1][0],pt[1][1],pt[1][2]);
	tri.Pt[1] = cgPoint3D(pt[2][0],pt[2][1],pt[2][2]);
	tri.Pt[2] = cgPoint3D(pt[3][0],pt[3][1],pt[3][2]);
	vTris.push_back(tri);

	//后面
	tri.Pt[0] = cgPoint3D(pt[4][4],pt[4][1],pt[4][2]);
	tri.Pt[1] = cgPoint3D(pt[5][0],pt[5][1],pt[5][2]);
	tri.Pt[2] = cgPoint3D(pt[6][0],pt[6][1],pt[6][2]);
	vTris.push_back(tri);

	tri.Pt[0] = cgPoint3D(pt[5][0],pt[5][1],pt[5][2]);
	tri.Pt[1] = cgPoint3D(pt[6][0],pt[6][1],pt[6][2]);
	tri.Pt[2] = cgPoint3D(pt[7][0],pt[7][1],pt[7][2]);
	vTris.push_back(tri);

	//上面
	tri.Pt[0] = cgPoint3D(pt[3][4],pt[3][1],pt[3][2]);
	tri.Pt[1] = cgPoint3D(pt[2][0],pt[2][1],pt[2][2]);
	tri.Pt[2] = cgPoint3D(pt[7][0],pt[7][1],pt[7][2]);
	vTris.push_back(tri);

	tri.Pt[0] = cgPoint3D(pt[2][0],pt[2][1],pt[2][2]);
	tri.Pt[1] = cgPoint3D(pt[6][0],pt[6][1],pt[6][2]);
	tri.Pt[2] = cgPoint3D(pt[7][0],pt[7][1],pt[7][2]);
	vTris.push_back(tri);

	//底面
	tri.Pt[0] = cgPoint3D(pt[0][4],pt[0][1],pt[0][2]);
	tri.Pt[1] = cgPoint3D(pt[1][0],pt[1][1],pt[1][2]);
	tri.Pt[2] = cgPoint3D(pt[4][0],pt[4][1],pt[4][2]);
	vTris.push_back(tri);

	tri.Pt[0] = cgPoint3D(pt[1][0],pt[1][1],pt[1][2]);
	tri.Pt[1] = cgPoint3D(pt[5][0],pt[5][1],pt[5][2]);
	tri.Pt[2] = cgPoint3D(pt[4][0],pt[4][1],pt[4][2]);
	vTris.push_back(tri);

	//左面
	tri.Pt[0] = cgPoint3D(pt[4][4],pt[4][1],pt[4][2]);
	tri.Pt[1] = cgPoint3D(pt[0][0],pt[0][1],pt[0][2]);
	tri.Pt[2] = cgPoint3D(pt[7][0],pt[7][1],pt[7][2]);
	vTris.push_back(tri);

	tri.Pt[0] = cgPoint3D(pt[0][0],pt[0][1],pt[0][2]);
	tri.Pt[1] = cgPoint3D(pt[3][0],pt[3][1],pt[3][2]);
	tri.Pt[2] = cgPoint3D(pt[7][0],pt[7][1],pt[7][2]);
	vTris.push_back(tri);

	//右面
	tri.Pt[0] = cgPoint3D(pt[1][4],pt[1][1],pt[1][2]);
	tri.Pt[1] = cgPoint3D(pt[5][0],pt[5][1],pt[5][2]);
	tri.Pt[2] = cgPoint3D(pt[2][0],pt[2][1],pt[2][2]);
	vTris.push_back(tri);

	tri.Pt[0] = cgPoint3D(pt[5][0],pt[5][1],pt[5][2]);
	tri.Pt[1] = cgPoint3D(pt[6][0],pt[6][1],pt[6][2]);
	tri.Pt[2] = cgPoint3D(pt[2][0],pt[2][1],pt[2][2]);
	vTris.push_back(tri);
}

void cgCube::Render()
{
	vector<cgTriangle>::iterator it = vTris.begin();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(ptPos.x,ptPos.y,ptPos.z);
	for(; it != vTris.end(); it++)
	{
	   (*it).Render();
	}

	glPopMatrix();
}

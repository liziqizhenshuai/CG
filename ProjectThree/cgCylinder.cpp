#include "cgCylinder.h"
#include <math.h>
#include <GL/glew.h>//OpenGL库

cgCylinder::cgCylinder(void)
{
}


cgCylinder::~cgCylinder(void)
{
}

void cgCylinder::SetPos(cgPoint3D pos)
{
	ptPos = pos;
}

void cgCylinder::InitData(float r,float h,float angle)
{
	fR = r;
	fH = h;

	vTris.clear();

	//生成柱面三角网
    int m = 20;//水平方向圆离散化多边形边数
	
	float dalpha = 2.0f*PI/m;

	float z0 = -h/2;
	float z1 =  h/2;

	float alpha = 0.0f;
	for (int j=0; j<m; j++)
	{
		float x01 = fR*cos(alpha);
		float y01 = fR*sin(alpha);

		float x02 = fR*cos(alpha+dalpha);
		float y02 = fR*sin(alpha+dalpha);

		//构建三角形
		//柱面
		
		cgTriangle tri;
		tri.Pt[0] = cgPoint3D(x01,z0,y01);
		tri.Pt[1] = cgPoint3D(x02,z0,y02);
		tri.Pt[2] = cgPoint3D(x01,z1,y01);
		vTris.push_back(tri);

		tri.Pt[0] = cgPoint3D(x02,z0,y02);
		tri.Pt[1] = cgPoint3D(x02,z1,y02);
		tri.Pt[2] = cgPoint3D(x01,z1,y01);
		vTris.push_back(tri);

			
		//底面
		

		alpha += dalpha;
	}
}

void cgCylinder::Render()
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
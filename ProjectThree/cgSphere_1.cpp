#include "cgSphere.h"
#include <math.h>
#include <GL/glew.h>//OpenGL��

cgSphere::cgSphere(void)
{
	ptPos = cgPoint3D();
}


cgSphere::~cgSphere(void)
{
}

void cgSphere::SetPos(cgPoint3D pos)
{
	ptPos = pos;
}

void cgSphere::InitData(float r)
{
	fR = r;

	vTris.clear();
	//��������������ԭ�������������
    int m = 20;//ˮƽ����Բ��ɢ������α���
	int n = 20;//��ֱ������Ƭ����

	float dalpha = 2*PI/m;
	float dbeta  = PI/n;

	float beta = -PI/2.0f;
	for (int i=0; i<n;i++)
	{
		float alpha = 0.0f;

		float z1 = fR*sin(beta);
		float z2 = fR*sin(beta+dbeta);

		float r0 = fR*cos(beta);
		float r1 = fR*cos(beta+dbeta);

		beta += dbeta;
		for (int j=0; j<m; j++)
		{
			float x01 = r0*cos(alpha);
			float y01 = r0*sin(alpha);

			float x02 = r0*cos(alpha+dalpha);
			float y02 = r0*sin(alpha+dalpha);

			float x11 = r1*cos(alpha);
			float y11 = r1*sin(alpha);

			float x12 = r1*cos(alpha+dalpha);
			float y12 = r1*sin(alpha+dalpha);

			//����������
			cgTriangle tri;
			tri.Pt[0] = cgPoint3D(x01,y01,z1);
			tri.Pt[1] = cgPoint3D(x02,y02,z1);
			tri.Pt[2] = cgPoint3D(x11,y11,z2);
			vTris.push_back(tri);

			tri.Pt[0] = cgPoint3D(x02,y02,z1);
			tri.Pt[1] = cgPoint3D(x12,y12,z2);
			tri.Pt[2] = cgPoint3D(x11,y11,z2);
			vTris.push_back(tri);

			alpha += dalpha;
		}
	}
}

void cgSphere::Render()
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



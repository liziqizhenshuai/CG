#pragma once
#include "cgPoint3D.h"
#include "cgTriangle.h"
#include<vector>
using namespace std;

class cgCylinder
{
public:
	cgCylinder(void);
	~cgCylinder(void);

	void InitData(float r,float h,float angle);
	void Render();
	void SetPos(cgPoint3D pos);	

private:
	float fR;
	float fH;

	cgPoint3D ptPos;
	vector<cgTriangle> vTris;
};


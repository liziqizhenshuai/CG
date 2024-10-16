#pragma once
#include "cgPoint3D.h"
#include "cgTriangle.h"
#include<vector>
using namespace std;

class cgCube
{
public:
	cgCube(void);
	~cgCube(void);

	void InitData(float length);
	void Render();
	void SetPos(cgPoint3D pos);	

private:
	float fLength;
	cgPoint3D ptPos;
	vector<cgTriangle> vTris;
};


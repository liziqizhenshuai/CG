// OpenGLOld.cpp : 定义控制台应用程序的入口点。
//
#include <GL/glew.h>//OpenGL库
#include <GL/glut.h>//OpenGL辅助库
#include <math.h>
#include "SOIL/SOIL.h"

#include "cgSphere.h"
#include "cgCylinder.h"
#include "cgCube.h"

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) //这句是不让控制台窗体出现，如果想要出现，去掉即可。

float alpha=0.0f;
float pos[]={3,2.0,5};
float headdir[]={0.0f,0.0f,-1.0f};
float rightdir[]={0.1f,0.0f,0.0f};

float step = 0.10f;
float beta = 180.0f;//与z轴正向夹角
float betastep = 0.5f;

cgSphere sphere;
cgCylinder cylinder;
cgCube cube;

int rendermode = 0;//0:填充； 1:线框；2：纹理

GLuint textureID;

GLuint LoadTexture(char* fname)
{
	GLuint tID;
	
	int width, height, nrComponents;
	unsigned char *data = SOIL_load_image(fname, &width, &height, &nrComponents,SOIL_LOAD_RGB);// 
	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;
		bool gammaCorrection = false;
		if (nrComponents == 1)
		{
			internalFormat = dataFormat = GL_RED;
		}
		else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		//生成纹理
		glGenTextures(1, &tID);
		glBindTexture(GL_TEXTURE_2D, tID);
	
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//这个函数居然不能用
		gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			int s=0;
		}

		SOIL_free_image_data(data);

		return tID;
	}

	return -1;	
}

void init(void)
{
	glClearColor(0.5, 1.0, 1.0, 1.0);//设置背景色白色
	glEnable(GL_DEPTH_TEST);

	textureID = LoadTexture("Textures/timg.jpg");	
	sphere.InitData(1.0f);
	sphere.SetPos(cgPoint3D(-1.0f,2.0f,1.0f));

	cylinder.InitData(1.0f,2.0f);
	cylinder.SetPos(cgPoint3D(5.0f,1.0f,2.0f));
	cube.InitData(1.0f);
	cube.SetPos(cgPoint3D(-1.0f,1.0f,5.0f));
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );//清除帧缓存和深度缓存
	
	//模型观察矩阵初始化
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float at[3];
	for (int i=0; i<3; i++)
		at[i] = pos[i] + headdir[i];

	gluLookAt (pos[0], pos[1], pos[2], at[0], at[1], at[2], 0.0, 1.0, 0.0);

	if (rendermode==1)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	//地面
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-50.0f,0.0f,50.0f);//指定顶点坐标
		glTexCoord2f(10.0f,0.0f);
		glVertex3f(50.0f,0.0f,50.0f);
		glTexCoord2f(10.0f,10.0f);
		glVertex3f(50.0f,0.0f,-50.0f);
		glTexCoord2f(0.0f,10.0f);
		glVertex3f(-50.0f,0.0f,-50.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
/*
	//三角形绕第3个顶点旋转
	glPushMatrix();
	glTranslatef(3.f,2.f,0.0f);
	glRotatef(alpha,0,0,1);
	glTranslatef(-3.f,-2.f,0.0f);
*/
/*	glBegin(GL_TRIANGLES);

		glColor3f(1.0f,0.0f,0.0f);//指定顶点颜色
		glVertex2f(1.0f,1.0f);//指定顶点坐标
		glColor3f(0.0f,1.0f,0.0f);
		glVertex2f(5.0f,1.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex2f(3.0f,4.0f);
	glEnd();
*/
	glPopMatrix();

	glColor3f(0.25,0.5,0.5);
	sphere.Render();

	glColor3f(0.0,0.0,1.0);
	cylinder.Render();

	glColor3f(1.0,0.0,0.0);
	cube.Render();
	
	glutSwapBuffers();//交换缓冲（双缓冲时使用）
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);//设置视区（窗口内绘图区域）

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(60,(GLfloat) width/(GLfloat)height, 1.0, 200.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void change()
{
	alpha += 2.f;

	if (alpha>360) alpha -= 360;

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'W':   //上移
		case 'w':
			for (int i=0; i<3; i++)
				pos[i] += step*headdir[i];
			break;
		case 'S':   //下移
		case 's':
			for (int i=0; i<3; i++)
				pos[i] -= step*headdir[i];
			break;
		case 'A':  //左移
		case 'a':
			for (int i=0; i<3; i++)
				pos[i] -= step*rightdir[i];	
			break;
		case 'D':   //右移
		case 'd':
			for (int i=0; i<3; i++)
				pos[i] += step*rightdir[i];
			break;

		case 'Z':   
		case 'z':
			//抬高相机
	         pos[1] += .30f;
			break;

		case 'X':   
		case 'x':
			//降低相机
	         pos[1] -= .30f;
			break;

		case 'T':   //修改绘制模式
		case 't':
			rendermode = (++rendermode)%2;
			break;	
	}

	glutPostRedisplay();
}

void SpecialKey(GLint key,GLint x,GLint y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			for (int i=0; i<3; i++)
				pos[i] += step*headdir[i];
			break;

		case GLUT_KEY_DOWN:
			for (int i=0; i<3; i++)
				pos[i] -= step*headdir[i];
			break;

		case GLUT_KEY_LEFT:
			//修改前进方向
			beta += betastep;
			headdir[0] = sin(beta/180*3.14);
			headdir[2] = cos(beta/180*3.14);
			headdir[1] = 0;

			//修改右向方向
			rightdir[0] = -cos(beta/180*3.14);
			rightdir[2] = sin(beta/180*3.14);
			rightdir[1] = 0;
			break;

		case GLUT_KEY_RIGHT:
			//修改前进方向
			beta -= betastep;
			headdir[0] = sin(beta/180*3.14);
			headdir[2] = cos(beta/180*3.14);
			headdir[1] = 0;

			//修改右向方向
			rightdir[0] = -cos(beta/180*3.14);;
			rightdir[2] = sin(beta/180*3.14);;
			rightdir[1] = 0;
			break;
	}

	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInitWindowPosition(200, 200);//应用程序窗口位置
	glutInitWindowSize(800, 600);//窗口大小
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE| GLUT_DEPTH );//双缓冲，单缓冲为GLUT_SINGLE，深度缓存
	glutCreateWindow("Project Three");//创建窗口，参数为窗口标题
	init();

	glutDisplayFunc(display);//图形绘制
	glutReshapeFunc(reshape);//窗口大小变化
	glutKeyboardFunc(keyboard);//键盘交互
	glutSpecialFunc(&SpecialKey);//方向键
	glutIdleFunc(change);//空闲时间执行

	glutMainLoop();//必须，进入GLUT事件处理循环

	return 0;
}

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
//光照变量
GLfloat light_position[] = { 0.0, 0.0, 0.0, 1 };
GLfloat light_color[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat aMaterial[] = { .20, 0.20, 0.20, 1.0 };//环境光反射系数
GLfloat dMaterial[] = { .60, 0.60, 0.60, 1.0 };//漫反射光反射系数
GLfloat sMaterial[] = { 0.4, 0.4,0.4, 1 };//镜面反射光反射系数
GLfloat shiniess = 20;//高光指数

float alpha=0.0f;
float pos[]={3,2.0,5};
float headdir[]={0.0f,0.0f,-1.0f};
float rightdir[]={0.1f,0.0f,0.0f};
float t = 0;
float xt=0;
float step = 0.10f;
float beta = 180.0f;//与z轴正向夹角
float betastep = 0.5f;

cgSphere sphere;
cgCylinder cylinder;
cgCube cube;

int rendermode = 0;//0:填充； 1:线框；2：纹理

bool flag = 1;
float jia=0;
float fudong = 2.6;
float xfudong = 0.001;
float fudongde = 2.6;

GLuint texture1;
GLuint texture2;
GLuint texture3;
GLuint texture4;
GLuint texture5;
GLuint texture6;
GLuint texture7;
GLfloat s[] = { 1, 0, 1 ,1 };
GLfloat diffuse_color[] = { 1,1,1,0.5 };
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
	
	
	
	texture1 = LoadTexture("Textures/gezi.BMP");	
	texture2 = LoadTexture("Textures/desert.bmp");
	texture3 = LoadTexture("Textures/mutou.bmp");
	texture4 = LoadTexture("Textures/huo.bmp");
	texture5 = LoadTexture("Textures/hui1.bmp");
	texture6 = LoadTexture("Textures/gezi.BMP");
	texture7 = LoadTexture("Textures/wu.bmp");


	sphere.InitData(1.0f);
	sphere.SetPos(cgPoint3D(0.0f,3.0f,0.0f));

	

	
}
void createsphere(int te,float r)
{
	
	float fR;
	vector<cgTriangle> vTris;
	
	if (te == 1)
	{
		fR = r + jia;
	}
	else
	{
		fR = r;
	}
	vTris.clear();
	//生成球心在坐标原点的球面三角网
	int m = 20;//水平方向圆离散化多边形边数
	int n = 20;//垂直方向切片个数

	float dalpha = 2 * PI / m;
	float dbeta = PI / n;

	float beta = -PI / 2.0f;
	for (int i = 0; i < n; i++)
	{

		float alpha = 0;

		float z1 = fR * sin(beta);
		float z2 = fR * sin(beta + dbeta);

		float r0 = fR * cos(beta);
		float r1 = fR * cos(beta + dbeta);

		if (te == 1)
		{
			beta += dbeta + jia;
		}
		else
		{
			beta += dbeta ;
		}
		
		for (int j = 0; j < m; j++)
		{
			float x01 = r0 * cos(alpha);
			float y01 = r0 * sin(alpha);

			float x02 = r0 * cos(alpha + dalpha);
			float y02 = r0 * sin(alpha + dalpha);

			float x11 = r1 * cos(alpha);
			float y11 = r1 * sin(alpha);

			float x12 = r1 * cos(alpha + dalpha);
			float y12 = r1 * sin(alpha + dalpha);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_color);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // 这是默认的纹理环境
			
				glBegin(GL_TRIANGLES);
				glTexCoord2f(alpha / 2 * PI, beta / 2 * PI);
				glVertex3f(x01, y01, z1);//指定顶点坐标
				glNormal3f(x01, y01, z1);
				glTexCoord2f((alpha + dalpha)/2*PI, beta / 2 * PI);
				glVertex3f(x02, y02, z1);
				glNormal3f(x02, y02, z1);
				glTexCoord2f(alpha / 2 * PI, (beta + dbeta) / 2 * PI);
				glVertex3f(x11, y11, z2);
				glNormal3f(x11, y11, z2);
				glTexCoord2f((alpha + dalpha )/ 2 * PI, beta / 2 * PI);
				glVertex3f(x02, y02, z1);//指定顶点坐标
				glNormal3f(x02, y02, z1);
				glTexCoord2f((alpha + dalpha) / 2 * PI, (beta + dbeta) / 2 * PI);
				glVertex3f(x12, y12, z2);
				glNormal3f(x12, y12, z2);
				glTexCoord2f(alpha / 2 * PI, (beta + dbeta) / 2 * PI);
				glVertex3f(x11, y11, z2);
				glNormal3f(x11, y11, z2);
				glEnd();
			glDisable(GL_TEXTURE_2D);


			alpha += dalpha;
		}
	}
}

void createtai()
{

	float fR;
	vector<cgTriangle> vTris;
	fR = 1.5 ;
	vTris.clear();
	//生成球心在坐标原点的球面三角网
	int m = 20;//水平方向圆离散化多边形边数
	int n = 20;//垂直方向切片个数

	float dalpha = 2 * PI / m;
	float dbeta = PI / n;

	float beta = -PI / 2.0f;
	for (int i = 0; i < n/2; i++)
	{

		float alpha = 0;

		float z1 = fR * sin(beta);
		float z2 = fR * sin(beta + dbeta);

		float r0 = fR * cos(beta);
		float r1 = fR * cos(beta + dbeta);

		beta += dbeta ;
		for (int j = 0; j < m; j++)
		{
			float x01 = r0 * cos(alpha);
			float y01 = r0 * sin(alpha);

			float x02 = r0 * cos(alpha + dalpha);
			float y02 = r0 * sin(alpha + dalpha);

			float x11 = r1 * cos(alpha);
			float y11 = r1 * sin(alpha);

			float x12 = r1 * cos(alpha + dalpha);
			float y12 = r1 * sin(alpha + dalpha);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture4);
			glColor3f(1.0f, 1.0f, 1.0f);
			glBegin(GL_TRIANGLES);
			glTexCoord2f(alpha, beta);
			glVertex3f(x01, y01, z1);//指定顶点坐标
			glTexCoord2f(alpha + dalpha, beta);
			glVertex3f(x02, y02, z1);
			glTexCoord2f(alpha, beta + dbeta);
			glVertex3f(x11, y11, z2);
			glTexCoord2f(alpha + dalpha, beta);
			glVertex3f(x02, y02, z1);//指定顶点坐标
			glTexCoord2f(alpha + dalpha, beta + dbeta);
			glVertex3f(x12, y12, z2);
			glTexCoord2f(alpha, beta + dbeta);
			glVertex3f(x11, y11, z2);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			alpha += dalpha;
		}
	}

	
}
void createdi(float r, float h)
{

	//glTranslated(0.0, 1.0, 0.0);
	float fR = r;
	float fH = h;
	float o = 0.0;
	//生成柱面三角网
	int m = 100;//水平方向圆离散化多边形边数

	float dalpha = 2.0f * PI / m;

	float z0 = -h / 2;
	float z1 = h / 2;

	float alpha = 0.0f;
	for (int j = 0; j < m; j++)
	{
		float x01 = fR * cos(alpha);
		float y01 = fR * sin(alpha);

		float x02 = fR * cos(alpha + dalpha);
		float y02 = fR * sin(alpha + dalpha);

		//柱面
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		glTexCoord2f(alpha , 0);
		glVertex3f(x01, y01, z0);
		glTexCoord2f((alpha + dalpha) , 0);
		glVertex3f(x02, y02, z0);
		glTexCoord2f((alpha + dalpha) , 1);
		glVertex3f(x02*4/5, y02*4/5, z1);

		glTexCoord2f(alpha, 0);
		glVertex3f(x01, y01, z0);
		glTexCoord2f((alpha + dalpha), 1);
		glVertex3f(x01 * 4 / 5, y01 * 4 / 5, z1);
		glTexCoord2f(alpha, 1);
		glVertex3f(x02 * 4 / 5, y02 * 4 / 5, z1);
		glEnd();

		alpha += dalpha;
	}
	glDisable(GL_TEXTURE_2D);
}//圆柱

void createhuo(float r, float h)
{

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture4);
	//glTranslated(0.0, 1.0, 0.0);
	float fR = r;
	float fH = h;
	float o = 0.0;
	//生成柱面三角网
	int m = 100;//水平方向圆离散化多边形边数

	float dalpha = 2.0f * PI / m;

	float z0 = -h / 2;
	float z1 = h / 2;

	float alpha = 0.0f;
	for (int j = 0; j < m; j++)
	{
		float x01 = fR * cos(alpha);
		float y01 = fR * sin(alpha);

		float x02 = fR * cos(alpha + dalpha);
		float y02 = fR * sin(alpha + dalpha);

		//柱面
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		glTexCoord2f(alpha, 0);
		glVertex3f(x01, y01, z0);
		glTexCoord2f((alpha + dalpha), 0);
		glVertex3f(x02, y02, z0);
		glTexCoord2f((alpha + dalpha), 1);
		glVertex3f(x02 * 4 / 5, y02 * 4 / 5, z1);


		alpha += dalpha;
	}
	glDisable(GL_TEXTURE_2D);
}//圆柱


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
	glBindTexture(GL_TEXTURE_2D, texture5);
	glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-50.0f,0.0f,0.0f);//指定顶点坐标
		glTexCoord2f(10.0f,0.0f);
		glVertex3f(50.0f,0.0f,50.0f);
		glTexCoord2f(10.0f,10.0f);
		glVertex3f(50.0f,0.0f,-50.0f);
		glTexCoord2f(0.0f,10.0f);
		glVertex3f(-50.0f,0.0f,-50.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);



	fudong = fudong + xfudong;
	if (fudong > 2.8)
	{
		xfudong = -xfudong ;
	}
	else if(fudong < 2.4)
	{
		xfudong = -xfudong;
	}
	fudongde = fudongde + xfudong;
	if (fudongde > 2.8)
	{
		xfudong = -xfudong;
	}
	else if (fudongde < 2.4)
	{
		xfudong = -xfudong;
	}
	glPushMatrix();
	glTranslatef(0.0f, 1.0f, 0.0f);
	glRotated(-90, 1.0f, 0.0f, 0.0f);
	createtai();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, fudongde, 0.0f);
	glRotated(t, 1.0f, 0.0f, 0.0f);
	sphere.InitData(0.4f);
	sphere.SetPos(cgPoint3D(0.0f, 0, 0.0f));
	glColor3f(1.0, 0.0, 0.0);
	sphere.Render();
	cube.InitData(0.5);
	cube.SetPos(cgPoint3D(0, 0, 0.0f));
	glColor3f(1.0, 1.0, 0.0);
	cube.Render();
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1.0f, 0.0f, 0.0f);
	glRotated(xt, 0, 0, -1.0f);
	xt = xt + 0.01;
	createsphere(2,50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.5f, 0.0f);
	glRotated(-90, 1.0f, 0.0f, 0.0f);
	createdi(1.9,1);
	glPopMatrix();

	
	glPushMatrix();
	glRotated(-90, 1.0f, 0.0f, 0.0f);
	glRotated(t, 0.0f, 0.0f, 1.0f);
	t=t+0.2;
	createhuo(3, 1);
	glPopMatrix();

	cube.InitData(0);
	cube.SetPos(cgPoint3D(0, 3.0f, 0.0f));
	glColor3f(1.0,0.0,0.0);
	cube.Render();

	if (flag)
	{
		glPushMatrix();
		glTranslatef(0.0f, fudong, 0.0f);
		glRotated(90, 1.0f, 0.0f, 0.0f);
		glRotated(t, 0.0f, 0.0f, 1.0f);

		createsphere(1, 1.5);
		glPopMatrix();
	}
	
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
		case 'U':
		case 'u':
			jia += 0.2;

			if (jia > 1)
			{
				flag = 0;
			}
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

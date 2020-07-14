#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include <GL/glut.h>

#ifdef WIN32
	#pragma comment(lib, "glut32.lib")
	#pragma comment(lib, "glew32.lib")
#endif

#define NCP 0.05
#define FCP 50
#define ANGLE 45.f
float angle = 0;
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0,0,-2);
	glRotatef(angle,1,1,0);
	glColor3ub(0,0,255);
	glutSolidTeapot(0.5);
	glutSwapBuffers();
	angle +=0.5;
	if(angle>360)angle-=360;
}

void reshape(int w, int h)
{
	if(h == 0)h = 1;
	float ratio = float(w) / float(h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(ANGLE, ratio, NCP, FCP);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}


/// Shaders handles
GLhandleARB m_program;
GLhandleARB m_fragmentShader, m_vertexShader;
char* fileRead(char* name)
{
	FILE *file;
	char *content = NULL;
	int count = 0;

	if(name != NULL)
	{
		file = fopen(name, "rt");
		if (file == NULL)return content;
		fseek(file, 0, SEEK_END);
		count = ftell(file);
		rewind(file);
		if(count > 0)
		{
			content = (char*)malloc(sizeof(char)*(count+1));
			count = (int)fread(content, sizeof(char), count, file);
			content[count] = '\0';
		}
		fclose(file);
	}
	return content;
}

void setFragmentShader()
{
	char* sourceCode;
	m_fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	sourceCode = fileRead("fragment.sh");
	const char* fragmentCode = sourceCode;
	glShaderSourceARB(m_fragmentShader, 1, &fragmentCode, NULL);
	free(sourceCode);
	glCompileShaderARB(m_fragmentShader);
}

void setVertexShader()
{
	char* sourceCode;
	m_vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	sourceCode = fileRead("vertex.sh");
	const char* vertexCode = sourceCode;
	glShaderSourceARB(m_vertexShader, 1, &vertexCode, NULL);
	free(sourceCode);
	glCompileShaderARB(m_vertexShader);
}

void printInfoLog(GLhandleARB obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
						&infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}

void initialize()
{
	//OpenGL
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	
	float lightAmbient[] = {1,0.5,0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	float lightDiffuse[] = {1,0.5,0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	float lightSpecular[] = {1,1,0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightSpecular);

	float matAmbient[] = {0.1,0.1,0.1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	float matDiffuse[] = {0.2,0.1,1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
	float matSpecular[] = {0.5,0.5,1};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
	float shininess = 80;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	//GLSL
	GLenum err = glewInit();
	if(GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(0);
	}
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
	printf("Ready for GLSL\n");
	else
	{
		printf("Not totally ready :( \n");
		exit(1);
	}
	
	setVertexShader();
	setFragmentShader();
	m_program = glCreateProgramObjectARB();
	glAttachObjectARB(m_program, m_fragmentShader);
	glAttachObjectARB(m_program, m_vertexShader);
	glLinkProgramARB(m_program);
	glUseProgramObjectARB(m_program);
}

void destroy()
{
	glDetachObjectARB(m_program, m_fragmentShader);
	glDetachObjectARB(m_program, m_vertexShader);
	glDeleteObjectARB(m_program);
}

void keyb(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'q':
		case 27:
			destroy();
			exit(0);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(800,600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("GLUTTemplate");
	initialize();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyb);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
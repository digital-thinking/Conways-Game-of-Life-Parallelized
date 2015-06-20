/* Exercise: [MGS] EPR: Game Of Life; with focus on performance
* Author: David Pertiller
* Date: 21.12.2012
* Version: 2, added OpenMP and OpenCL
*/

// OpenCL includes
//#include <CL/cl.h>
#include <vector>
#include "Change.hpp"
#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"

#define MSTRINGIFY(A) #A
char* stringifiedSourceCL = 
#include "KernelOpenCL.cl"
#include "Shared.h"


#include <GL/glut.h>


using namespace std;

cl::CommandQueue* queue;
cl::Kernel* gol;
cl::Buffer* SwapBuffer;
int ELEMENTS;

BoardData data;
size_t datasize;

int width;
int height;

GLuint texture;
GLuint textureSwap;
cl::Image2DGL openCLTexture;
cl::Image2DGL openCLTextureSwap;
bool toggle = false;

static void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, GLdouble(w), 0, GLdouble(h));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	width = w;
	height = h;
}

static void render()
{
	glClearColor(0.0f, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


	//////////////// openCL
	std::vector<cl::Memory> memObjs;
	memObjs.push_back(openCLTexture);
	memObjs.push_back(openCLTextureSwap);

	//Get images
	cl::Event ev;
	cl_int result = CL_SUCCESS;
	queue->enqueueAcquireGLObjects(&memObjs, NULL, &ev);
	ev.wait();	

	//Assign
	GLuint nextTexture = texture;
	if (!toggle){
	
		gol->setArg(0, openCLTexture);
		gol->setArg(1, openCLTextureSwap);
	}
	else{
		//nextTexture = textureSwap;
		gol->setArg(0, openCLTextureSwap);
		gol->setArg(1, openCLTexture);
	}	

	// Run
	cl::NDRange globalWork(data.width, data.height);
	cl::NDRange groupSize(1, 1);
	queue->enqueueNDRangeKernel(*gol, cl::NullRange, globalWork, groupSize);	

	//Release
	queue->enqueueReleaseGLObjects(&memObjs, NULL, &ev);
	ev.wait();

	//////////////// openGL
	glBindTexture(GL_TEXTURE_2D, nextTexture);

	glLoadIdentity();	
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);

	// Draw a textured quad
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0, height, 0);
	glTexCoord2f(1, 1); glVertex3f(width, height, 0);
	glTexCoord2f(1, 0); glVertex3f(width, 0, 0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	

	glFinish();
	toggle = !toggle;
	//glutSwapBuffers();

}

static void idle()
{

	glutPostRedisplay();
}


void initGL(){

	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glDisable(GL_LIGHTING);
	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);
	glutMainLoop();


}

//the computation method will iterate over the data for the given number of generations
static void ComputeCL(BoardData& dataStruct, const int generations, unsigned __int64 deviceType, int platformId = -1, int deviceID = -1)
{	
	const int width = dataStruct.width;
	const int height = dataStruct.height;

	ELEMENTS = (width*height);   // elements in each vector
	datasize = sizeof(cl_uchar)*ELEMENTS;

	data = dataStruct;

	//pre-calculate field indexes
	static const cl_int lastRow = height - 1; //this avoids the computing expensive modulo which should only be called if necessary
	static const cl_int secondLastRow = lastRow - 1; //this would crash if the height of the board would be below 2, but than GOF would not be possible anyway ;)
	static const cl_int lastColumn = width - 1;
	static const cl_int secondLastColumn = lastColumn - 1;	

	try {
		// Get list of OpenCL platforms.
		vector<cl::Platform> platform;
		cl::Platform::get(&platform);

		//fulfilling additional command line parameters of exercise 2
		if (platformId > -1 && platformId < platform.size())
		{
			cl::Platform userSpecifiedPlatform = platform[platformId];
			platform.clear();
			platform.push_back(userSpecifiedPlatform);
		}

		if (platform.empty()) {
			cerr << "OpenCL platforms not found." << endl;
			exit(-1);
		}
				// Get first available GPU device which supports double precision.
		cl::Context context;
		vector<cl::Platform> platforms;
		vector<cl::Device> device;
		for(auto p = platform.begin(); device.empty() && p != platform.end(); p++) {
			vector<cl::Device> pldev;						
			cout << (*p).getInfo<CL_PLATFORM_NAME>() << endl;

			try {
				p->getDevices(deviceType, &pldev);

				int t = pldev.front().getInfo<CL_DEVICE_TYPE>();				
				for(auto d = pldev.begin(); device.empty() && d != pldev.end(); d++) {
					if (!d->getInfo<CL_DEVICE_AVAILABLE>()) continue;
					
					platforms.push_back(*p);
					device.push_back(*d);	

					cout << (*d).getInfo<CL_DEVICE_NAME>() << endl;
				}
			}
			catch (cl::Error er) {
				device.clear();
				printf("ERROR: %s(%s)\n", er.what(), oclErrorString(er.err()));
			}
		}

		if (device.empty()) {
			cerr << "No devices or no device with specified type found!" << endl;
			exit(-2);
		}	

		//Make Context current
		glutInitWindowSize(800, 600);
		glutCreateWindow("OpenGL openCL test");

		cl_context_properties props[] =
		{
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
			CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(),
			0
		};

		//cl_context cxGPUContext = clCreateContext(props, 1, &cdDevices[uiDeviceUsed], NULL, NULL, &err);
		try{
			context = cl::Context(CL_DEVICE_TYPE_GPU, props);
		}
		catch (cl::Error er) {
			printf("ERROR: %s(%s)\n", er.what(), oclErrorString(er.err()));
		}

		std::ifstream programFile("KernelOpenCL.cl");
		std::string programString(stringifiedSourceCL);
		cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length()+1));
		cl::Program program(context, source);

		try 
		{
			program.build(device);

		} catch (const cl::Error&) 
		{
			cerr << "OpenCL compilation error" << endl << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device[0]) << endl;
			exit(-3);
		}

		// Create Texture
		texture = 0;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data.data.data());

		//Second
		textureSwap = 0;
		glGenTextures(1, &textureSwap);
		glBindTexture(GL_TEXTURE_2D, textureSwap);
		glBindTexture(GL_TEXTURE_2D, textureSwap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data.data.data());

			
		cl_int result;
		openCLTexture = cl::Image2DGL(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, texture, &result);
		if (result != CL_SUCCESS) {
			std::cout << "Texture sharing failed";
		};
		openCLTextureSwap = cl::Image2DGL(context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, textureSwap, &result);
		if (result != CL_SUCCESS) { 
			std::cout << "Texture sharing failed";
		};

		gol = new cl::Kernel(program, "Gol_All");

		// Set kernel parameters.
		cl_int clWidth = width;
		cl_int clHeight = height;

		gol->setArg(2, clWidth);
		gol->setArg(3, clHeight);

		// Create command queue.
		queue = new cl::CommandQueue(context, device[0]);	

		initGL();		

	
	}
	catch (const cl::Error &err)
	{
		cerr << "OpenCL error: " << err.what() << "(" << err.err() << ")" << endl;
		exit(-4);
	}
	

}



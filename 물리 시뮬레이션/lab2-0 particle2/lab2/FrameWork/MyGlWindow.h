
#include <FL/Fl_Gl_Window.h>
#include <Fl/Fl.h>
#include <Fl/Fl_Value_Slider.H>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <Fl/Fl_Double_Window.h>

#include "stdio.h"
#include "math.h"
#include "3DUtils.h"

#include "Vec3f.h"
#include "Viewer.h"
#include <vector>

#include "core.h"
#include "MyContact.h"
#include "Mover.h"

class MyGlWindow : public Fl_Gl_Window {
	public:
		MyGlWindow(int x, int y, int w, int h);

		std::vector<Vec3f *> history;
		

		Fl_Slider* time;	
		int run;
		void update();
		void drawStuff();
		void doPick();
		void test();
	private:
		void draw();					// standard FlTk
		
		void putText(char * string, int x, int y, float r, float g, float b);
		int handle(int);				// standard FlTk
		
		float fieldOfView;
		Viewer *m_viewer;

		std::vector<Mover*> m_movers;
		
		cyclone::ParticleContact m_contact[2];
		std::vector<cyclone::ParticleContactGenerator*> m_contactGenerators;
		cyclone::ParticleContactResolver* m_resolver;

		void setProjection(int clearProjection = 1);
		void getMouseNDC(float& x, float& y);
		void setupLight(float x, float y, float z);
};


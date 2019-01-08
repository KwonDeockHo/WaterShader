


#include <iostream>
#include <windows.h>


#include <gl/glew.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <Fl/Fl_Gl_Window.H>
#include <Fl/Fl_Timer.H>

#include "MyGlWindow.h"
#include "Timing.h"


#include <time.h>

clock_t ttt = 0;

void idle(void * w)
{
	clock_t t = clock();

	ttt += t;
	if (ttt > 3000) {
		//	MyGlWindow * window = (MyGlWindow *)w;
		std::cout << " now " << std::endl;
		ttt = 0;
	}


}


void idler(void * w)
{

	MyGlWindow * window = (MyGlWindow *)w;
	/* idler code BEGIN */
	if (window->m_run) {
		//m_staticAnimController->setStep(0);

		_int64 ctime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&ctime));

		int r = 60;

		if ((ctime - window->m_clock.etime) < (window->m_clock.freq / r)) return;

		window->m_clock.stop();

		if (window->m_water) {
			window->m_water->update();
		}
		window->m_clock.start();

		window->damage(1);
	}
	else {


	}

}


int main()
{
	Fl::scheme("plastic");// plastic
	int width = 1000;
	int height = 800;
	Fl_Double_Window* wind = new Fl_Double_Window(100, 100, width, height, "OpenGL Water Shader");

	wind->begin();		// put widgets inside of the window
	MyGlWindow* gl = new MyGlWindow(10, 10, width - 20, height - 20);

	wind->resizable(gl);

	Fl::add_idle((void(*)(void*)) idler, gl);

	wind->show();	// this actually opens the window


	Fl::run();
	delete wind;

	return 1;

}

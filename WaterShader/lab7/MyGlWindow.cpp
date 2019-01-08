

#include "MyGlWindow.h"


static double DEFAULT_VIEW_POINT[3] = { 50, 50, 50 };//
static double DEFAULT_VIEW_CENTER[3] = { 7.5, -5, 7.5 };
//static double DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };

static double DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };


MyGlWindow::MyGlWindow(int x, int y, int w, int h)
: Fl_Gl_Window(x, y, w, h)
//==========================================================================
{

	mode(FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL);
	first = 0;


	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);


	m_run = 1;
}


glm::mat4 lookAt(glm::vec3 & pos, glm::vec3 & look, glm::vec3  & up)
{
	glm::vec3 zaxis = glm::normalize(pos - look);
	glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

	glm::mat4 R;

	R[0] = glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0.0f);
	R[1] = glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0.0f);
	R[2] = glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0.0f);
	R[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 T;

	T[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	T[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	T[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	T[3] = glm::vec4(-pos.x, -pos.y, -pos.z, 1.0f);


	return R*T;
}


glm::mat4 perspective(float fov, float aspect, float n, float f)
{
	glm::mat4 P(0.0f);
	float rad = fov * glm::pi<float>() / 180.0f;
	const float tanHalfFOV = tan(rad / 2.0f);

	float A = -(n + f) / (f - n);
	float B = (2.0f * (n * f)) / (n - f);

	P[0] = glm::vec4(aspect / (tanHalfFOV), 0, 0, 0);
	P[1] = glm::vec4(0, 1.0f / tanHalfFOV, 0, 0);
	P[2] = glm::vec4(0, 0, A, -1.0f);
	P[3] = glm::vec4(0, 0, B, 0.0f);


	return P;

}
void MyGlWindow::draw(void)
{
	if (!first) {

		glewExperimental = TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			//Problem: glewInit failed, something is seriously wrong.
			std::cout << "glewInit failed, aborting." << std::endl;
		}
		//m_water->setupFBO_refraction();

		first = 1;

		initialize();
		m_water->setupFBO_reflection(w(), h());
		m_water->setupFBO_refraction(w(), h());

	}

	glm::vec3 eye = m_viewer->getViewPoint(); // m_viewer->getViewPoint().x(), m_viewer->getViewPoint().y(), m_viewer->getViewPoint().z());
	glm::vec3 look = m_viewer->getViewCenter();   //(m_viewer->getViewCenter().x(), m_viewer->getViewCenter().y(), m_viewer->getViewCenter().z());
	glm::vec3 up = m_viewer->getUpVector(); // m_viewer->getUpVector().x(), m_viewer->getUpVector().y(), m_viewer->getUpVector().z());
	glm::mat4 view = glm::lookAt(eye, look, up);

	glm::mat4 projection = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);

	//reflectionFrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_water->reflectionFrameBuffer);

	glClearColor(GL_RED, GL_GREEN, GL_BLUE, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CLIP_DISTANCE0);

	glViewport(0, 0, w(), h());

	m_model.glPushMatrix();
		//Ä«¸Þ¶ó
		float distance = 2.0f *(eye.y - (-5));
		
		glm::vec3 camPos = eye;
	
		
		camPos.y -= distance;
			
		glm::mat4 view2 = glm::lookAt(camPos, look, up);
			

		glm::vec4 plane(0, 1, 0, 5+1);
		if (m_skybox)
			m_skybox->draw(m_model.getMatrix(), view2, projection);

		if (m_terrain) {
			m_terrain->draw(m_model.getMatrix() , view2, projection, plane);
		}
	m_model.glPopMatrix();

	//refractionFrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_water->refractionFrameBuffer);

	glClearColor(GL_RED, GL_GREEN, GL_BLUE, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, w(), h());

	m_model.glPushMatrix();
		
		view = lookAt(eye, look, up);

		glm::vec4 plane2 = glm::vec4(0, -1, 0, -5.0);
		if (m_terrain) {
			m_terrain->draw(m_model.getMatrix(), view, projection, plane2);
		}
	m_model.glPopMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(GL_RED, GL_GREEN, GL_BLUE, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_CLIP_DISTANCE0);

	glViewport(0, 0, w(), h());

	view = lookAt(eye, look, up);

	m_model.glPushMatrix();
		glm::vec4 planeMain = glm::vec4(0, -1, 0, 1500);
		
		if (m_skybox)
			m_skybox->draw(m_model.getMatrix(), view, projection);

		if (m_terrain) {
			m_terrain->draw(m_model.getMatrix(), view, projection, planeMain);
		}
	m_model.glPopMatrix();


	m_model.glPushMatrix();

		if (m_water)
			m_water->drawPlane(&m_model, view, projection);

	m_model.glPopMatrix();

}

void MyGlWindow::idle()
{


}

MyGlWindow::~MyGlWindow()
{

	delete m_screen;
	delete m_screen2;
	delete m_water;
	delete m_skybox;
	delete m_terrain;
}


void MyGlWindow::initialize()
{
	m_terrain = new Terrain();
	m_skybox = new skybox();
	m_water = new Water(w(),h());

	m_screen = new Screen();
	m_screen2 = new Screen();
}


int m_pressedMouseButton;
int m_lastMouseX;
int m_lastMouseY;

int MyGlWindow::handle(int e)
//==========================================================================
{
	switch (e) {
	case FL_SHOW:		// you must handle this, or not be seen!
		show();
		return 1;
	case FL_PUSH:
	{

					m_pressedMouseButton = Fl::event_button();

					//	  m_viewer->setAspectRatio( static_cast<double>(this->w()) / static_cast<double>(this->h()) );

					m_lastMouseX = Fl::event_x();
					m_lastMouseY = Fl::event_y();
	}
		damage(1);
		return 1;
	case FL_RELEASE:
		m_pressedMouseButton = -1;
		damage(1);
		return 1;
	case FL_DRAG: // if the user drags the mouse
	{

					  double fractionChangeX = static_cast<double>(Fl::event_x() - m_lastMouseX) / static_cast<double>(this->w());
					  double fractionChangeY = static_cast<double>(m_lastMouseY - Fl::event_y()) / static_cast<double>(this->h());

					  if (m_pressedMouseButton == 1) {
						  m_viewer->rotate(fractionChangeX, fractionChangeY);
					  }
					  else if (m_pressedMouseButton == 2) {
						  m_viewer->zoom(fractionChangeY);
					  }
					  else if (m_pressedMouseButton == 3) {
						  m_viewer->translate(-fractionChangeX, -fractionChangeY, (Fl::event_key(FL_Shift_L) == 0) || (Fl::event_key(FL_Shift_R) == 0));
					  }
					  else {
						  std::cout << "Warning: dragging with unknown mouse button!  Nothing will be done" << std::endl;
					  }

					  m_lastMouseX = Fl::event_x();
					  m_lastMouseY = Fl::event_y();
					  redraw();
	}

		return 1;

	case FL_KEYBOARD:
		return 0;

	default:
		return 0;
	}
}
/*
cs250_project1.cpp
-Name : Hwang Chan IL
-Assignment Number : project1
-Course Name : CS250
-term : Spring 2019
*/

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#include "Projection.h"
#include "Render.h"
#include "CubeMesh.h"
#include "SnubDodecMesh.h"
#include "FrustumMesh.h"

using namespace std;

const	Point	O(0, 0, 0);
const	Vector	EX(1, 0, 0),
				EY(0, 1, 0),
				EZ(0, 0, 1);
const	float	PI = 4.0f * atan(1.0f);
const	Vector	WHITE(1, 1, 1),
				PURPLE(1, 0, 1),
				BLACK(0, 0, 0),
				BLUE(0, 0, 1),
				BLUED(0, 32.f / 256, 0),
				GRAY(150.f/256, 75.f/256, 0),
				RED(1, 0, 0),
				GREEN(0, 1, 0),
				GOLD(1, 215.f / 256, 0);

class StarMesh : public Mesh {
public:
	int VertexCount(void) { return 12; }
	Point GetVertex(int i) { return vertices[i]; }
	Vector Dimensions(void) { return Vector(2, 2, 2); }
	Point Center(void) { return Point(0, 0, 0); }
	int FaceCount(void) { return 20; }
	Face GetFace(int i) { return faces[i]; }
	int EdgeCount(void) { return 30; }
	Edge GetEdge(int i) { return edges[i]; }
private:
	static Point vertices[12];	
	static Face faces[20];
	static Edge edges[30];
};

Point StarMesh::vertices[12] = {
	Point(0,0,1), Point(1,0,0), Point(0.404509f,0.293893f, 0), Point(0.309017f, 0.951057f, 0), Point(-0.154508f, 0.475528f,0),
	Point(-0.809017f, 0.587785f, 0), Point(-0.5f,-4.37114e-08f,0), Point(-0.809017f, -0.587785f, 0), Point(-0.154508f, -0.475528f, 0),
	Point(0.309017f, -0.951056f, 0), Point(0.404508f, -0.293893f, 0), Point(0,0,-1)
};

Mesh::Face StarMesh::faces[20] = {
	Face(0,1,2), Face(0,2,3), Face(0,3,4), Face(0,4,5),
	Face(0,5,6), Face(0,6,7), Face(0,7,8), Face(0,8,9),
	Face(0,9,10), Face(11,2,1), Face(11,3,2), Face(11,4,3),
	Face(11,5,4), Face(11,6,5), Face(11,7,6), Face(11,8,7),
	Face(11,9,8), Face(11,10,9), Face(0,10,1), Face(11,1,10)
};

Mesh::Edge StarMesh::edges[30] = {
	Edge(0,1), Edge(0,2), Edge(0,3), Edge(0,4),
	Edge(0,5), Edge(0,6), Edge(0,7), Edge(0,8),
	Edge(0,9), Edge(0,10), Edge(11,1), Edge(11,2),
	Edge(11,3), Edge(11,4), Edge(11,5), Edge(11,6),
	Edge(11,7), Edge(11,8), Edge(11,9), Edge(11,10),
	Edge(1,2), Edge(2,3), Edge(3,4), Edge(4,5),
	Edge(5,6), Edge(6,7), Edge(7,8), Edge(8,9),
	Edge(9,10), Edge(10,1)
};

class Client
{
public:
    Client(SDL_Window* window);
    ~Client(void);
    void draw(double dt);
    void keypress(SDL_Keycode kc);
    void resize(int W, int H);
	void draw_all_objects(float dt);
	void draw_faces(Mesh& mesh, Affine& a, const Vector& color, Affine& wor2cam, Matrix& cam2ndc);
private:
    // variables for frame rate:
    SDL_Window* window;
    int frame_count;
    double frame_time;
	float acuumulate_dt;
    // other variables:
	Camera cam1, cam2, cam3;
	Affine camworld2camera;
	Matrix camcamera2ndc;
	std::vector<Hcoord> cam_vertices,
					   ndc_vertices;
	SnubDodecMesh sun_mesh,
				  planets_mesh[8];
	CubeMesh cube_mesh,
			camera_mesh;
	StarMesh star_mesh;
	Affine sun2world,
		planets2world[8],
		cube2world[36],
		star2world[5],
		camera2world;
	float sun_rot_rate,
		planets_rot_rate[8];
	Vector sun_rot_axis,
			planets_rot_axis[8],
			light,
			cam2look;
	const Vector planets_color[8] = {
		WHITE,
		GOLD,
		BLUE,
		GRAY,
		GRAY,
		GRAY,
		BLUE,
		BLUED
	};
	Point sun_center,
		planets_center[8],
		camera_center;

	bool use_cam1,
		 use_cam2;
	Render*	render;
};


Client::Client(SDL_Window* win) : window(win)
{
	acuumulate_dt = 0;
    frame_count = 0;
    frame_time = 0;
	render = new Render();

	use_cam1 = true;
	use_cam2 = false;

	sun_rot_rate = 2 * PI / 10.0f;
	sun_rot_axis = EY;
	sun_center = Point(0, 0, 0);

	for (int i = 0; i < 8; i++) {
		planets_rot_rate[i] = (2.f * PI) / ((i + 1) * 1.5f);
		planets_center[i] = Point(0.f, 0.f, -6.5f - i * 3);
		planets_rot_axis[i] = EY;
		planets2world[i] = translate(sun_center - planets_center[i])
			 * scale( 3 - float( i / 3 ));
	}
	camera_center = Point(20 * sin(PI / 10 + PI), 20 * sin(PI / 15 - PI / 2) + 20, 20 * sin(PI / 20));
	camera2world = translate(Vector(camera_center - O))
		 * scale(2.f, 1.f, 2.f);
	sun2world = translate(sun_center - O) * scale(5);
	for (int i = 0; i < 36; i++) {
		cube2world[i] = translate(Vector(float(10 * (i % 6) - 25), -15.f, float(10 * (i / 6) - 35)))
			* rotate(PI / 6, EX)
			* scale(3.75f, 3.75f, 3.75f);
	}
	for (int i = 0; i < 5; i++) {
		star2world[i] = translate(Vector(10.f - i * 5.f, 8.f, -7.f)) * scale(3.f);
	}

	cam1 = Camera(O + 10 * EZ, -EZ, EY, 0.5f * PI, 1, 0.01f, 100.1f);
	camworld2camera = WorldToCamera(cam1);
	camcamera2ndc = CameraToNDC(cam1);

	cam2 = Camera(O + 30 * EZ, -EZ, EY, 0.5f * PI, 1, 0.01f, 100.1f);

	cam3 = Camera(O + 30 * EZ, -EZ, EY, 0.5f * PI, 1, 0.01f, 100.1f);
	
	light = Vector(0, 0, 1);
	cam2look = Vector(0, 0, 1);
}


Client::~Client(void)
{
	delete render;
}

void Client::draw_all_objects(float dt) {
	sun2world = translate(sun_center - O)
		* rotate(sun_rot_rate * static_cast<float>(dt), sun_rot_axis)
		* translate(O - sun_center)
		* sun2world;
	sun_center = sun2world * sun_center;
	//draw_faces(sun_mesh, sun2world, RED, camworld2camera, camcamera2ndc);
	for (int j = 0; j < 8; j++) {
		planets2world[j] = translate(sun_center - O)
			* rotate(planets_rot_rate[j] * static_cast<float>(dt), planets_rot_axis[j])
			* translate(O - sun_center)
			* planets2world[j];
		draw_faces(planets_mesh[j], planets2world[j], planets_color[j], camworld2camera, camcamera2ndc);
	}
	for (int z = 0; z < 36; z++) {
		draw_faces(cube_mesh, cube2world[z], BLUE, camworld2camera, camcamera2ndc);
	}
	for (int i = 0; i < 5; i++) {
		draw_faces(star_mesh, star2world[i], GOLD, camworld2camera, camcamera2ndc);
	}
	acuumulate_dt += static_cast<float>(dt);
	Point pre_camera_center = camera_center;
	camera_center = Point(20 * sin(PI * acuumulate_dt / 10 + PI), 20 * sin(PI * acuumulate_dt / 15 - PI / 2) + 25, 20 * sin(PI * acuumulate_dt / 20));
	Vector pre_camlook = cam2look;
	cam2look = Vector(camera_center - pre_camera_center);
	float cam_rot_rate = acos((dot(pre_camlook, cam2look)) / (abs(pre_camlook) * abs(cam2look)));
	camera2world = translate(Vector(camera_center.x, camera_center.y, camera_center.z))
		*  rotate(cam_rot_rate, cross(pre_camlook, cam2look))
		* translate(-Vector(pre_camera_center.x, pre_camera_center.y, pre_camera_center.z))
		* camera2world;
	draw_faces(camera_mesh, camera2world, BLACK, camworld2camera, camcamera2ndc);
}

void Client::draw_faces(Mesh& mesh, Affine& a, const Vector& color, Affine& wor2cam, Matrix& cam2ndc) {
	cam_vertices.clear();
	ndc_vertices.clear();
	Affine meshmodel2world = wor2cam * a;
	for (int i = 0; i < mesh.VertexCount(); i++) {
		cam_vertices.push_back(meshmodel2world * mesh.GetVertex(i));
		Hcoord result = cam2ndc * cam_vertices[i];
		ndc_vertices.push_back(result);
	}
	for (int i = 0; i < mesh.FaceCount(); i++) {
		const Mesh::Face& targetFace = mesh.GetFace(i);
		const Point& P = cam_vertices[targetFace.index1],
			Q = cam_vertices[targetFace.index2],
			R = cam_vertices[targetFace.index3];
		if (P.z < 0 || Q.z < 0 || R.z < 0) {
			Vector normal = cross(Q - P, R - P);
			if (dot(normal, O - P) > 0) {
				float mu = abs(dot(normal, light)) / (abs(normal) * abs(light));
				render->SetColor(mu * color);
				render->FillTriangle(ndc_vertices[targetFace.index1],
					ndc_vertices[targetFace.index2],
					ndc_vertices[targetFace.index3]);
			}
		}
	}
}

void Client::draw(double dt)
{
    ++frame_count;
    frame_time += dt;
    if (frame_time >= 0.5)
    {
        double fps = frame_count / frame_time;
        frame_count = 0;
        frame_time = 0;
        stringstream ss;
        ss << "CS 250: Project #1 [fps=" << int(fps) << "]";
        SDL_SetWindowTitle(window, ss.str().c_str());
    }

	render->ClearBuffers(WHITE);

	if (use_cam1) {
		cam1 = Camera(O + 30 * EZ, -EZ, EY, 0.5f * PI, cam1.ViewportGeometry().x / cam1.ViewportGeometry().y, 0.01f, 100.1f);
		camworld2camera = WorldToCamera(cam1);
		camcamera2ndc = CameraToNDC(cam1);
	}
	else if (use_cam2) {
		cam2 = Camera(camera_center, cam2look, EY, 0.5f * PI, cam2.ViewportGeometry().x / cam2.ViewportGeometry().y, cam2.NearDistance(), cam2.FarDistance());
		camworld2camera = WorldToCamera(cam2);
		camcamera2ndc = CameraToNDC(cam2);
	}
	else {
		cam3 = Camera(camera_center, Vector(sun_center - camera_center), EY, 0.5f * PI, cam3.ViewportGeometry().x / cam3.ViewportGeometry().y, cam3.NearDistance(), cam3.FarDistance());
		camworld2camera = WorldToCamera(cam3);
		camcamera2ndc = CameraToNDC(cam3);
	}
	draw_all_objects(static_cast<float>(dt));
}

void Client::keypress(SDL_Keycode kc)
{
	switch (kc) {
		case SDLK_SPACE:
			if (use_cam1) {
				use_cam1 = false;
				use_cam2 = true;
			}
			else if (use_cam2) {
				use_cam2 = false;
			}
			else {
				use_cam1 = true;
			}
			break;
	}
}


void Client::resize(int W, int H)
{
	glViewport(0, 0, W, H);
	cam1 = Camera(cam1.Eye(), -cam1.Back(), EY, 0.5f*PI, static_cast<float>(W)/H, cam1.NearDistance(), cam1.FarDistance());
}


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int /*argc*/, char* /*argv*/[])
{
    srand(unsigned(time(0)));

    // SDL: initialize and create a window
    SDL_Init(SDL_INIT_VIDEO);
    const char* title = "CS250: Project #1";
    int width = 600, height = 600;
    SDL_Window* window =
        SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                         width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    // GLEW: get function bindings (if possible)
    glewInit();
    if (!GLEW_VERSION_2_0)
    {
        cout << "needs OpenGL version 3.0 or better" << endl;
        return -1;
    }

    // animation loop
    try
    {
        bool done = false;
        Client* client = new Client(window);
        Uint32 ticks_last = SDL_GetTicks();
        while (!done)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                        done = true;
                        break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                            done = true;
                        else
                            client->keypress(event.key.keysym.sym);
                        break;
                    case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                            client->resize(event.window.data1, event.window.data2);
                        break;
                }
            }
            Uint32 ticks = SDL_GetTicks();
            double dt = 0.001 * (ticks - ticks_last);
            ticks_last = ticks;
            client->draw(dt);
            SDL_GL_SwapWindow(window);
        }
        delete client;
    }

    catch (exception& e)
    {
        cout << e.what() << endl;
    }

    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return 0;
}

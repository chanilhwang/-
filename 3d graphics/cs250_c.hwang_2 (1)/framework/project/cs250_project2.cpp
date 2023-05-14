/*
cs250_project2.cpp
-Name : Hwang Chan IL
-Assignment Number : project2
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
#include "Clip.h"

using namespace std;
namespace {
	const Point	O(0, 0, 0);
	const Vector EX(1, 0, 0),
				 EY(0, 1, 0),
				 EZ(0, 0, 1);
	const float PI = 4.0f * atan(1.0f);
	const Vector WHITE(1, 1, 1),
				 PURPLE(1, 0, 1),
				 BLACK(0, 0, 0),
				 BLUE(0, 0, 1),
				 BLUED(0, 32.f / 256, 0),
				 GRAY(150.f / 256, 75.f / 256, 0),
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
	struct Object {
		Object() {
			object_count++;
		}
		Mesh* mesh;
		Vector color;
		Affine to_world;
		Point center;
		int object_num;
		float size;
		static int object_count;
	};
	int Object::object_count = 0;
	struct Rot_Object : public Object {
		float rot_rate;
		Vector rot_axis;
	};
	Affine NDCtoDevice(int width, int height) {
		return Affine(Vector(width / 2.f, 0, 0),
			Vector(0, height / 2.f, 0),
			Vector(0, 0, 0.5f),
			Point(width / 2.f, height / 2.f, 0.5f));
	}
	Affine DevicetoNDC(int width, int height) {
		return inverse(NDCtoDevice(width, height));
	}
	Matrix NDCToCamera(const Camera& cam) {
		const float& Ex = cam.Eye().x;
		const float& Ey = cam.Eye().y;
		const float& N = cam.NearDistance();
		const float& W = cam.ViewportGeometry().x * N / cam.ViewportGeometry().z;
		const float& H = cam.ViewportGeometry().y * N / cam.ViewportGeometry().z;
		Matrix result;

		result[0] = Hcoord(W / 2, 0, 0, Ex);
		result[1] = Hcoord(0, H / 2, 0, Ey);
		result[2] = Hcoord(0, 0, 0, -N);
		result[3] = Hcoord(0, 0, 0, 1);

		return result;
	}
	//only for plane : y = 1 shadowing
	Matrix Direction_Shading_Matrix(Point light) {
		Matrix result;

		result[0] = Hcoord(light.y + 1, -light.x, 0, -light.x);
		result[1] = Hcoord(0, -1, 0, light.y);
		result[2] = Hcoord(0, -light.z, light.y + 1, -light.z);
		result[3] = Hcoord(0, -1, 0, light.y);

		return result;
	}
}
class Client
{
public:
    Client(SDL_Window* window);
    ~Client(void);
    void draw(double dt);
	void keypress(SDL_Keycode kc);
	void mousepress(Sint32 x, Sint32 y, Uint8 mb);
	void mouserelease(Uint8 mb);

	//helper functions
	void draw_all_objects(float dt);
	void draw_faces(const Object& object);
	void draw_faces_with_diffuse_shading(const Object& object, const Vector& light_);
	void draw_shadows(const Object& object, const Matrix& to_shadow);
	void calculate_matrices();
	float intersect_test(const Object& obj, const Vector& vec, const Point& point);

private:
	Render*	render;

    // variables for frame rate:
    SDL_Window* window;
    int frame_count;
    double frame_time;

	//meshs
	StarMesh star;
	CubeMesh cube;
	SnubDodecMesh snub;

    // variables for camera
	Camera cam;

	//variables for objects
	float object_size;
	std::vector<Object> occluders;
	Rot_Object light;
	Object receiver;

	//variables for counts
	unsigned int culled_objects_count;
	int picked_id;

	//varibles for storing temp
	std::vector<Hcoord> cam_vertices,
		ndc_vertices,
		dev_vertices,
		shadow_world_vertices,
		shadow_cam_vertices,
		shadow_ndc_vertices,
		shadow_dev_vertices;
	Matrix cam_to_ndc,
		ndc_to_cam,
		shadow;
	Affine world_to_cam,
		cam_to_world,
		ndc_to_dev,
		dev_to_ndc;

	//variables for mouse
	//can be removed
	Point mousepos;
	Point mousepos_in_world;
	Vector ray_from_eye_to_mouse;
	bool LeftMousePressed;
};

Client::Client(SDL_Window* win)
	: window(win), frame_count(0), frame_time(0), LeftMousePressed(false),
	culled_objects_count(0), picked_id(-1)
{
	//allocate render
	render = new Render();

	//initialize light object
	light.mesh = &cube;
	light.color = RED;
	light.center = Point(5, 10, 0);
	light.rot_axis = EY;
	light.rot_rate = 2 * PI / 3.f;
	light.to_world = translate(light.center - O);

	//initialize occluder objects
	object_size = 1.f;
	Object object1;
	object1.size = object_size;
	object1.mesh = &star;
	object1.color = RED;
	object1.center = Point(float(-6), 2, 0);
	object1.to_world = translate(object1.center - O)
		* scale(object_size, object_size, object_size);
	occluders.push_back(object1);
	Object object2;
	object2.size = object_size;
	object2.mesh = &snub;
	object2.color = BLUE;
	object2.center = Point(float(-2), 2, 0);
	object2.to_world = translate(object2.center - O)
		* scale(object_size, object_size, object_size);
	occluders.push_back(object2);
	Object object3;
	object3.size = object_size;
	object3.mesh = &snub;
	object3.color = BLUE;
	object3.center = Point(float(2),2, 0);
	object3.to_world = translate(object3.center - O)
		* scale(object_size, object_size, object_size);
	occluders.push_back(object3);
	Object object4;
	object4.size = object_size;
	object4.mesh = &cube;
	object4.color = GOLD;
	object4.center = Point(float(6), 2, 0);
	object4.to_world = translate(object4.center - O)
		* scale(object_size, object_size, object_size);
	occluders.push_back(object4);

	receiver.mesh = &cube;
	receiver.center = Point(0, 1, 0);
	receiver.color = GREEN;
	receiver.to_world = translate(receiver.center - O)
		* scale(30,-0.001f,8);

	//initialize camera
	cam = Camera(O + 10 * EZ + 5 * EY, -EZ, EY, 0.5f * PI, 1, 1.f, 100.1f);

	//calculate matrices
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	ndc_to_dev = NDCtoDevice(width, height);
	dev_to_ndc = inverse(ndc_to_dev);
	calculate_matrices();
}


Client::~Client(void)
{
	delete render;
}

void Client::draw_all_objects(float dt) {
	//clear buffers;
	render->ClearBuffers(WHITE);
	culled_objects_count = 0;

	//3d Picking
	if (LeftMousePressed) {
		LeftMousePressed = false;
		Object* collided_object = nullptr;
		float collided_time = -1;
		for (int i = 0; i < Object::object_count - 2; i++) {
			float time = intersect_test(occluders[i], ray_from_eye_to_mouse, cam.Eye());
			if (time >= 0) {
				collided_object = &occluders[i];
				if (time != -1 && time < collided_time) {
					collided_time = time;
				}
			}
		}
		if (collided_time >= 0) {
			(*collided_object).color = WHITE;
		}
	}

	//update model to world matrices
	light.to_world = rotate(light.rot_rate * dt, light.rot_axis)
		* light.to_world;
	light.center = Point(light.to_world[0][3], light.to_world[1][3], light.to_world[2][3]);
	shadow = Direction_Shading_Matrix(light.center);

	//render objects
	draw_faces(light);
	for (int i = 0; i < Object::object_count - 2; i++) {
		draw_faces_with_diffuse_shading(occluders[i], occluders[i].center - light.center);
		draw_shadows(occluders[i], shadow);
	}

	//render reicever
	draw_faces(receiver);
}

void Client::draw_faces(const Object& object) {
	//clear temps;
	cam_vertices.clear();
	ndc_vertices.clear();
	dev_vertices.clear();
	Affine meshmodel2cam = world_to_cam * object.to_world;
	for (int i = 0; i < object.mesh->VertexCount(); i++) {
		cam_vertices.push_back(meshmodel2cam * object.mesh->GetVertex(i));
		Hcoord result = cam_to_ndc * cam_vertices[i];
		result = 1 / result.w * result;
		ndc_vertices.push_back(result);
		dev_vertices.push_back(ndc_to_dev * ndc_vertices[i]);
	}
	for (int i = 0; i < object.mesh->FaceCount(); i++) {
		const Mesh::Face& targetFace = object.mesh->GetFace(i);
		const Point& P = cam_vertices[targetFace.index1],
			Q = cam_vertices[targetFace.index2],
			R = cam_vertices[targetFace.index3];
		if (P.z < 0 || Q.z < 0 || R.z < 0) {
			Vector normal = cross(Q - P, R - P);
			if (dot(normal, O - P) > 0) {
				render->SetColor(object.color);
				render->FillTriangle(ndc_vertices[targetFace.index1],
					ndc_vertices[targetFace.index2],
					ndc_vertices[targetFace.index3]);
			}
		}
	}
}

void Client::draw_faces_with_diffuse_shading(const Object& object, const Vector& light_) {
	//clear temps;
	cam_vertices.clear();
	ndc_vertices.clear();
	Matrix model2cam = world_to_cam * object.to_world;
	for (int i = 0; i < object.mesh->VertexCount(); i++) {
		cam_vertices.push_back(model2cam * object.mesh->GetVertex(i));
		Hcoord result = cam_to_ndc * cam_vertices[i];
		result = 1 / result.w * result;
		ndc_vertices.push_back(result);
	}
	for (int i = 0; i < object.mesh->FaceCount(); i++) {
		const Mesh::Face& targetFace = object.mesh->GetFace(i);
		const Point& P = cam_vertices[targetFace.index1],
					 Q = cam_vertices[targetFace.index2],
					 R = cam_vertices[targetFace.index3];
		if (P.z < 0 || Q.z < 0 || R.z < 0) {
			Vector normal = cross(Q - P, R - P);
			if (dot(normal, O - P) > 0) {
				float mu = abs(dot(normal, light_)) / (abs(normal) * abs(light_));
				render->SetColor(mu * object.color);
				render->FillTriangle(ndc_vertices[targetFace.index1],
					ndc_vertices[targetFace.index2],
					ndc_vertices[targetFace.index3]);
			}
		}
	}
}

void Client::draw_shadows(const Object& object, const Matrix& to_shadow) {
	//clear temps;
	cam_vertices.clear();
	ndc_vertices.clear();
	Matrix model2cam = world_to_cam * to_shadow * object.to_world;
	for (int i = 0; i < object.mesh->VertexCount(); i++) {
		Hcoord result = model2cam * object.mesh->GetVertex(i);
		result = 1 / result.w * result;
		cam_vertices.push_back(result);
		result = cam_to_ndc * cam_vertices[i];
		result = 1 / result.w * result;
		ndc_vertices.push_back(result);
	}
	for (int i = 0; i < object.mesh->FaceCount(); i++) {
		const Mesh::Face& targetFace = object.mesh->GetFace(i);
		const Point& P = cam_vertices[targetFace.index1],
			Q = cam_vertices[targetFace.index2],
			R = cam_vertices[targetFace.index3];
		if (P.z < 0 || Q.z < 0 || R.z < 0) {
			Vector normal = cross(Q - P, R - P);
			if (dot(normal, O - P) > 0) {
				render->SetColor(BLACK);
				render->FillTriangle(ndc_vertices[targetFace.index1],
					ndc_vertices[targetFace.index2],
					ndc_vertices[targetFace.index3]);
			}
		}
		else
			culled_objects_count++;
	}
}

float Client::intersect_test(const Object& obj, const Vector& vec, const Point& point) {
	//1st test
	Vector center2p = obj.center - point,
		center2rayend = obj.center - (point + vec);
	float square_size = obj.size * obj.size,
		dot_center2p = dot(center2p, center2p);
	if (dot_center2p <= square_size) {
		if (dot(center2rayend, center2rayend) <= square_size) {
			return 0;
		}
	}
	else {
		//2nd test
		float p = dot(center2p, vec);
		if (p < 0) {
			return -1;
		}
		//3rd test
		float abs_vec = abs(vec);
		float m = p / abs_vec;
		float square_n = dot_center2p - m * m;
		if (square_n > square_size) {
			return -1;
		}
		//non trivial test
		float s = sqrt(square_size - square_n);
		float time = (m - s) / abs_vec;
		return time;
	}
	return -1;
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
		fps;
	}
	render->ClearBuffers(WHITE);
	draw_all_objects(static_cast<float>(dt));
}

void Client::keypress(SDL_Keycode kc)
{
	switch (kc) {
		case SDLK_SPACE:
			break;
	}
}

void Client::mousepress(Sint32 mouse_x, Sint32 mouse_y, Uint8 mb) {
	switch (mb) {
	case SDL_BUTTON_LEFT:
		if (!LeftMousePressed) {
			mousepos = Point(static_cast<float>(mouse_x), static_cast<float>(mouse_y), 0);
			Hcoord p = world_to_cam * occluders[0].to_world * O;
			p = ndc_to_cam * dev_to_ndc * mousepos;
			mousepos_in_world = cam_to_world * ndc_to_cam * dev_to_ndc * mousepos;
			ray_from_eye_to_mouse = mousepos_in_world - cam.Eye();
			LeftMousePressed = true;
		}
		break;
	}
}

void Client::mouserelease(Uint8 mb) {
	switch (mb) {
	case SDL_BUTTON_LEFT:
		LeftMousePressed = false;
		break;
	}
}

void Client::calculate_matrices() {
	world_to_cam = WorldToCamera(cam);
	cam_to_world = CameraToWorld(cam);
	cam_to_ndc = CameraToNDC(cam);
	ndc_to_cam = NDCToCamera(cam);
}

/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int /*argc*/, char* /*argv*/[])
{
    srand(unsigned(time(0)));

    // SDL: initialize and create a window
    SDL_Init(SDL_INIT_VIDEO);
    const char* title = "CS250: Project #2";
    int width = 640, height = 480;
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
					case SDL_MOUSEBUTTONDOWN:
						client->mousepress(event.button.x, event.button.y, event.button.button);
						break;
					case SDL_MOUSEBUTTONUP:
						client->mouserelease(event.button.button);
                }
            }
            Uint32 ticks = SDL_GetTicks();
            double dt = 0.001f * (ticks - ticks_last);
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

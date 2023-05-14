#include <SDL2/SDL.h>
#include <GL/GL.h>
#include <GL/glew.h>
#include <vector>
#include <CubeMesh.h>
#include <FrustumMesh.h>
#include <Projection.h>
#include <SnubDodecMesh.h>

const float PI = 4.0f * atan(1.0f);

namespace {
	struct Object {
		Mesh* mesh;
		Affine to_world;
		Vector color;
	};
}

class Client {
public:
	Client(SDL_Window* w);
	~Client();
	void draw(float dt);
private:
	SDL_Window* window;
	Object light;
	int number_of_occluder_objects;
	std::vector<Object> occluder_objects;
	Camera cam;
};

Client::Client(SDL_Window* w)
	: window(w)
{
	int width, height;
	SDL_GetWindowSize(window, &width, &height);

}

Client::~Client() {
	delete light.mesh;
	for (int i = 0; i < number_of_occluder_objects; i++) {
		delete occluder_objects[i].mesh;
	}
}

void Client::draw(float dt) {
	//world - cam
	//cam - ndc
	//ndc - dev
	//world - dev

	//3d picking

	//shadow
}

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	const char* title = "CS250";
	int width = 700,
		height = 700;
	SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, width, height,
		SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	bool done = false;
	Client* client = new Client(window);
	Uint32 ticks_last = SDL_GetTicks();
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT :
				done = true;
				break;					
			}
		}
		Uint32 ticks = SDL_GetTicks();
		float dt = 0.001f * (ticks * ticks_last);
		ticks_last = ticks;
		client->draw(dt);
		SDL_GL_SwapWindow(window);
	}

	//clean up
	delete client;
	SDL_GL_DeleteContext(context);
	SDL_Quit();

	return 0;
}
#include <DrawingWindow.h>
#include <Colour.h>
#include <fstream>
#include <vector>

#define WIDTH 700
#define HEIGHT 700

std::string currentMap = "example";

//updates the current map image and graph by reading the new file
void updateMap(std::string newMap){
	if(currentMap != newMap){
		currentMap = newMap;
	}
}

//circle drawing function, takes enter x and y as well as a radius
void drawCircle(DrawingWindow &window, int cx, int cy, int r){
	float red = 255;
	float green = 255;
	float blue = 255;
	uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
	for(int y = cy-r; y < cy+r; y++){
		for(int x = cx-r; x < cx+r; x++){
			float dist = sqrt((cy-y)*(cy-y) + (cx-x)*(cx-x));
			if(dist < r){
				window.setPixelColour(x, y, colour);
			}
		}
	}
}

//main draw function for page refreshes
void draw(DrawingWindow &window) {
	window.clearPixels();
	/*
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			float red = (rand() % 256);
			float green = (rand() % 256);
			float blue = (rand() % 256);
			uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
			window.setPixelColour(x, y, colour);
		}
	}
	*/

	drawCircle(window, 100, 100, 5);
}

//event handler, after initial cleanse for quit function
void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		//window.savePPM("output.ppm");
		//window.saveBMP("output.bmp");
	}
}

//entry point of program, calls event handler and loops drawing
int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}

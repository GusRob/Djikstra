#include <DrawingWindow.h>
#include <Colour.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <Graph.h>

#define WIDTH 600
#define HEIGHT 600

std::string currentMap = "";
Graph map = Graph();

//splits a string into a vector of integers
std::vector<int> split(std::string in, char del){
	std::vector<int> result;
	std::string subString = "";
	for(int i = 0; i < in.length()+1; i++){
		if(in[i] == del){
			result.push_back(std::stoi(subString));
			subString = "";
		} else {
			subString.append(1, in[i]);
		}
	}
	result.push_back(std::stoi(subString));
	return result;
}

//updates the current map image and graph by reading the new file
void updateMap(std::string newMap){
	if(currentMap != newMap){
		currentMap = newMap;
		//open inputfilestream
		std::ifstream ifs ("assets/" + newMap + "/" + newMap + "_graph.txt", std::ifstream::in);
		//start reading chars
		char c = ifs.get();
		//reading mode
		std::string reading = "locations";
		//main read loop
		std::string currentWord = "";
		int point_count = 0;
		int arc_count = 0;
		while(ifs.good()){
			if(c == '\n' || c == ' '){
				//if graph read mode update
				if(currentWord == "locations:"){
					reading = "locations";
				} else if(currentWord == "roads:"){
					reading = "roads";
				} else {
					//split word with comma delimiter
					std::vector<int> elements = split(currentWord, ',');
					if(reading == "locations"){
						map.points.push_back(Point(elements[0], elements[1], point_count++));
					} else if(reading == "roads"){
						map.arcs.push_back(Arc(elements[0], elements[1], arc_count++, bool(elements[2])));
					}
				}
				//
				currentWord = "";
			} else {
				currentWord.append(1, c);
			}
			c = ifs.get();
		}
		ifs.close();
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
	for(int i = 0; i < map.points.size(); i++){
		drawCircle(window, map.points[i].x, map.points[i].y, 5);
	}
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
	updateMap("example");
	while (true) {
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}

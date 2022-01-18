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
int locationSize = 7;

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
						map.arcs.push_back(Arc(map.points[elements[0]], map.points[elements[1]], arc_count++, bool(elements[2])));
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

//packs Colour object into unsigned int colours
uint32_t packCol(Colour col){ return (255 << 24) + (int(col.red) << 16) + (int(col.green) << 8) + int(col.blue); }

//circle drawing function, takes enter x and y as well as a radius
void drawCircle(DrawingWindow &window, int cx, int cy, int r, Colour col){
	uint32_t colour = packCol(col);
	for(int y = cy-r; y < cy+r; y++){
		for(int x = cx-r; x < cx+r; x++){
			float dist = sqrt((cy-y)*(cy-y) + (cx-x)*(cx-x));
			if(dist < r){
				window.setPixelColour(x, y, colour);
			}
		}
	}
}

//linear interpolation function used for line drawing func
std::vector<int> linearInterp(float start, float end, int count){
	float step = (end-start)/count;
	std::vector<int> result;
	for(int i = 0; i < count; i++){
		result.push_back( (int)(start + i*step) );
	}
	return result;
}

//line drawing function, doesnt have perfect rasterization but does the job
void drawLine(DrawingWindow &window, int p1x, int p1y, int p2x, int p2y, int width, Colour col){
	int maxDiff = std::max(std::abs(p1x-p2x), std::abs(p1y-p2y));
	std::vector<int> xs = linearInterp(p1x, p2x, maxDiff);
	std::vector<int> ys = linearInterp(p1y, p2y, maxDiff);
	uint32_t colour = packCol(col);
	for(int i = 0; i < maxDiff; i++){
		window.setPixelColour(xs[i], ys[i], colour);
	}
}

//main draw function for page refreshes
void draw(DrawingWindow &window) {
	window.clearPixels();
	//define colours
	Colour white = Colour(255, 255, 255);
	Colour red = Colour(255, 0, 0);
	Colour green = Colour(255, 0, 255);
	//draw arcs
	for(int i = 0; i < map.arcs.size(); i++){
		Arc a = map.arcs[i];
		if(a.inRoute){
			drawLine(window, a.p1.x, a.p1.y, a.p2.x, a.p2.y, locationSize, red);
		} else {
			drawLine(window, a.p1.x, a.p1.y, a.p2.x, a.p2.y, locationSize, white);
		}
	}
	//draw points
	for(int i = 0; i < map.points.size(); i++){
		Point p = map.points[i];
		if(p.hover){
			drawCircle(window, p.x, p.y, locationSize, red);
		} else {
			drawCircle(window, p.x, p.y, locationSize, white);
		}
		if(p.selected){
			drawCircle(window, p.x, p.y, (locationSize)*3 /4, green);
		} else {
			drawCircle(window, p.x, p.y, (locationSize)*3 /4, white);
		}
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
		for(int i = 0; i < map.points.size(); i++){
			Point p = map.points[i];
			if(p.hover){
				map.points[i].selected = !map.points[i].selected;
			}
		}
		//window.savePPM("output.ppm");
		//window.saveBMP("output.bmp");
	}
}

//mouse hover/position handler
void handleMousePos(){
	int x, y;
	SDL_GetMouseState(&x, &y);
	for(int i = 0; i < map.points.size(); i++){
		Point p = map.points[i];
		if( (x-p.x)*(x-p.x) + (y-p.y)*(y-p.y) <= locationSize*locationSize ){
			map.points[i].hover = true;
		} else {
			map.points[i].hover = false;
		}
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
		handleMousePos();
		draw(window);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}

#include <DrawingWindow.h>
#include <Colour.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <Graph.h>
#include <limits>
#include <Button.h>

#define WIDTH 600
#define HEIGHT 600

//graph global vars
std::string currentMap = "";
Graph map = Graph();
int locationSize = 7;
Point *start = NULL;
Point *end = NULL;
	//0 = least arcs
	//1 = shortest physical distance travelled (ie cartesian between points)
	//2 = shortest route using lengths from asset file


void cleanMap();
void updateMap(std::string);

//function calls djikstras algorithm and calculates route length etc
void setRoute(Point *a, Point *b){
	std::vector<Arc *> route = map.djikstra(a, b);
}

////////////////////////////////
// Initialise Control Buttons //
////////////////////////////////

//testing

void setMapUK(){
	updateMap("uk");
}
void setMapExample(){
	updateMap("example");
}
void changeMode(int i){
	Point *start_temp = start;
	Point *end_temp = end;
	cleanMap();
	if(i == 0){
		map.routeMode = 0;
	} else if(i == 1){
		map.routeMode = 1;
	} else if(i == 2){
		map.routeMode = 2;
	}
	start = start_temp;
	if(start != NULL){
		start->selected = true;
	}
	end = end_temp;
	if(end != NULL){
		end->selected = true;
		setRoute(start, end);
	}
}
void setMode0(){ changeMode(0);}
void setMode1(){ changeMode(1);}
void setMode2(){ changeMode(2);}

Button controlButtons[] = {
	Button(&cleanMap, 0, 0, 100, 50, 25, "assets/buttonicons/refreshicon.png"),
	Button(&setMapExample, 100, 0, 100, 50, 25, "assets/example/example_map.png"),
	Button(&setMapUK, 200, 0, 100, 50, 25, "assets/uk/uk_map.png"),
	Button(&setMode0, 0, 550, 100, 50, 0, "assets/buttonicons/arcs.png"),
	Button(&setMode1, 100, 550, 100, 50, 0, "assets/buttonicons/shortest.png"),
	Button(&setMode2, 200, 550, 100, 50, 0, "assets/buttonicons/custom.png"),
};

/////////////////////////
// WINDOW/UI FUNCTIONS //
/////////////////////////

//packs Colour object into unsigned int colours
uint32_t packCol(Colour col){ return (255 << 24) + (int(col.red) << 16) + (int(col.green) << 8) + int(col.blue); }

//circle drawing function, takes enter x and y as well as a radius
void drawCircle(DrawingWindow &window, int cx, int cy, int r, Colour col){
	uint32_t colour = packCol(col);
	for(int y = cy-r; y < cy+r; y++){
		for(int x = cx-r; x < cx+r; x++){
			float dist = sqrt((cy-y)*(cy-y) + (cx-x)*(cx-x));
			if(dist <= r){
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
void drawLine(DrawingWindow &window, int p1x, int p1y, int p2x, int p2y, Colour col){
	int maxDiff = std::max(std::abs(p1x-p2x), std::abs(p1y-p2y));
	std::vector<int> xs = linearInterp(p1x, p2x, maxDiff);
	std::vector<int> ys = linearInterp(p1y, p2y, maxDiff);
	uint32_t colour = packCol(col);
	for(int i = 0; i < maxDiff; i++){
		window.setPixelColour(xs[i], ys[i], colour);
	}
}

//rectangle filling function,
void fillRect(DrawingWindow &window, int x, int y, int w, int h, Colour col){
	uint32_t colour = packCol(col);
	for(int i = 0; i < w; i++){
		for(int j = 0; j < h; j++){
			window.setPixelColour(x + i, y + j, colour);
		}
	}
}

void drawText(DrawingWindow &window, std::string &text, int x, int y, int w, int h){
	window.drawText(text, 10, 100, 100, 100);

}

//main draw function for page refreshes
void draw(DrawingWindow &window) {
	window.clearPixels();
	//define colours
	Colour white = Colour(255, 255, 255);
	Colour midGray = Colour(178, 178, 178);
	Colour darkGray = Colour(126, 126, 126);
	Colour red = Colour(255, 0, 0);
	Colour green = Colour(255, 0, 255);
	//draw background
	window.drawIMG("assets/" + currentMap + "/" + currentMap + "_map.png", 100, 100, 400, 400);
	//draw arcs
	for(int i = 0; i < map.arcs.size(); i++){
		Arc a = map.arcs[i];
		if(a.inRoute){
			drawLine(window, a.p1->x, a.p1->y, a.p2->x, a.p2->y, red);
		} else {
			drawLine(window, a.p1->x, a.p1->y, a.p2->x, a.p2->y, white);
		}
		//ADD TEXT ARC WEIGHTS
		int mx = ( a.p1->x + a.p2->x )/ 2;
		int my = ( a.p1->y + a.p2->y )/ 2;
		std::string txt = std::to_string(a.length(map.routeMode));
		drawText(window, txt, mx, my, 30, 30);
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
	//draw button icons
	for(Button b : controlButtons){
		Colour bg = b.selected ? white :(b.hover ? darkGray : midGray);
		fillRect(window, b.xPos, b.yPos, b.width, b.height, bg);
		window.drawIMG(b.bgIm, b.xPos + 5 + b.imWidthPad, b.yPos + 5, b.width - 10 - 2*b.imWidthPad, b.height - 10);
	}
}

//event handler function - each frame execute one event from the queue
//
//
void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		for(int i = 0; i < map.points.size(); i++){
			if(map.points[i].hover){
				if(end != NULL){
					cleanMap();
				}
				if(map.points[i].selected){
					map.points[i].selected = false;
					cleanMap();
				} else {
					map.points[i].selected = true;
					if(start == NULL){
						start = &map.points[i];
					} else if(end == NULL){
						end = &map.points[i];
						setRoute(start, end);
					}
				}
			}
		}
		for(int i = 0; i < 6; i++){
			if(controlButtons[i].hover){
				controlButtons[i].Click();
			}
		}

	} else if(event.type == SDL_MOUSEBUTTONUP){
		for(int i = 0; i < 6; i++){
			controlButtons[i].UnClick();
		}
	}
}

//handle mouse position function - called upon window update
//
//
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
	for(int i = 0; i < 6; i++){
		controlButtons[i].hover = controlButtons[i].IsWithin(x, y);
	}
}

////////////////////////////////////////////
// FILE READING FUNCTIONS FOR MAP UPDATES //
////////////////////////////////////////////

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

//cleanses graph points and arcs, 'resets' variables to initial values
void cleanMap(){
	//clear points
	for(int i = 0; i < map.points.size(); i++){
		map.points[i].selected = false;
		map.points[i].visited = false;
		map.points[i].dist = std::numeric_limits<float>::infinity();
	}
	//clear arcs
	for(int i = 0; i < map.arcs.size(); i++){
		map.arcs[i].inRoute = false;
	}
	//reset start and end
	start = NULL;
	end = NULL;
}

//updates the current map image and graph by reading the new file
void updateMap(std::string newMap){
	if(currentMap != newMap){
		cleanMap();
		currentMap = newMap;
		map = Graph();
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
				//end of word
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
						map.arcs.push_back(Arc(&map.points[elements[0]], &map.points[elements[1]], arc_count++, bool(elements[2]), elements[3]));
					}
				}
				currentWord = "";
			} else {
				//add to word
				currentWord.append(1, c);
			}
			c = ifs.get();
		}
		ifs.close();
	} else {
		cleanMap();
	}
}

///////////////
// MAIN FUNC //
///////////////

//main function - entry point of program
//creates window
//loops event handling and rendering
int main(int argc, char *argv[]) {
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	updateMap("example");
	while (true) {
		handleMousePos();
		//while event exists in the queue, handle event and check for next item in queue
		int isAnotherEvent = window.pollForInputEvents(event);
		while (isAnotherEvent) {
			handleEvent(event, window);
			isAnotherEvent = window.pollForInputEvents(event);
		}
		draw(window);
		window.renderFrame();
	}
}

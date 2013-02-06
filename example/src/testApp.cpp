#include "testApp.h"
int frame = 0;
const static int col = 16;
const static int row = 16;
static int mode = 0;
const static int totalMode = 2;
//--------------------------------------------------------------
void testApp::setup(){
#ifdef TARGET_LINUX_ARM
	tcl.setup(256,"/dev/spidev0.0");
#else
	tcl.setup(5,50);
#endif
	pixel.allocate(256, 1, OF_IMAGE_COLOR);
	ofSetFrameRate(60);
}
void testApp::exit()
{
	tcl.exit();
}
//--------------------------------------------------------------
void testApp::update(){
	frame++;
	frame %= tcl.numLEDs();
	if(frame%400==0)
	{
		mode++;
		mode%=totalMode;
	}
	
	if(mode==0)
	{
		int i = 0 ;
		for(int y = 0 ; y < row ; y++)
		{
			for(int x = 0 ; x < col ; x++)
			{
				int index =  (x+(y*col))*3;
				ofColor c = ofColor::white;
#define ZIGZAG
#ifdef ZIGZAG
				if(y%2==1)
					
				{
					index = (((col-1)-x)+(y*col))*3;
				}
#endif
				if(frame == i)
				{
					
					pixel[index] = c.r;
					pixel[index+1] = c.g;
					pixel[index+2] = c.b;
				}
				else
				{
					pixel[index] = 0;
					pixel[index+1] = 0;
					pixel[index+2] = 0;
					
				}
				i++;
			}
		}
	}
	else if (mode ==1)
	{
		for(int i = 0 ; i < tcl.numLEDs(); i++)
		{
			int index = i*3;
			ofColor c = ofColor::fromHsb((i+frame)%255,255,255);
			pixel[index] = c.r;
			pixel[index+1] = c.g;
			pixel[index+2] = c.b;
		}
	}
	
	
	tcl.update(pixel);
}

//--------------------------------------------------------------
void testApp::draw(){
	int sizeX = 20;
	int sizeY = 20;
	for(int x = 0 ; x < col ; x++)
	{
		for(int y = 0 ; y < row ; y++)
		{
			ofPushStyle();
			int i = (x+(y*col))*3;
			
			
			ofSetColor(pixel [i], pixel [i+1], pixel [i+2]);
			ofRect((x)*sizeX,(y)*sizeY,sizeX,sizeY);
			ofPopStyle();
		}
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}
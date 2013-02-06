#include "testApp.h"
const static int row = 16;
const static int col = 16;
const static int numLED = col*row;
int SQAURE_ROOT =int(sqrt((float)col));
float ptSize = 5;
int counter = 0;

//--------------------------------------------------------------
void testApp::setup(){
	player.loadMovie("Bad_Apple.mov");
	player.play();
	player.setPaused(false);
	player.setLoopState(OF_LOOP_NORMAL);
	ofSetWindowShape(player.getWidth()*2, player.getHeight());

	ofSetFrameRate(25);
	ofSetLogLevel(OF_LOG_VERBOSE);

#ifdef TARGET_LINUX_ARM
	tcl.setup(numLED,"/dev/spidev0.0");
#else
	tcl.setup(5,50);
#endif
	pixel.allocate(numLED, 1, OF_IMAGE_COLOR);
	startThread();
}
void testApp::exit()
{
	
	stopThread();
	tcl.exit();
	ofLogVerbose("tcl")<< "close and clear led";
	

	
}
//--------------------------------------------------------------
void testApp::threadedFunction()
{
	while( isThreadRunning() != 0 ){
		if( lock() ){
			if(!lockPixel)
			{

				{
					tcl.update(pixel);
					usleep(1000);
				}
			}
			unlock();
		}
	}
}
//--------------------------------------------------------------
void testApp::update(){
	player.update();
	lockPixel = true;
	int sizeX = player.getWidth()/col;
	int sizeY = player.getHeight()/row;
	
	for(int x = 0 ; x < col ; x++)
	{
		for(int y = 0 ; y < row ; y++)
		{
			int i = (x+(y*col))*3;
			
			int i2 = ((x*sizeX)+((y*sizeY)*col*sizeX))*3;
			unsigned char *pixels = player.getPixels();
			pixel[i+1] = pixels[i2];
			pixel[i] = pixels[i2+1];
			pixel[i+2] = pixels[i2+2];
		}
		
	}
	lockPixel = false;
}

//--------------------------------------------------------------
void testApp::draw(){
	player.draw(player.getWidth(), 0);
#ifndef TARGET_LINUX_ARM
	int sizeX = player.getWidth()/col;
	int sizeY = player.getHeight()/row;
	for(int x = 0 ; x < col ; x++)
	{
		for(int y = 0 ; y < row ; y++)
		{
			ofPushStyle();
			int i = (x+(y*col))*3;
			
			
			ofSetColor(pixel [i+1], pixel [i], pixel [i+2]);
			ofRect((x)*sizeX,(y)*sizeY,sizeX,sizeY);
			ofPopStyle();
		}
	}
#endif
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
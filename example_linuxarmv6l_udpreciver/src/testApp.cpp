#include "testApp.h"
int frame = 0;
const static int col = 16;
const static int row = 16;
static int NUM_LED = col*row;
//static int mode = 0;
const static int totalMode = 2;
//--------------------------------------------------------------
void testApp::setup(){
#ifdef TARGET_LINUX_ARM
	tcl.setup(NUM_LED,"/dev/spidev0.0");
#else
	tcl.setup(5,50);
#endif
	pixel.allocate(col, row, OF_IMAGE_COLOR);
	ofSetFrameRate(60);
	
	//create the socket and bind to port 11999
	udpConnection.Create();
	udpConnection.Bind(5005);
	udpConnection.SetNonBlocking(true);
}
void testApp::exit()
{
	tcl.exit();
}
//--------------------------------------------------------------
void testApp::update(){
	int MAX_LEN = NUM_LED*3;
	char udpMessage[MAX_LEN];
	
	if(udpConnection.Receive(udpMessage,MAX_LEN)>0)
	{
		

//		for(int y = 0 ; y < row ; y++)
//		{
//			for(int x = 0 ; x < col ; x++)
//			{
//				int i = (x+(y*col))*3;
//				int i2 = (x+(y*col))*3;
//				//handle zigzag
////				if(y%2==1)
////				{
////					i2 = (((col-1)-x)+(y*col))*3;
////				}
//				//
//				pixel[i2] = udpMessage[i];
//				pixel[i2+1] = udpMessage[i+1];
//				pixel[i2+2] = udpMessage[i+2];
//				
//			}
//
//		}
		memcpy(pixel.getPixels(),udpMessage,MAX_LEN);
		for(int y = 0 ; y < row ; y++)
		{
			if(y%2==1)
			{
				int i=0, j=col-1;
				while(i<j)
				{

					unsigned char r = pixel[i*3+(y*col)*3];
					unsigned char g = pixel[i*3+1+(y*col)*3];
					unsigned char b = pixel[i*3+2+(y*col)*3];
					pixel[i*3+(y*col)*3] = pixel[j*3+(y*col)*3];
					pixel[i*3+1+(y*col)*3] = pixel[j*3+1+(y*col)*3];
					pixel[i*3+2+(y*col)*3] = pixel[j*3+2+(y*col)*3];

					pixel[j*3+(y*col)*3] = r;
					pixel[j*3+1+(y*col)*3] = g;
					pixel[j*3+2+(y*col)*3] = b;
					i++;
					j--;
				}
			}
		}

		tcl.update(pixel);
	}
}
//--------------------------------------------------------------
void testApp::draw(){
//	int sizeX = 20;
//	int sizeY = 20;
//	for(int x = 0 ; x < col ; x++)
//	{
//		for(int y = 0 ; y < row ; y++)
//		{
//			ofPushStyle();
//			int i = (x+(y*col))*3;
//			
//			
//			ofSetColor(pixel [i], pixel [i+1], pixel [i+2]);
//			ofRect((x)*sizeX,(y)*sizeY,sizeX,sizeY);
//			ofPopStyle();
//		}
//	}
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
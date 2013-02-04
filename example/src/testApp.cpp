#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	#ifdef TARGET_LINUX_ARM
	tcl.setup(256,"/dev/spidev0.0");
#else
	tcl.setup(5,50);
#endif
	pixel.allocate(256, 1, OF_IMAGE_COLOR);
	
}
void testApp::exit()
{
	tcl.exit();
}
//--------------------------------------------------------------
void testApp::update(){
	
	tcl.update(pixel);
}

//--------------------------------------------------------------
void testApp::draw(){

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
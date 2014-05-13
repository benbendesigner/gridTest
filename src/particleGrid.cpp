//
//  particleGrid.cpp
//  gridTest
//
//  Created by Ben Sorensen on 5/13/14.
//
//

#include "particleGrid.h"

//------------------------------------------------------------------
particleGrid::particleGrid(){
	attractPoints = NULL;
}

//------------------------------------------------------------------
void particleGrid::setMode(particleMode newMode){
	mode = newMode;
}

//------------------------------------------------------------------
void particleGrid::setAttractPoints( vector <ofPoint> * attract ){
	attractPoints = attract;
}

//------------------------------------------------------------------
void particleGrid::reset(){
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);
	
	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();
	
	vel.x = ofRandom(-3.9, 3.9);
	vel.y = ofRandom(-3.9, 3.9);
	
	frc   = ofPoint(0,0,0);
	
	scale = ofRandom(1.5, 3.0);
	
//	if( mode == PARTICLE_MODE_NOISE ){
//		//drag  = ofRandom(0.97, 0.99);
//		vel.y = fabs(vel.y) * 3.0; //make the particles all be going down
//	}else{
//		drag  = ofRandom(0.76, 0.8);
//        //vel.y = fabs(vel.y) * 3.0; //make the particles all be going down
//	}
}

//------------------------------------------------------------------
void particleGrid::update(){
    
	//1 - APPLY THE FORCES BASED ON WHICH MODE WE ARE IN
	
//	if( mode == PARTICLE_MODE_ATTRACT ){
//		ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
//		frc = attractPt-pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
//		frc.normalize(); //by normalizing we disregard how close the particle is to the attraction point
//		
//		vel *= drag; //apply drag
//		vel += frc * 0.6; //apply force
//	}
	

	if( mode == PARTICLE_MODE_NEAREST_POINTS ){
		
		if( attractPoints ){
            
			//1 - find closest attractPoint
			ofPoint closestPt;
			int closest = -1;
			float closestDist = 100000;
			
			for(unsigned int i = 0; i < attractPoints->size(); i++){
				float lenSq = ( attractPoints->at(i)-pos ).lengthSquared();
				if( lenSq < closestDist ){
					closestDist = lenSq;
					closest = i;
				}
			}
			
			//2 - if we have a closest point - lets calcuate the force towards it
			if( closest != -1 ){
				closestPt = attractPoints->at(closest);
				float dist = sqrt(closestDist);
				
				//in this case we don't normalize as we want to have the force proportional to distance
				frc = closestPt - pos;
                
				vel *= drag;
                
				//lets also limit our attraction to a certain distance and don't apply if 'f' key is pressed
				if( dist < 300 && dist > 40 && !ofGetKeyPressed('f') ){
					vel += frc * 0.003;
				}else{
					//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy.
					frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
					frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
					vel += frc * 0.4;
				}
				
			}
            
		}
		
	}
	
	
	//2 - UPDATE OUR POSITION
	
	pos += vel;
    
}

//------------------------------------------------------------------
void particleGrid::draw(){
    
	if( mode == PARTICLE_MODE_ATTRACT ){
		ofSetColor(255, 63, 180);
	}
	//ofCircle(pos.x, pos.y, scale * 4.0);
    ofSetRectMode(OF_RECTMODE_CENTER); //set rectangle mode to the center
    ofSetColor(0, 0, 85*scale);
    ofRect(pos.x, pos.y, (16*scale), (9*scale));

}

//--------------------------------------------------------------
void particleGrid::mousePressed(int x, int y, int button){
    ofSetColor(200,200,0);
    scale = 200;
}

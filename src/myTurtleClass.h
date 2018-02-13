//
//  myTurtleClass.h
//  Turtle_02_29_16_Mesh_Class
//
//  Created by Irving Angulo on 2/29/16.
//
//

#ifndef _myTurtleClass
#define _myTurtleClass

#include "ofMain.h"
#include <stdio.h>

class myTurtleClass {
    
public:
    
    //  Default Constructor
    myTurtleClass();
    
    //  Overload Constructor #1
    myTurtleClass(float angleOfTurn, float sizeOfSteps, ofVec3f startingCoordinates, ofVec3f setLookAt, ofVec3f setHeading, ofVec3f color);
    
    //  Overload Constructor #2
    myTurtleClass(float angle, float sizeOfStep, float startPosX, float startPosY, float startPosZ, float lookAtX, float lookAtY, float lookAtZ, float headingX, float headingY, float headingZ, float r, float g, float b);
    
    //  destructor
    ~myTurtleClass();
    
    // Accessor functions: these may come in handy. See https://www.youtube.com/watch?v=b9wialxvcVA&list=PL318A5EB91569E29A&index=21
    float getTheta() const;
    ofVec3f getCurrentCoords() const;
    
    //  Mutator functions   // change the parameters
    void setTheta(float);   // these work with myTurtleClass.setTheta, etc
    void setStepSize(float);
    void setCurrentCoordinates(ofVec3f);
    void setCurrentCoordinates(float x, float y, float z);
    void setColor(float r, float g, float b);
    void setColor(float r, float g, float b, float a);
    void commandTurtle(char lindenCharacter);
    void draw();
    void setup();
    void setDupVertex(bool setDupVertex);
    void reset(bool resetTurtles);
    void clearReset(bool clearResetTurtles);
    void setPrimitive(char setPrimitive);
    
    ofMesh meshInClass;
    // mesh
    
private:
    
    void pathFunction(ofVec3f positionCoordinates);    // draw function
    ofPath pathInClass;                     // path
    
    ofColor pathColor;
    
    
    ofVec3f rotateMatrix(float thetaInMatrix, ofVec3f ofVec3fSet, ofVec3f ofVec3fMultiplied);
    // rotation matrix function
    
    //Variables
    
    // store coordinates for push matrix. An array of ofVec3fs.
    vector<ofVec3f> storedCoordsArrays;
    vector<ofVec3f> stored_lookAt_Arrays;
    vector<ofVec3f> stored_heading_Arrays;
    vector<ofVec3f> stored_rightHand_Arrays;
    
    ofVec3f storeCoords;    //use this one to assign to the vector(arrayList)
    ofVec3f currentCoords;  // this one will be copied to the store coords
    ofVec3f storeVecs;      //will use to assign to the vector(arrayList)
    ofVec3f currentVecs;    // copy this one to the one that will be stored. this is the one that is intialized first
    ofVec3f lookAt, heading, rightHand, currentCoordinates;
    
    
    double stepSize, theta, cosTheta, sinTheta, thetaRestore;
    double xV, yV, zV; // in rotate function. x of ofVec3f, etc.
    float red, green, blue, alpha;
    bool dupVertexMesh;
    ofVec3f startingPosition, startingLookAt, startingHeading; // for reseting.
    
    ofMatrix4x4 rotateUHR;
    
};

#endif /* defined(_myTurtleClass) */

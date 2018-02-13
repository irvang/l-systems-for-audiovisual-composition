
//
//  myTurtleClass.cpp
//  Turtle_02_29_16_Mesh_Class
//
//  Created by Irving Angulo on 2/29/16.
//
//

#include "myTurtleClass.h"

//--------------------------------------------------------------
myTurtleClass::myTurtleClass(){
    // default constructor
    
    currentCoordinates = {0.0, 0.0, 0.0};
    lookAt = {1.0, 0.0, 0.0};
    heading = {0.0, 1.0, 0.0};
    rightHand = {0.0, 0.0, 1.0};
    theta = 90;
    stepSize = 10;
    
    
    ofColor pathColor(25, 200, 150);
    
    meshInClass.setMode(OF_PRIMITIVE_LINES);
    startingLookAt = lookAt; startingHeading = heading; startingPosition = currentCoordinates;
}
//--------------------------------------------------------------
myTurtleClass::myTurtleClass(float angleOfTurn, float sizeOfSteps, ofVec3f startingCoordinates, ofVec3f setLookAt, ofVec3f setHeading, ofVec3f color){
    
    //  Overload constructor #1
    
    theta = angleOfTurn;
    stepSize = sizeOfSteps;
    currentCoordinates = startingCoordinates;
    lookAt = setLookAt.normalize();;
    heading = setHeading.normalize();;
    rightHand = lookAt.getCrossed(heading);
    ofColor pathColor(color.x, color.y, color.z);
    
    meshInClass.enableColors();
    meshInClass.enableIndices();
    
    startingLookAt = setLookAt; startingHeading = setHeading; startingPosition = startingCoordinates;
    
    meshInClass.setMode(OF_PRIMITIVE_LINES);
    
}
//--------------------------------------------------------------
myTurtleClass::myTurtleClass(float angle, float sizeOfStep, float startPosX, float startPosY, float startPosZ, float lookAtX, float lookAtY, float lookAtZ, float headingX, float headingY, float headingZ, float r, float g, float b) {
    
    //  Overload constructor #2
    lookAt.set(lookAtX, lookAtY, lookAtZ);
    heading.set(headingX, headingY, headingZ);
    currentCoordinates.set(startPosX, startPosY, startPosZ);
    
    lookAt.normalize();
    heading.normalize();
    
    theta = angle;
    stepSize = sizeOfStep;
    rightHand = lookAt.getCrossed(heading);
    
    meshInClass.setMode(OF_PRIMITIVE_LINE_STRIP_ADJACENCY);
    
    startingLookAt = lookAt; startingHeading = heading; startingPosition = currentCoordinates;
    
}
//--------------------------------------------------------------
myTurtleClass::~myTurtleClass(){
    //  Destructor
}

//--------------------------------------------------------------
void myTurtleClass::setColor(float r, float g, float b){
    
    red = r; green = g; blue = b; alpha = 1;
}

//--------------------------------------------------------------
void myTurtleClass::setColor(float r, float g, float b, float a){
    
    red = r; green = g; blue = b; alpha = a;
}
//--------------------------------------------------------------
void myTurtleClass::draw(){
    
    //    pathInClass.draw();

    meshInClass.draw();
}

//--------------------------------------------------------------
void myTurtleClass::pathFunction(ofVec3f positionCoordinates){
    // My draw function
    
    meshInClass.addVertex(positionCoordinates);
    meshInClass.addColor(ofFloatColor(red, green, blue, alpha));
    
}
//--------------------------------------------------------------
float myTurtleClass::getTheta() const{
    return theta;
}
//--------------------------------------------------------------
ofVec3f myTurtleClass::getCurrentCoords() const{
    return currentCoordinates;
}
//--------------------------------------------------------------

//--------------------------------------------------------------
//  Mutator functions
void myTurtleClass::setTheta(float setTheta){
    theta = setTheta; // mutator
}
//--------------------------------------------------------------
void myTurtleClass::setStepSize(float setStepSize){
    stepSize = setStepSize; // mutator
}

//--------------------------------------------------------------
void myTurtleClass::setCurrentCoordinates(ofVec3f setCurrentCoordinates){
    currentCoordinates = setCurrentCoordinates; // mutator
    
}
//--------------------------------------------------------------
void myTurtleClass::setCurrentCoordinates(float x, float y, float z){
    currentCoordinates.set(x, y, z);
}
//--------------------------------------------------------------
void myTurtleClass::setDupVertex(bool setDupVertex){
    dupVertexMesh = setDupVertex;
    
}
//--------------------------------------------------------------


//--------------------------------------------------------------
//  Turtle
void myTurtleClass::commandTurtle(char lindenCharacter){
    
    // My drawing command instructions. It interprets either char or key input.
    
    switch(lindenCharacter)
    {
            //---------------------------------
        case '-':// - //   first add and then do the calculations
        {
            theta = -(abs(theta)); // take the absolute of theta and make it negative, in case theta is already negative
            lookAt = rotateMatrix(theta, heading, lookAt);// heading remains the same, and the output is the new lookAt. the unchanged one is in the middle. the changed one is at both ends.
            rightHand = lookAt.getCrossed(heading); // cross product of lookAt and new heading:  u x h
            // heading remains the same
        }break;
            //---------------------------------
        case '+':   // 43// +
        {
            theta = (abs(theta));
            lookAt = rotateMatrix(theta, heading, lookAt);
            
            lookAt.normalize();
            heading.normalize();
            
            rightHand = lookAt.getCrossed(heading);
            // heading remains the same
        }break;
            //---------------------------------
        case '|':   //124:// |  // rotates 180º around Ru (- +)
        {
            thetaRestore = theta; // store theta in thetaStore
            theta = 180;// theta becomes 180 to run the matrix
            //            theta = (abs(theta));
            
            lookAt = rotateMatrix(theta, heading, lookAt);
            
            lookAt.normalize();
            heading.normalize();
            
            rightHand = lookAt.getCrossed(heading);
            theta = thetaRestore; // restores theta to initial angle
            
        }break;
            //---------------------------------
        case 'u':   //117:// u, up
        {
            theta = (abs(theta));
            lookAt = rotateMatrix(theta, rightHand, lookAt);
            heading = rotateMatrix(theta, rightHand, heading);
            
            lookAt.normalize();
            heading.normalize();
            // rightHand remains unchanged
        }break;
            //---------------------------------
        case 'd':   //100:// d, up
        {
            theta = -(abs(theta));
            lookAt = rotateMatrix(theta, rightHand, lookAt);
            heading = rotateMatrix(theta, rightHand, heading);
            
            lookAt.normalize();
            heading.normalize();
            // rightHand remains unchanged
        }break;
            //---------------------------------
        case 'l':   // l  // , roll left
        {
            theta = -(abs(theta));
            heading = rotateMatrix(theta, lookAt, heading);
            
            lookAt.normalize();
            heading.normalize();
            
            rightHand = lookAt.getCrossed(heading);
            // lookAt remains the same
        }break;
            //---------------------------------
        case '/':   //47:// /, roll right
        {
            theta = (abs(theta)); // take the absolute of theta, in case theta may be negative
            heading = rotateMatrix(theta, lookAt, heading);
            
            lookAt.normalize();
            heading.normalize();
            
            rightHand = lookAt.getCrossed(heading);
            // lookAt remains the same
        }break;
            //---------------------------------
        case 'f':       //102:// f, draw forward
        case 'h':       // h also draws forward. used for some algorightms that use f2. So that h=f2
        {
            // coordinates becomes coordinates plus vector. Vector should be in one direction.
            
            if(dupVertexMesh == true){pathFunction(currentCoordinates);}// enable or disable depending on wether to double the vertices. More accurate and akin to the turtle graphics, but less variety of images with the mesh
            
            currentCoordinates = currentCoordinates + (stepSize * lookAt);
            pathFunction(currentCoordinates);
        }break;
            //---------------------------------
        case 'g':   //103:// g
        {
            //            pathFunction(currentCoordinates);
            if(dupVertexMesh == true){pathFunction(currentCoordinates);}
            currentCoordinates = currentCoordinates - (stepSize * lookAt);
            pathFunction(currentCoordinates);
        }break;
            //---------------------------------
        case 'F':   //70:// F, move forward but not draw
        {
            currentCoordinates = currentCoordinates + (stepSize * lookAt);
            pathInClass.moveTo(currentCoordinates);
        }break;
            //---------------------------------
        case 'G':   //71:// G, move forward but not draw
        {
            currentCoordinates = currentCoordinates - (stepSize * lookAt);
            pathInClass.moveTo(currentCoordinates);
        }break;
            //---------------------------------
        case '[':   //91:// [   push matrix  //
        {
            storedCoordsArrays.push_back(currentCoordinates);
            stored_lookAt_Arrays.push_back(lookAt);
            stored_heading_Arrays.push_back(heading);
            stored_rightHand_Arrays.push_back(rightHand);
            
        }break;
            //---------------------------------
        case ']':   // 93:// ]   pop matrix
        {
            if(storedCoordsArrays.size() != 0){
                
                //  first get last value on vectorArray, and then delete it with pop_back
                currentCoordinates = storedCoordsArrays.back();
                storedCoordsArrays.pop_back();// delete after retrieving
                pathInClass.moveTo(currentCoordinates);
                
                lookAt = stored_lookAt_Arrays.back();
                stored_lookAt_Arrays.pop_back();
                //                lookAt.normalize();
                
                heading = stored_heading_Arrays.back();
                stored_heading_Arrays.pop_back();
                //                heading.normalize();
                
                rightHand = stored_rightHand_Arrays.back();
                stored_rightHand_Arrays.pop_back();
                
                //restore the coordinates and then move to that position without drawing
            }
            
        }break;
            //---------------------------------
        default:
        {
        }
            break;
            //---------------------------------
    }   //end switch
}

//--------------------------------------------------------------
//  Rotation Matrix
ofVec3f myTurtleClass::rotateMatrix(float thetaInMatrix, ofVec3f ofVec3fSet, ofVec3f ofVec3fMultiplied){
    
    /* This function returns an ofVec3f which replaces the ofVec3fMultiplied. Two values are changed and one is kept the same. The one that is kept the same is the axis over which we rotate.
     For example:
     theta = (abs(theta));
     lookAt = rotateMatrix(theta, heading, lookAt);
     rightHand = lookAt.getCrossed(heading);
     // heading remains the same                                    */
    
    //  ofVec3fSet is the one that will remain the same
    //  ofVec3fMultiplied is the vector that rotates
    //  rightHand vector rotates by "default," since it is a cross product of the other two vectors
    
    //  theta is passed through the second argument thetaInMatrix. That is how cosTheta and sinTheta are generated.
    cosTheta = cos(ofDegToRad(thetaInMatrix)); sinTheta = sin(ofDegToRad(thetaInMatrix));
    
    //  make cos & sin = 0 if they are less than... //This is to avoid 0 approximation error
    if((cosTheta < 0.000000045) && (cosTheta > -0.000000045)){cosTheta = 0.0;}
    if((sinTheta < 0.000000045) && (sinTheta > -0.000000045)){sinTheta = 0.0;}
    
    //  this is unnecessary, and maybe inefficient, but declutters the code quite a bit in the matrix below
    xV = ofVec3fSet.x;
    yV = ofVec3fSet.y;
    zV = ofVec3fSet.z;
    
    
    rotateUHR.set(
                  //    roll left 'l' -theta, roll right '/' +theta // like rotate over x
                  
                  cosTheta + (xV * xV * (1 - (cosTheta))),//first row
                  (xV * yV * (1-cosTheta)) - (zV * sinTheta),
                  (xV * zV * (1-cosTheta)) + (yV * sinTheta),
                  0.0,
                  (yV * xV * (1-cosTheta)) + (zV * sinTheta), //second row
                  cosTheta + (yV * yV * (1 - (cosTheta))),
                  (yV * zV * (1-cosTheta)) - (xV * sinTheta),
                  0.0,
                  (zV * xV * (1-cosTheta)) - (yV * sinTheta), //third row
                  (zV * yV * (1-cosTheta)) + (xV * sinTheta),
                  cosTheta + (zV * zV * (1 - (cosTheta))),
                  0.0,
                  0.0, 0.0, 0.0, 1.0);        // fourth row, it is discarded
    
    //  in openFrameworks, for multiplication, the matrix values come after the vector, contrary to how it works in math, see documentation.
    
    //  returns the value of the new ofVec3f that is being altered.
    //  probably can be passed by reference
    return ofVec3fMultiplied * rotateUHR;
}
//--------------------------------------------------------------
void myTurtleClass::reset(bool resetTurtles){
    
    if(resetTurtles == true){
        
        lookAt = startingLookAt;
        heading = startingHeading;
        currentCoordinates = startingPosition;
        pathInClass.moveTo(currentCoordinates);
    }
}
//--------------------------------------------------------------
void myTurtleClass::clearReset(bool clearResetTurtles){
    
    if(clearResetTurtles == true){
        
        //Clear path and restore starting settings
        pathInClass.moveTo(currentCoordinates);
        pathInClass.clear();
        
        lookAt = startingLookAt;
        heading = startingHeading;
        rightHand = lookAt.getCrossed(heading);
        currentCoordinates = startingPosition;
        
        meshInClass.clear();
        
        //Reset pop-push matrix
        stored_rightHand_Arrays.clear();    stored_heading_Arrays.clear();
        storedCoordsArrays.clear();         stored_rightHand_Arrays.clear();
    }
}
//--------------------------------------------------------------
void myTurtleClass::setPrimitive(char setPrimitive){
    //  Set different primitive drawing modes
    
    switch(setPrimitive)
    {
            //        case '1': {meshInClass.setMode(OF_PRIMITIVE_LINE_LOOP);} break; // did not make much sense
        case '1': {meshInClass.setMode(OF_PRIMITIVE_LINE_STRIP);} break;
        case '2': {meshInClass.setMode(OF_PRIMITIVE_LINE_STRIP_ADJACENCY);} break;
        case '3': {meshInClass.setMode(OF_PRIMITIVE_LINES);} break;
        case '4': {meshInClass.setMode(OF_PRIMITIVE_LINES_ADJACENCY);} break;
            //        case '6': {meshInClass.setMode(OF_PRIMITIVE_PATCHES);} break; // never did anything
        case '5': {meshInClass.setMode(OF_PRIMITIVE_POINTS);} break;
        case '6': {meshInClass.setMode(OF_PRIMITIVE_TRIANGLE_FAN);} break;
        case '7': {meshInClass.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);} break;
        case '8': {meshInClass.setMode(OF_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY);} break;
        case '9': {meshInClass.setMode(OF_PRIMITIVE_TRIANGLES);} break;
        case '0': {meshInClass.setMode(OF_PRIMITIVE_TRIANGLES_ADJACENCY);} break;
    }
}
//--------------------------------------------------------------
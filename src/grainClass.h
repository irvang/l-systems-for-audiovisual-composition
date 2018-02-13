//
//  grainClass.h
//  turtle_newClass_03_05_16
//
//  Created by Irving Angulo on 3/5/16.
//
//

#ifndef _grainClass
#define _grainClass

#include "ofMain.h"
#include <stdio.h>
#include "ofxMaxim.h"

class grainClass {
    
public:
    
    //default constructor
    grainClass();
    
    //Overload Constructor: passes values at construction of object
    grainClass(double freq, double amp, double pan, bool doSynth, unsigned long lengthOfWindow);
    
    //Destructor
    ~grainClass();
//    void destruct();
    
    // Accessor functions: getThis, getThat, etc.
    double get_mFreq() const;// get mFreq
    double get_mAmp() const;
    long get_mWindowPos() const;
    bool get_mDoSynth() const;
    double get_mPan() const;
    unsigned long get_dur() const;
    string getWinType() const;
    string getWaveType() const;
    
    //Mutator functions: set this, set that, etc.
    void set_mFreq(double);
    void set_mAmp(double);
    void set_mWindowPos(long);
    void set_mDoSynth(bool);
    void set_mPan(double);
    void set_dur(unsigned long);
    void setWinType(string mWindowType);
    void setWaveType(string mWaveType);
    
    // oscillator and mixer
    maxiOsc oscInSynth;
    maxiMix mixerInSynth;
    
private:
    
    double mFreq;
    double mAmp;
    bool mDoSynth;
    double mPan;
    
    
    long mWindowPos;
    unsigned long dur;
    string windowType, waveType;
};


#endif /* defined(_grainClass) */

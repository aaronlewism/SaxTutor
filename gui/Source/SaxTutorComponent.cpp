/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  4 Nov 2012 6:00:38pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#define BELLEBONNESAGE_COMPILE_INLINE
#include "SaxTutorComponent.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SaxTutorComponent::SaxTutorComponent ()
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 600);

    //Add a portrait page to the score.
    myScore.Canvases.Add() = new Score::Page;
    myScore.Canvases.z()->Dimensions = bbs::Paper::Portrait(bbs::Paper::Letter);
  
    /*Add some rectangles for the score to paint. Note this is just a custom
    member that was created to demonstrate how to pass information to the painter.
    There is nothing intrinsic to the Score about painting rectangles.*/
    const bbs::number GeometricConstant = 1.2;
    for(bbs::number i = 0.01; i < 8.0; i *= GeometricConstant)
      myScore.RectanglesToPaint.Add() = bbs::Rectangle(bbs::Vector(i, i), bbs::Vector(i, i) *
	GeometricConstant);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SaxTutorComponent::~SaxTutorComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SaxTutorComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]
    
    bbsJuceProperties.PageArea = bbs::RectangleInt(0,0,600,800);
    bbsJuceProperties.PageVisibility = bbs::RectangleInt(0,0,600,800);
    bbsJuceProperties.PageDimensions = myScore.Canvases.z()->Dimensions;
    bbsJuceProperties.IndexOfCanvas = 0;
    bbsJuceProperties.ComponentContext = this;
    bbsJuceProperties.GraphicsContext = &g;
    g.fillAll (Colours::white);

    //bbsJuce.paint(&myScore, &bbsJuceProperties);
    myScore.Create<bbs::JUCE>(bbsJuceProperties);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SaxTutorComponent::resized()
{
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SaxTutorComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="800" initialHeight="600">
  <BACKGROUND backgroundColour="ffffffff"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

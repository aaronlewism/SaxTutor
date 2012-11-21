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
bbs::String DetermineResourcePath();
void LoadResources(Score*);

//[/MiscUserDefs]

//==============================================================================
SaxTutorComponent::SaxTutorComponent ()
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 600);

    //[Constructor] You can add your own custom stuff here..
		//Add a page to the score
		myScore.Canvases.Add() = new Score::Page;
    myScore.Canvases.z()->Dimensions = bbs::Measurement<bbs::Units::Point>(getWidth(), getHeight());

		//Load fonts and graphs
		LoadResources(&myScore);
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
    
    g.fillAll (Colours::white);

    //bbsJuce.paint(&myScore, &bbsJuceProperties);
    
    //[UserPaint] Add your own custom painting code here..
		bbsJuceProperties.PageArea = bbs::RectangleInt(0,0,getWidth(),getHeight());
    bbsJuceProperties.PageVisibility = bbs::RectangleInt(0,0,getWidth(), getHeight());
    bbsJuceProperties.PageDimensions = myScore.Canvases.z()->Dimensions;
    bbsJuceProperties.IndexOfCanvas = 0;
    bbsJuceProperties.ComponentContext = this;
    bbsJuceProperties.GraphicsContext = &g;

		myScore.Create<bbs::JUCE>(bbsJuceProperties);
    //[/UserPaint]
}

void SaxTutorComponent::resized()
{
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
bbs::String DetermineResourcePath()
{
  bbs::String dummy;
  if(bbs::File::Read("./GentiumBasicRegular.bellefont", dummy))
    return "./";
  else if(bbs::File::Read("../Resources/GentiumBasicRegular.bellefont", dummy))
    return "../Resources/";
  else if(bbs::File::Read("./Resources/GentiumBasicRegular.bellefont", dummy))
    return "./Resources/";
	else if(bbs::File::Read("/home/amlewis/SaxTutor/gui/Resources/GentiumBasicRegular.bellefont", dummy))
		return "/home/amlewis/SaxTutor/gui/Resources/";
  else
    prim::c >> "Path to resources could not be determined. (SaxTutorComponent.cpp::DetermineResourcePath()";
  return "";
}

void LoadResources(Score* myScore)
{
  //Find the font path.
  bbs::String Path = DetermineResourcePath();
  if(!Path)
    return;
    
  //Load some typefaces into the font.
  bbs::String Joie = Path; Joie << "Joie.bellefont";
  bbs::Array<bbs::byte> a;
  if(bbs::File::Read(Joie, a))
    myScore->myFont.Add(bbs::Font::Special1)->ImportFromArray(&a.a());

  //Load a graph.
  bbs::String Doc = Path; Doc << "ChopinPreludeOp28No20.mgf";
  bbs::graph::MusicSerial s;
  bbs::File::Read(Doc, s);
  bbs::UUID Version(1, 0);
  if(!s.ChecksumValid()) return;
  bbs::c >> "Loaded graph\n";
  myScore->g = new bbs::graph::MusicGraph;
  myScore->g->Serialize(s, bbs::Serial::Reading, Version);
	myScore->SerializeGraph();
  bbs::c >> "Serialized graph\n";
}
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

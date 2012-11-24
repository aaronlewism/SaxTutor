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
#define BELLEBONNESAGE_COMPILE_INLINE
#include "../bbs/Examples/Resources.h"
#include "tinyxml2.h"
#include "SaxTypes.h"
//[/Headers]

#include "SaxTutorComponent.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace bbs;
using namespace tinyxml2;
bbs::String DetermineResourcePath();
void LoadResources(Score*);
bool LoadSong();

//[/MiscUserDefs]

//==============================================================================
SaxTutorComponent::SaxTutorComponent ()
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 600);

    //[Constructor] You can add your own custom stuff here..
		//Load fonts and graphs
		LoadResources(&myScore);

		myScore.Canvases.z()->Dimensions = bbs::Measurement<bbs::Units::Point>(800,600);
		myScore.UpdatePiece();

		//TODO: Clean this shit up, for testing only
		LoadSong();
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
    bbs::c >> "Path to resources could not be determined. (SaxTutorComponent.cpp::DetermineResourcePath()";
  return "";
}


void LoadResources(Score* myScore)
{   
  //Load some typefaces into the font.
  myScore->myFont.Add(bbs::Font::Special1)->ImportFromArray((bbs::byte*)Resources::joie_bellefont);

  //Load a graph.
	bbs::File::Read(DetermineResourcePath() << "ChopinPreludeOp28No20.xml", myScore->graphXML);

	myScore->LoadTypeface();
}

bool LoadSong()
{
	XMLDocument doc;
	//TODO: Choose what song to load
	if (doc.LoadFile(DetermineResourcePath() << "Songs/DrunkenLullabies.xml")) {
		//TODO: Error
		//Some XML file reading error occured.
		return false;
	}

	//Find the tenr part for the song
	//TODO: Confirm it's P4 for all songs
	XMLElement* partRoot = NULL;
	for (XMLElement* cur = doc.FirstChildElement("score-partwise")->FirstChildElement("part");
			 cur != NULL; cur = cur->NextSiblingElement("part")) {
		if (cur->Attribute("id", "P4")) {
			partRoot = cur;
			break; //found Tenrz
		}
	}

	//Tenr part not found
	if (partRoot == NULL) {
		//TODO: error
		return false;
	}

  //Parse part;
	for (XMLElement* curMeasure = partRoot->FirstChildElement("measure");
			 curMeasure != NULL;
			 curMeasure = curMeasure->NextSiblingElement("measure")) {
		bbs::c >> "Measure #: " << curMeasure->IntAttribute("number") << "\n";
	}

	return true;
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

/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  1 May 2011 12:12:59pm

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
#include "MainComponent.h"
#include "../bbs/BelleBonneSage.h"

namespace bbs {
	using namespace prim;
	using namespace prim::planar;
	using namespace bellebonnesage;
	using namespace bellebonnesage::painters;
}

//Subclass Score from bellebonnesage::Portfolio
struct Score : public bbs::Portfolio
{
  //An array of rectangles to paint.
  bbs::Array<bbs::Rectangle> RectanglesToPaint;

  /*Subclass Page from bellebonnesage::Canvas. Note that Page is a class inside
  a class, so it is really a Score::Page; however, this it is not necessary to
  do it this way. It just logically groups the Page class with the Score to
  which it pertains.*/
  struct Page : public bbs::Canvas
  {
    //This method gets called once per canvas.
    void Paint(bbs::Painter& Painter, bbs::Portfolio& Portfolio)
    {
      /*Since we need access to the Score (as opposed to base class Portfolio)
      in order to draw the rectangles, we can forward the paint call to a custom
      paint method which uses a Score& instead of a Portfolio&.*/
      Paint(Painter, dynamic_cast<Score&>(Portfolio));
    }
    
    //Custom paint method with score.
    void Paint(bbs::Painter& Painter, Score& Score)
    {
      //Print-out which page is being painted.
      bbs::c >> "Painting page: " << Painter.GetPageNumber();
      
      //Paint each rectangle in the rectangle array.
      for(bbs::count i = 0; i < Score.RectanglesToPaint.n(); i++)
      {
        /*Create an empty path. A path is a vector graphics object containing
        a list of core instructions: move-to (start new path), line-to,
        cubic-to (Bezier curve), and close-path. Generally, multiple subpaths
        are interpreted by the rendering targets according to the zero-winding
        rule.*/
        bbs::Path p;
        
        /*Add the rectangle shape to the path. The Shapes class contains several
        primitive building methods.*/
        bbs::Shapes::AddRectangle(p, Score.RectanglesToPaint[i]);
        
        //Alternate green fill with blue stroke.
        if(i % 2 == 0)
          Painter.SetFill(bbs::Colors::green);
        else
          Painter.SetStroke(bbs::Colors::blue, 0.01);
        
        //Draw the path, separating the fills and strokes by page.
        if(i % 2 == Painter.GetPageNumber())
          Painter.Draw(p);
      }
    }
  };
};


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainComponent::MainComponent ()
    : helloWorldLabel (0),
      quitButton (0)
{
    addAndMakeVisible (helloWorldLabel = new Label (String::empty,
                                                    L"Sax Tutor!"));
    helloWorldLabel->setFont (Font (40.0000f, Font::bold));
    helloWorldLabel->setJustificationType (Justification::centred);
    helloWorldLabel->setEditable (false, false, false);
    helloWorldLabel->setColour (Label::textColourId, Colours::black);
    helloWorldLabel->setColour (TextEditor::textColourId, Colours::black);
    helloWorldLabel->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (quitButton = new TextButton (String::empty));
    quitButton->setButtonText (L"Quit");
    quitButton->addListener (this);

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 800);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MainComponent::~MainComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (helloWorldLabel);
    deleteAndZero (quitButton);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffc1d0ff));

    g.setColour (Colours::white);
    g.fillPath (internalPath1);
    g.setColour (Colour (0xff6f6f6f));
    g.strokePath (internalPath1, PathStrokeType (5.2000f));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainComponent::resized()
{
    helloWorldLabel->setBounds (152, 80, 296, 48);
    quitButton->setBounds (getWidth() - 176, getHeight() - 60, 120, 32);
    internalPath1.clear();
    internalPath1.startNewSubPath (136.0f, 80.0f);
    internalPath1.quadraticTo (176.0f, 24.0f, 328.0f, 32.0f);
    internalPath1.quadraticTo (472.0f, 40.0f, 472.0f, 104.0f);
    internalPath1.quadraticTo (472.0f, 192.0f, 232.0f, 176.0f);
    internalPath1.lineTo (184.0f, 216.0f);
    internalPath1.lineTo (200.0f, 168.0f);
    internalPath1.quadraticTo (96.0f, 136.0f, 136.0f, 80.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == quitButton)
    {
        //[UserButtonCode_quitButton] -- add your button handler code here..

        JUCEApplication::quit();

        //[/UserButtonCode_quitButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="600" initialHeight="800">
  <BACKGROUND backgroundColour="ffc1d0ff">
    <PATH pos="0 0 100 100" fill="solid: ffffffff" hasStroke="1" stroke="5.19999981, mitered, butt"
          strokeColour="solid: ff6f6f6f" nonZeroWinding="1">s 136 80 q 176 24 328 32 q 472 40 472 104 q 472 192 232 176 l 184 216 l 200 168 q 96 136 136 80 x</PATH>
  </BACKGROUND>
  <LABEL name="" id="be4f6f2e5725a063" memberName="helloWorldLabel" virtualName=""
         explicitFocusOrder="0" pos="152 80 296 48" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="Hello World!" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="40" bold="1" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="bcf4f7b0888effe5" memberName="quitButton" virtualName=""
              explicitFocusOrder="0" pos="176R 60R 120 32" buttonText="Quit"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

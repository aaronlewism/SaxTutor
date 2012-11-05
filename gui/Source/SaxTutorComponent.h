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

#ifndef __JUCER_HEADER_SAXTUTORCOMPONENT_SAXTUTORCOMPONENT_9EB00C00__
#define __JUCER_HEADER_SAXTUTORCOMPONENT_SAXTUTORCOMPONENT_9EB00C00__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
//[/Headers]

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


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SaxTutorComponent  : public Component
{
public:
    //==============================================================================
    SaxTutorComponent ();
    ~SaxTutorComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Score myScore;
		bbs::JUCE bbsJuce;
		//[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaxTutorComponent);
};


#endif   // __JUCER_HEADER_SAXTUTORCOMPONENT_SAXTUTORCOMPONENT_9EB00C00__

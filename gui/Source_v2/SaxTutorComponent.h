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
#include "../bbs/BelleBonneSage.h"

namespace bbs {
	using namespace prim;
	using namespace prim::planar;
	using namespace bellebonnesage;
	using namespace bellebonnesage::painters;
}

using namespace bbs;

//Subclass Score from bellebonnesage::Portfolio
struct Score : public bbs::Portfolio
{
	//bbs::graph::MusicGraph* g = NULL;
	bbs::String graphXML;
	bbs::Font myFont;

	~Score() {
		Canvases.RemoveAndDeleteAll();
	}

  /*Subclass Page from bellebonnesage::Canvas. Note that Page is a class inside
  a class, so it is really a Score::Page; however, this it is not necessary to
  do it this way. It just logically groups the Page class with the Score to
  which it pertains.*/
  struct Page : public bbs::Canvas
  {
		bbs::modern::House h;
    bbs::modern::Cache c;
		const bbs::Typeface* t;

		Page() {
			t = NULL;
		}

    //This method gets called once per canvas.
    void Paint(bbs::Painter& Painter, bbs::Portfolio& Portfolio)
    {
      /*Since we need access to the Score (as opposed to base class Portfolio)
      in order to draw the rectangles, we can forward the paint call to a custom
      paint method which uses a Score& instead of a Portfolio&.*/
      Paint(Painter, dynamic_cast<Score&>(Portfolio));
    }
    
    //Custom paint method with score.
    void Paint(bbs::Painter& Painter, Score& score)
    {
			if (t == NULL) {
				t = score.myFont.GetTypeface(bbs::Font::Special1);
				c.Create(h, *t);
			}

			bbs::graph::MusicGraph* g = new bbs::graph::MusicGraph;
			bbs::graph::XML::Read(g, score.graphXML);

			bbs::number SystemWidth = Dimensions.x * 0.85;
			bbs::number SpaceHeight = 0.065;
			bbs::number SystemWidthSpaces = SystemWidth / SpaceHeight;

			//bbs::c >> "Outputting 1\n";
      bbs::modern::Piece p;
			p.Initialize(g, h, c, *t, score.myFont);
			p.Typeset(true);
      bbs::List<bbs::modern::System> Systems;
      p.CreateSystems(Systems, SystemWidthSpaces, SystemWidthSpaces);
			bbs::Vector BottomLeft = bbs::Vector(Dimensions.x * 0.075, Dimensions.y * 0.925);
      for(bbs::count i = 0; i < Systems.n(); i++)
      {
				Systems[i].CalculateSpacing(10,
					(i < Systems.n() - 1 ? SystemWidthSpaces : 0.0));
        BottomLeft -= bbs::Vector(0.0, Systems[i].Bounds.Height() * SpaceHeight);
				Systems[i].Paint(Painter, BottomLeft, SpaceHeight);
      }
		
			p.ClearTypesetting();
			//Systems.Clear();
			//s2.Clear();
    }
  };

	private:
		bbs::graph::MusicSerial s;
};


//[/Headers]

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
		bbs::JUCE::Properties bbsJuceProperties;
		//[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaxTutorComponent);
};


#endif   // __JUCER_HEADER_SAXTUTORCOMPONENT_SAXTUTORCOMPONENT_9EB00C00__

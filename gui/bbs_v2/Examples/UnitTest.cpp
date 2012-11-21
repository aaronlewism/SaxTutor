/*
  ==============================================================================

  Copyright 2007-2012 William Andrew Burnson. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

     1. Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY WILLIAM ANDREW BURNSON ''AS IS'' AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
  EVENT SHALL WILLIAM ANDREW BURNSON OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  The views and conclusions contained in the software and documentation are
  those of the authors and should not be interpreted as representing official
  policies, either expressed or implied, of William Andrew Burnson.

  ------------------------------------------------------------------------------

  This file is part of Belle, Bonne, Sage --
    The 'Beautiful, Good, Wise' C++ Vector-Graphics Library for Music Notation 

  ==============================================================================
*/

#define BELLEBONNESAGE_COMPILE_INLINE
#include "../BelleBonneSage.h"
#include "Resources.h"

//Use some namespaces to help with scoping.
using namespace prim;
using namespace bellebonnesage;
using namespace bellebonnesage::painters;
using namespace prim::planar;

//An example of deriving a Portfolio and Canvas.
struct Score : public Portfolio
{
  //Font used by the typesetter
  Font ScoreFont;

  //Music graph to typeset
  graph::MusicGraph* Music;
  
  //House style
  modern::House HouseStyle;
  
  //Notation cache
  modern::Cache Cache;
  
  //Piece object
  modern::Piece Piece;
  
  //Array of systems produced by the piece
  Array<modern::Piece::System> Systems;

  ///Default constructor
  Score() : Music(new graph::MusicGraph)
  {
    //Load the font from the resource file.
    ScoreFont.Add(Font::Special1)->ImportFromArray(
      (byte*)Resources::joie_bellefont);
  
    //Load the score from the resource file.
    graph::XML::Read(Music, String((byte*)Resources::chopin_xml,
      Resources::chopin_xmlSize));
    
    //Get the notation typeface.
    const Typeface& NotationTypeface = *ScoreFont.GetTypeface(Font::Special1);
    
    //Create the cache.
    Cache.Create(HouseStyle, NotationTypeface);
    
    //Initialize the piece.
    Piece.Initialize(Music, HouseStyle, Cache, NotationTypeface, ScoreFont);
    
    //Prepare the systems.
    Piece.Prepare(Systems, 6.5, 6.5);
  }
  
  //Page of the score
  struct Page : public Canvas
  {
    //Page paint method
    virtual void Paint(Painter& Painter, Portfolio& Portfolio)
    {
      //Get the subclassed score.
      Score& s = dynamic_cast<Score&>(Portfolio);
      
      //Paint each system with descending y-position.
      for(count i = 0; i < s.Systems.n(); i++)
      {
        //Translate the system.
        Painter.Translate(Vector(1.0, 8.0 - (number)i * 1.5));
        
        //Paint the system.
        s.Piece.Paint(&Painter, s.Systems[i]);
        
        //Undo the translation.
        Painter.Revert();
      }
    }
  };
};

int main()
{
  //Create a score.
  Score MyScore;

  //Add a page to the score.
  MyScore.Canvases.Add() = new Score::Page;

  //Set the PDF-specific properties.
  PDF::Properties PDFSpecificProperties("UnitTest.pdf");

  //Write the score to PDF.
  MyScore.Create<PDF>(PDFSpecificProperties);

  //Note success to console window.
  c >> "Successfully wrote '" << PDFSpecificProperties.Filename << "'.";

  //Add a blank line to the output.
  c++;
  
  return 0;
}

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

#define PRIM_WITH_TIMER
#define BELLEBONNESAGE_COMPILE_INLINE
#include "../BelleBonneSage.h"

//Use some namespaces to help with scoping.
using namespace prim;
using namespace bellebonnesage;
using namespace bellebonnesage::painters;
using namespace prim::planar;

//Font which will be loaded.
Font MyFont;

//Graph to load.
graph::MusicGraph* g = 0;

//An example of deriving a Portfolio and Canvas.
struct Score : public Portfolio
{
  struct Page : public Canvas
  {
    virtual void Paint(Painter& Painter, Portfolio& Portfolio)
    {
      modern::House h;
      modern::Cache c;
      const Typeface& t = *MyFont.GetTypeface(Font::Special1);
      c.Create(h, t);
      modern::Piece p(g, h, c, t, MyFont);
      Array<modern::Piece::System> Systems;
      p.Prepare(Systems, 6.5, 6.5);
      //p.Parse();
      for(count i = 0; i < Systems.n(); i++)
      {
        Painter.Translate(Vector(1.0, 8 - (number)i * 1.5));
        p.Paint(&Painter, Systems[i]);
        Painter.Revert();
      }
    }
  };
};

String DetermineResourcePath()
{
  String Dummy;
  if(File::Read("./GentiumBasicRegular.bellefont", Dummy))
    return "./";
  else if(File::Read("../Resources/GentiumBasicRegular.bellefont", Dummy))
    return "../Resources/";
  else if(File::Read("./Resources/GentiumBasicRegular.bellefont", Dummy))
    return "./Resources/";
  else
    prim::c >> "Path to resources could not be determined.";
  return "";
}

void LoadResources()
{
  //Find the font path.
  String Path = DetermineResourcePath();
  if(!Path)
    return;
    
  //Load some typefaces into the font.
  String Joie = Path; Joie << "Joie.bellefont";
  Array<byte> a;
  if(File::Read(Joie, a))
    MyFont.Add(Font::Special1)->ImportFromArray(&a.a());

  //Load a graph.
  String Doc = Path; Doc << "ChopinPreludeOp28No20.mgf";
  graph::MusicSerial s;
  File::Read(Doc, s);
  UUID Version(1, 0);
  if(!s.ChecksumValid()) return;
  c >> "Loaded graph";
  g = new graph::MusicGraph;
  g->Serialize(s, Serial::Reading, Version);
}

int main()
{
  //Load the fonts.
  LoadResources();
  
  //Create a score.
  Score MyScore;

  //Add a page to the score.
  MyScore.Canvases.Add() = new Score::Page;

  //Set to letter landscape.
  MyScore.Canvases.z()->Dimensions = Inches(8.5, 11.0);

  //Set the PDF-specific properties.
  PDF::Properties PDFSpecificProperties;
  PDFSpecificProperties.Filename = "Chopin2.pdf";
  
  //Write the score to PDF.
  MyScore.Create<PDF>(PDFSpecificProperties);

  //Note success to console window.
  prim::c >> "Successfully wrote '" << PDFSpecificProperties.Filename << "'.";

  //Add a blank line to the output.
  prim::c++;
  
  return 0;
}

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

//Use some namespaces to help with scoping.
using namespace prim;
using namespace bellebonnesage;
using namespace bellebonnesage::painters;
using namespace prim::planar;
using namespace bellebonnesage::modern;

//An example of deriving a Portfolio and Canvas.
struct Score : public Portfolio
{
  struct Page : public Canvas
  {
    virtual void Paint(Painter& Painter, Portfolio& Portfolio)
    {
      //Create a bunch of random beamings. 
      Random r(11);
      for(count i = 1; i <= 16; i++)
      {
        for(count j = 1; j <= 10; j++)
        {
          //Create a beam with a random number of stems.
          Beam bg(r.NextIntegerInRange((prim::integer)2, (prim::integer)6));
          
          //Indicate to all the stems how many levels deep they are.
          for(count k = 0; k < bg.Stems.n(); k++)
            bg.Stems[k].ExtraLevels = r.NextIntegerInRange((prim::integer)0,
              (prim::integer)3);            
          
          //Set some rendering properties.
          bg.UnitsFlagWidth = 0.1f;
          bg.UnitsLevelSpacing = 0.1f;
          bg.UnitsLevelThickness = 0.07f;
          
          //Set the slant of the beams.
          bg.UnitsBaseline = Line((prim::number)i - 0.3f, (prim::number)j,
          (prim::number)i + 0.9f, (prim::number)j +
            r.NextNumberInRange(-0.2f, 0.2f));
          
          //Randomly set the stem directions.
          for(count s = 0; s < bg.Stems.n(); s++)
          {
            bg.Stems[s].UnitsNextStemDistance =
              0.9f / (prim::number)bg.Stems.n();
            bg.Stems[s].StemUp = r.NextIntegerInRange((prim::integer)10) < 5;
          }
          
          //Paint the beam.
          bg.Paint(&Painter);
        }
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

  //Set to letter landscape.
  MyScore.Canvases.z()->Dimensions = Paper::Landscape(Paper::Tabloid);

  //Set the PDF-specific properties.
  PDF::Properties PDFSpecificProperties;
  PDFSpecificProperties.Filename = "Beaming.pdf";
  
  //Write the score to PDF.
  MyScore.Create<PDF>(PDFSpecificProperties);

  //Note success to console window.
  c >> "Successfully wrote '" << PDFSpecificProperties.Filename << "'.";
  c++;
}

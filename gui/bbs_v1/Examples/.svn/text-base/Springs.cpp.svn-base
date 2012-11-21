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

//Font which will be loaded.
Font MyFont;

//Idea: a spring between two things should be a function of both things.

///Represents a Hookean spring.
struct Spring
{
  ///The spring constant.
  number k;
  
  ///The spring equilibrium length.
  number e;
  
  ///The displacement from the equilibrium.
  number x;
  
  ///Additional force bias.
  number c;
  
  ///Returns the total length of the spring.
  number l()
  {
    return e + x;
  }
  
  ///Returns the restoring force.
  number F()
  {
    return -k * x + c;
  }
  
  ///Default constructor initializes constants.
  Spring() : k(0), e(0), x(0), c(0) {}
};

struct Symbols
{
  enum SymbolType
  {
    Barline,
    QuarterNote,
    HalfNote,
    WholeNote,
    FontSymbols,
    TrebleClef = 65,
    BassClef = 66,
    CommonTime = 76,
    CutTime = 77,
    WholeRest = 79,
    HalfRest = 80,
    QuarterRest = 81,
    EighthRest = 82,
    SixteenthRest = 83
  };
  
  static Path Get(SymbolType x)
  {
    if(x > FontSymbols)
    {
      const Typeface& t = *MyFont.GetTypeface(Font::Special1);
      Path p;
      if(Glyph* g = t.LookupGlyph(x))
        p.Append(*g, Affine::Scale(4.0));
      return p;
    }
    
    Path p;
    
    if(x == Barline)
    {
      Shapes::AddLine(p, Vector(0.0, 2.0), Vector(0.0, -2.0), 0.1);
    }
    else if(x == QuarterNote)
    {
      Shapes::Music::AddQuarterNote(p, Vector());
    }
    else if(x == HalfNote)
    {
      Shapes::Music::AddHalfNote(p, Vector());
    }
    else if(x == WholeNote)
    {
      Shapes::Music::AddWholeNote(p, Vector());
    }
    
    return p;
  }
  
  static number GetK(SymbolType x)
  {
    switch(x)
    {
      case Barline: return 50.0;
      case QuarterNote: return 2.0;
      case HalfNote: return 4.0;
      case WholeNote: return 8.0;
      case TrebleClef: return 50.0;
      case BassClef: return 50.0;
      case CommonTime: return 50.0;
      case CutTime: return 50.0;
      case WholeRest: return 8.0;
      case HalfRest: return 4.0;
      case QuarterRest: return 2.0;
      case EighthRest: return 4.0;
      case SixteenthRest: return 8.0;
      default: return 1.0;
    }
    return 1.0;
  }
  
  static number GetE(SymbolType x)
  {
    switch(x)
    {
      case Barline: return 1.0;
      case QuarterNote: return 2.0;
      case HalfNote: return 4.0;
      case WholeNote: return 8.0;
      case TrebleClef: return 1.0;
      case BassClef: return 1.0;
      case CommonTime: return 1.0;
      case CutTime: return 1.0;
      case WholeRest: return 8.0;
      case HalfRest: return 4.0;
      case QuarterRest: return 2.0;
      case EighthRest: return 1.0;
      case SixteenthRest: return 1.0;
      default: return 1.0;
    }
    return 1.0;
  }
};

struct Object
{
  Symbols::SymbolType s;
  
  //Cached...
  Path p;
  Rectangle Bounds;
  
  //Spring...
  number k;
  number e;
  
  //Calculated...
  number Displacement;
  number Position;
  
  number l()
  {
    return p.Bounds().Left();
  }
  
  number r()
  {
    return p.Bounds().Right();
  }
  
  number w()
  {
    return p.Bounds().Width();
  }
  
  Object() {}
  
  Object(Symbols::SymbolType s) : s(s)
  {
    p = Symbols::Get(s);
    Bounds = p.Bounds();
    k = Symbols::GetK(s);
    e = Symbols::GetE(s);
    Displacement = 0;
  }
};

struct SeriesSystem : public prim::Array<Object>
{
  ///Returns the current width of the system.
  number SystemWidth()
  {
    return z().Position;
  }
  
  ///Calculates the size of the void.
  number Void()
  {
    number V = SystemWidth();
    for(count i = 0; i < n() - 1; i++)
      V -= ith(i).r();
    for(count i = 1; i < n(); i++)
      V -= -ith(i).l();
    return V;
  }
  
  ///Calculates the amount of stretch.
  number Stretch()
  {
    number S = Void();
    for(count i = 0; i < n() - 1; i++)
      S -= ith(i).e;
    return S;
  }
  
  ///Returns the number of active springs.
  count Springs()
  {
    return n() - 1;
  }
  
  number k(count i) {return ith(i).k;}
  
  ///Calculates the displacements due to the spring force.
  void CalculateDisplacements()
  {
    //Pre-calculate some of the knowns.
    count m = Springs();
    number S = Stretch();
    
    //Pre-calculate the denominator (common to all springs).
    number d = 0.0;
    for(count i = 0; i < m; i++)
    {
      number dProduct = 1.0;
      for(count j = 0; j < m; j++)
      {
        if(j == i) continue; //Skip multiplying by k(i).
        dProduct *= k(j);
      }
      d += dProduct;
    }
    
    prim::c >> "Denominator: " << d;
    //Bail out if the denominator is zero or near zero.
    if(Abs(d) < 1.0e-10)
      return;
    
    //Go through each spring and calculate its displacement.
    for(count a = 0; a < m; a++)
    {
      number x = S / d;
      for(count i = 0; i < m; i++)
      {
        if(i == a) continue; //Skip multiplying by k(a)
        x *= k(i);
      }
      
      prim::c >> "Displacement " << a << ": " << x;
      ith(a).Displacement = x;
    }
    
    //Reposition the islands.
    for(count i = 0; i < n() - 1; i++)
    {
      //Force the first island to have an origin of zero.
      if(!i) ith(i).Position = 0.0;
      
      number Distance = ith(i).e + ith(i).Displacement +
        -ith(i + 1).l() + ith(i).r();
      prim::c >> "Distance " << i << ": " << Distance;
      
      ith(i + 1).Position = ith(i).Position + Distance;
    }
  }
};

//An example of deriving a Portfolio and Canvas.
struct Score : public Portfolio
{
  struct Page : public Canvas
  {
    Affine Pos(number x, number y = 0.0)
    {
      number h = Dimensions.y;
      return Affine::Translate(Vector(1.0, h / 2.0 + y)) * Affine::Scale(0.1) *
        Affine::Translate(Vector(x, 0.0));
    }
    
    void Distribute(SeriesSystem& a, number TotalLength)
    {
      for(count i = 0; i < a.n(); i++)
      {
        number d = (number)i;
        number t = (number)a.n();
        a[i].Position = d / t * TotalLength;
      }
    }
    
    void DrawSpring(Painter* Painter, number l, number r, number y, number h,
      count Windings)
    {
      number yt = h / 2.0;
      number yb = -h / 2.0;
      number Thickness = 0.1;
      l += Thickness / 2.0;
      r -= Thickness / 2.0;      
      number w = r - l;
      Path p;
      Shapes::AddLine(p, Vector(0.0, yt), Vector(0.0, yb), Thickness);
      Windings *= 2;
      for(number i = 0; i < (number)Windings; i++)
      {
        number x1 = (i / Windings) * w;
        number x2 = ((i + 1.0) / Windings) * w;
        number y1 = yt, y2 = yb;
        if(((count)i) % 2 == 1)
          Swap(y1, y2);
        
        Shapes::AddLine(p, Vector(x1, y1), Vector(x2, y2), Thickness);
      }
      
      Shapes::AddLine(p, Vector(w, yt), Vector(w, yb), Thickness);
      
      Painter->SetFill(Colors::gray);
      //Painter->Draw(p, Pos(l, y));
    }
    
    void Draw(Painter* Painter, SeriesSystem& a, number y = 0.0)
    {
      for(count i = 0; i < a.n(); i++)
      {
        Painter->SetFill(Colors::black);
        Painter->Draw(a[i].p, Pos(a[i].Position, y));
        
        if(i == a.n() - 1)
          continue;
          
        number l = a[i].r() + a[i].Position;
        number r = a[i + 1].l() + a[i + 1].Position;
        number w = 6;//r - l - a[i].Displacement;
        DrawSpring(Painter, l, r, y, 1.0, (count)w + 1);
      }
    }
    
    virtual void Paint(Painter& Painter, Portfolio& Portfolio)
    {
      modern::House h;
      modern::Cache c;
      const Typeface& t = *MyFont.GetTypeface(Font::Special1);
      c.Create(h, t);
      
      SeriesSystem a;
      a.Add() = Object(Symbols::Barline);
      a.Add() = Object(Symbols::TrebleClef);
      a.Add() = Object(Symbols::CommonTime);
      a.Add() = Object(Symbols::WholeNote);
      a.Add() = Object(Symbols::Barline);
      a.Add() = Object(Symbols::HalfNote);
      a.Add() = Object(Symbols::QuarterNote);
      a.Add() = Object(Symbols::QuarterNote);
      a.Add() = Object(Symbols::Barline);
      a.Add() = Object(Symbols::HalfNote);
      a.Add() = Object(Symbols::QuarterNote);
      a.Add() = Object(Symbols::EighthRest);
      a.Add() = Object(Symbols::SixteenthRest);
      a.Add() = Object(Symbols::SixteenthRest);
      a.Add() = Object(Symbols::Barline);
      Distribute(a, 10.0);
      Draw(&Painter, a, 0.5);
      
      //Process...
      prim::c >> "System Width   : " << a.SystemWidth();
      prim::c >> "Size of Void   : " << a.Void();
      prim::c >> "Size of Stretch: " << a.Stretch();
      a.CalculateDisplacements();
      Draw(&Painter, a, -0.5);
    }
  };
};

String DetermineFontPath()
{
  String Dummy;
  if(File::Read("./Joie.bellefont", Dummy))
    return "./";
  else if(File::Read("../Resources/Joie.bellefont", Dummy))
    return "../Resources/";
  else if(File::Read("./Resources/Joie.bellefont", Dummy))
    return "./Resources/";
  else
    c >> "Path to bellefonts could not be determined.";
  return "";
}

void LoadFonts()
{
  //Find the font path.
  String Path = DetermineFontPath();
  if(!Path)
    return;
  
  //Queue up some font files to load.
  String Joie = Path; Joie << "Joie.bellefont";
  
  //Load some typefaces into the font.
  Array<byte> a;
  if(File::Read(Joie, a))
    MyFont.Add(Font::Special1)->ImportFromArray(&a.a());
}

int main()
{
  //Load the fonts.
  LoadFonts();
  
  //Create a score.
  Score MyScore;

  //Add a page to the score.
  MyScore.Canvases.Add() = new Score::Page;

  //Set to letter landscape.
  MyScore.Canvases.z()->Dimensions = Inches(11.0, 3.0);

  //Set the PDF-specific properties.
  PDF::Properties PDFSpecificProperties;
  PDFSpecificProperties.Filename = "Springs.pdf";
  
  //Write the score to PDF.
  MyScore.Create<PDF>(PDFSpecificProperties);

  //Note success to console window.
  c >> "Successfully wrote '" << PDFSpecificProperties.Filename << "'.";

  //Add a blank line to the output.
  c++;
  
  return 0;
}

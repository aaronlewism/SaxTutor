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

#ifndef BELLEBONNESAGE_MODERN_STAMP_H
#define BELLEBONNESAGE_MODERN_STAMP_H

namespace bellebonnesage { namespace modern
{
#if 0
/*
  ///Base class for stamp graphic. 
  struct StampGraphicBase
  {
    ///Returns the bounds of the stamp graphic.
    virtual prim::planar::Rectangle Bounds(Affine Other = Affine::Unit())
    {
      return prim::planar::Rectangle();
    }
    
    ///Returns whether the stamp graphic is empty.
    virtual bool IsEmpty()
    {
      return true;
    }
    
    ///Returns whether the stamp graphic is not empty.
    virtual bool IsNotEmpty()
    {
      return !IsEmpty();
    }
  };
*/
#endif
  
  ///Single graphical object consisting of a path and an affine transform.
  struct StampGraphic
  {
    ///Path of the graphic.
    bellebonnesage::Path p;
    
    ///Fill color of the stamp.
    bellebonnesage::Color c;
    
    ///Path to cached graphic.
    const bellebonnesage::Path* p2;
    
    ///Affine transformation of the graphic.
    bellebonnesage::Affine a;
    
    ///Text of the graphic (provisional object, needs rework).
    bellebonnesage::Text* t;
    
    ///Strokes the path with the width instead of filling it.
    prim::number StrokeWidth;
    
    ///Graph node related to the graphic.
    graph::MusicNode* n;
    
    ///Placement of the stamp graphic on the last paint.
    prim::planar::Rectangle PlacementOnLastPaint;
    
    ///Index of page.
    prim::count PlacementPageIndex;
    
    /**Click index of the stamp graphic. This keeps track of cycle-through.
    Essentially when an object is clicked, its click index becomes the next
    cycle-through value and thus becomes least-preferred for the next click.
    This value is maintained by the ClickThroughManager.*/
    prim::count ClickIndex;
    
    /**Transfers the selection of another stamp graphic if node is the same.
    This is used to transfer a selection of the part to the whole. For example,
    clicking on a flag would select the rest of the note if the underlying node
    was the same.*/
    void TransferSelected(StampGraphic* SelectedStamp)
    {
      if(SelectedStamp && SelectedStamp->n && SelectedStamp->n == n)
        ClickIndex = SelectedStamp->ClickIndex;
    }
    
    ///Gets the bounds of the stamp graphic.
    prim::planar::Rectangle Bounds(Affine Other = Affine::Unit()) const
    {
      return p2 ? p2->Bounds(Other * a) : p.Bounds(Other * a);
    }
    
    ///Returns whether the stamp graphic does not contain visible graphics.
    bool IsEmpty() const
    {
      return Bounds().IsEmpty();
    }
    
    ///Returns whether the stamp graphic contains visible graphics.
    bool IsNotEmpty() const
    {
      return !IsEmpty();
    }
    
    ///Applies a selection coloring if the selection matches the click index.
    void ApplySelectionColoring(prim::count SelectionIndex,
      bellebonnesage::Color SelectionColor = bellebonnesage::Colors::royalblue,
      bellebonnesage::Color DefaultColor = bellebonnesage::Colors::black)
    {
      c = (SelectionIndex == ClickIndex && SelectionIndex) ? SelectionColor :
        DefaultColor;
    }
    
    ///Paints the stamp graphic.
    void Paint(bellebonnesage::Painter* Painter)
    {
      //Set the stroke and fill style.
      if(StrokeWidth)
        Painter->SetStroke(c, StrokeWidth);
      else
        Painter->SetFill(c);

      //Paint the stamp graphic.
      if(p2)
        Painter->Draw(*p2, a);
      else if(t)
      {
        Painter->Transform(Affine::Scale(20.0));
        Painter->Draw(*t, a * Affine::Scale(20.0));
      }
      else
        Painter->Draw(p, a);
      
      //Cache the bounds of the stamp graphic on the page.
      PlacementOnLastPaint = Bounds(Painter->CurrentSpace());
      PlacementPageIndex = Painter->GetPageNumber();
    }
    
    ///Constructor to zero the text pointer.
    StampGraphic() : c(bellebonnesage::Colors::black), p2(0), t(0),
     StrokeWidth(0.0), n(0), PlacementPageIndex(-1), ClickIndex(0) {}
    
    ///Destructor to delete the text pointer.
    ~StampGraphic() {delete t;}
  };
  
  ///Graphical object with multiple items positioned relative to each other.
  struct Stamp : public graph::MusicNode::TypesettingInfo
  {
    ///Array of individual graphical objects with their own affine transforms.
    prim::Array<StampGraphic*> Graphics;
    
    ///Adds a stamp graphic.
    StampGraphic& Add()
    {
      return *(Graphics.Add() = new StampGraphic);
    }

    ///Gets the last stamp graphic.
    StampGraphic& z()
    {
      return *Graphics.z();
    }
    
    ///Gets the last stamp graphic.
    const StampGraphic& z() const
    {
      return *Graphics.z();
    }
    
    ///The transformation to be applied to the stamp to position and scale it.
    bellebonnesage::Affine Context;
    
    ///Indicates whether the stamp needs to be retypeset before displaying it.
    bool NeedsTypesetting;
    
    ///Resets the stamp to have no graphical objects.
    void Clear()
    {
      NeedsTypesetting = true;
      Graphics.ClearAndDeleteAll();
      Context = Affine::Unit();
    }
    
    ///Paints the stamp.
    void Paint(bellebonnesage::Painter* Painter)
    {
      //Transform to the context of the stamp.
      Painter->Transform(Context);
      
      //Paint each object in the stamp.
      for(prim::count i = 0; i < Graphics.n(); i++)
        Graphics[i]->Paint(Painter);
      
      //Reset the color and undo the transformation.
      Painter->Revert();
    }
  
    ///Gets the bounds of the stamp.
    prim::planar::Rectangle Bounds(Affine a = Affine::Unit()) const
    {
      prim::planar::Rectangle r;
      for(prim::count i = 0; i < Graphics.n(); i++)
        r = r + Graphics[i]->Bounds(a);
      return r;
    }
    
    ///Constructor creates a blank stamp.
    Stamp() {Clear();}
  };
}}
#endif

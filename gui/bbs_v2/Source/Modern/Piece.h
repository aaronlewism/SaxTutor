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

#ifndef BELLEBONNESAGE_MODERN_PIECE_H
#define BELLEBONNESAGE_MODERN_PIECE_H

#include "Directory.h"
#include "Island.h"
#include "Selection.h"

namespace bellebonnesage { namespace modern
{
  struct InstantState
  {
    ///Left-most coordinate of the instant
    prim::number Left;
    
    ///Right-most coordinate of the instant
    prim::number Right;
    
    ///Width of the instant
    prim::number Width() const {return Right - Left;}
    
    ///Extra buffer space for the instant
    prim::number ExtraBuffer;
    
    ///Sets whether the instant is breakable.
    bool IsBreakable;
    
    ///Creates a union with this instant state.
    const InstantState& Union(const InstantState& i)
    {
      //Be careful to only union with non-zero width instant states.
      if(i.Width())
      {
        if(Width())
        {
          Left = prim::Min(Left, i.Left);
          Right = prim::Max(Right, i.Right);
        }
        else
        {
          Left = i.Left;
          Right = i.Right;
        }
      }
      return *this;
    }
    
    ///Default constructor
    InstantState() : Left(0), Right(0), ExtraBuffer(0), IsBreakable(false) {}
    
    ///Constructor from rectangle.
    InstantState(prim::planar::Rectangle r)
    {
      if(r.IsEmpty())
        Left = Right = 0;
      else
      {
        Left = r.Left();
        Right = r.Right();
      }
    }
  };
  
  ///Instant consisting of all islands in series by instant-wise links.
  struct Instant : public prim::Array<graph::MusicNode*>
  {
    prim::count GetPartIndex(prim::count i)
    {
      //if(
      return 0;
    }
    
    Instant() {}
    
    Instant(prim::Node* NodeToSearch)
    {
      NodeToSearch->FindAll(*this, graph::ID(mica::InstantWiseLink),
        prim::Link::Directions::Forwards);
    }
  };
  
  ///Stores a piece of music existing on the same timeline.
  struct Piece : public ClickThroughManager
  {
    ///Stores the music graph.
    graph::MusicGraph* Music;
    
    //Objects for typesetting.
    const House* h;
    const Cache* c;
    const Typeface* t;
    const Font* f;
    
    //Calculated...
    graph::Geometry GraphGeometry; 
        
    ///Default constructor.
    Piece() : Music(0), h(0), c(0), t(0), f(0) {}
    
    ///Constructor to initialize typesetting objects.
    Piece(graph::MusicGraph* Music, const House& h, const Cache& c,
      const Typeface& t, const Font& f) : Music(Music), h(&h), c(&c), t(&t),
      f(&f) {}
    
    ~Piece()
    {
      delete Music;
    }
    
    ///Returns whether the class is initialized.
    bool Initialized() {return Music && h && c && t && f;}
    
    ///Initialize the typesetting objects.
    void Initialize(graph::MusicGraph* Music, const House& h, const Cache& c,
      const Typeface& t, const Font& f)
    {
      Piece::Music = Music;
      Piece::h = &h;
      Piece::c = &c;
      Piece::t = &t;
      Piece::f = &f;
    }
    
    ///Typesets only the islands needing to be typeset.
    void TypesetRemaining()
    {
      if(!Initialized())
      {
        prim::c >> "Typesetting objects for piece are uninitialized.";
        return;
      }
            
      graph::MusicNode* m =
        dynamic_cast<graph::MusicNode*>(Music->GetTop());
      while(m)
      {
        graph::MusicNode* n = m;
        State EngraverState;
        Directory d(EngraverState, *Music, *h, *c, *t, *f);
        IslandEngraver Engraver(d);
        while(n)
        {
          if(prim::Pointer<Stamp> s = n->Typesetting)
          {
            if(s->NeedsTypesetting)
            {
              Engraver.Engrave(n, *s);
              d.s.AdvanceAccidentalState();
              s->NeedsTypesetting = false;
            }
          }
          else
            prim::c >> "Warning: Stamp not created for MusicNode.";
          
          n->Find<graph::MusicNode>(n, graph::ID(mica::PartWiseLink));
        }
        m->Find<graph::MusicNode>(m, graph::ID(mica::InstantWiseLink));
      }
    }
    
    ///Clears typesetting data for all islands.
    void ClearTypesetting()
    { 
      graph::MusicNode* m =
        dynamic_cast<graph::MusicNode*>(Music->GetTop());
      while(m)
      {
        graph::MusicNode* n = m;
        while(n)
        {
          //Create a new stamp if it is not there, or clear an existing one.
          if(!n->Typesetting)
            n->Typesetting = new Stamp(n);
          else if(prim::Pointer<Stamp> s = n->Typesetting)
            s->Clear();
          else
            prim::c >> "Error: Non-Stamp class detected while clearing.";
          
          n->Find<graph::MusicNode>(n, graph::ID(mica::PartWiseLink));
        }
        m->Find<graph::MusicNode>(m, graph::ID(mica::InstantWiseLink));
      }
    }
    
    ///Clears typesetting data for all islands.
    void InitializeTypesetting()
    { 
      graph::MusicNode* m =
        dynamic_cast<graph::MusicNode*>(Music->GetTop());
      while(m)
      {
        graph::MusicNode* n = m;
        while(n)
        {
          //Create a new stamp if it is not there, or clear an existing one.
          if(!n->Typesetting)
            n->Typesetting = new Stamp(n);
          else if(!(prim::Pointer<Stamp>(n->Typesetting)))
            prim::c >> "Error: Non-Stamp class detected while clearing.";
          
          n->Find<graph::MusicNode>(n, graph::ID(mica::PartWiseLink));
        }
        m->Find<graph::MusicNode>(m, graph::ID(mica::InstantWiseLink));
      }
    }
    
    ///Determines the minimum space for typesetting each island together.
    void CollectInstants(prim::Array<InstantState>& InstantStates)
    {      
      InstantStates.Clear();
      Instants.Clear();
      graph::MusicNode* m =
        dynamic_cast<graph::MusicNode*>(Music->GetTop());
      while(m)
      {
        Instants.Add() = Instant(m);
        
        { //Old style instant collection
          graph::MusicNode* n = m;
          InstantState i;
          
          //Determine the buffer for each type of object.
          graph::Token* t = 0;
          m->Find<graph::Token>(t, graph::ID(mica::TokenLink));
          if(dynamic_cast<graph::ChordToken*>(t))
            i.ExtraBuffer = 0.1;
          else if(dynamic_cast<graph::MeterToken*>(t))
          {
            i.ExtraBuffer = 0.1;
          }
          else if(dynamic_cast<graph::BarlineToken*>(t))
          {
            i.ExtraBuffer = 0.1;
            i.IsBreakable = true;
          }
          else
          {
            i.ExtraBuffer = 0.0;
          }
          
          while(n)
          {
            if(!n->Typesetting)
              prim::c >> "Warning: some nodes have not been typeset";
            else if(prim::Pointer<Stamp> s = n->Typesetting)
              i = i.Union(InstantState(s->Bounds(Affine::Scale(0.05))));
            n->Find<graph::MusicNode>(n,
              graph::ID(mica::InstantWiseLink));
          }
          InstantStates.Add() = i;
        }
        
        m->Find<graph::MusicNode>(m,
          graph::ID(mica::PartWiseLink));
      }
    }
    
    ///Retypesets all the islands.
    void ClearAndTypeset()
    {
      //Clear existing typesetting and retypeset everything.
      ClearTypesetting();
      TypesetRemaining();
      CollectInstants(InstantStates);
    }
    
    ///Typesets the islands.
    void Typeset()
    {
      InitializeTypesetting();
      TypesetRemaining();
      CollectInstants(InstantStates);
    }
    
    ///Parses the score geometry.
    void Parse()
    {
      //Parse the geometry.
      prim::Timer t; t.Start();
      GraphGeometry.Parse(*Music);
      prim::c >> "Parse Time: " << (t.Stop() * 1000.0) << " ms";
      
      //Display the extents.
      prim::c >> "" >> "Part Extents:";
      for(prim::count i = 0; i < GraphGeometry.GetNumberOfParts(); i++)
        prim::c >> "Part " << i << ": " << GraphGeometry.GetPartRange(i);
      
      prim::c >> "" >> "Instant Part Membership:";
      for(prim::count i = 0; i < GraphGeometry.GetNumberOfInstants(); i++)
      {
        prim::List<prim::count> l;
        GraphGeometry.GetPartListForInstant(i, l);
        prim::c >> "Instant " << i << ": " << l <<
          (GraphGeometry.IsInstantComplete(i) ? " (complete) " : " ");
      }
      
      //Transfer to a spring matrix.
      t.Start();
      prim::count Rows = GraphGeometry.GetNumberOfParts();
      prim::count Columns = GraphGeometry.GetNumberOfInstants();
      SpringMatrix S;
      S.mn(Rows, Columns);
      for(prim::count j = 0; j < Columns; j++)
      {
        prim::List<prim::count> l;
        GraphGeometry.GetPartListForInstant(j, l);
        for(prim::count i = 0; i < l.n(); i++)
        {
          SpringNode& sn = S(l[i], j);
          sn.Activate();
          graph::Island* IslandOfNode = GraphGeometry.LookupIsland(l[i], j);
          prim::Pointer<Stamp> s = IslandOfNode->Typesetting;
          prim::planar::Rectangle IslandBounds = s->Bounds();
          if(IslandBounds.IsEmpty())
          {
            prim::c >> "Deactivating island " << i << ", " << j;
            sn.Deactivate();
            sn.LeftExtent = 0.0;            
            sn.RightExtent = 0.0;
            continue;
          }
          sn.LeftExtent = IslandBounds.Left();
          sn.RightExtent = IslandBounds.Right();
        }
      }
#if 0
      for(prim::count i = 0; i < Rows; i++)
      {
        graph::Island* Isle = GraphGeometry.LookupIsland(i, 1);
        prim::Array<graph::MusicNode*> mn;
        Isle->FindAll(mn, graph::ID(mica::TokenLink));
        prim::c >> "Row " << i << ": " << Isle->GetLinkCount();
        
        for(prim::count j = 0; j < Isle->GetLinkCount(); j++)
        {
          prim::c >> mica::M(graph::ID(Isle->GetLink(i).Label)).Name();
        }
      }
#endif
      t.Stop();      
      prim::c >> S >> "";
      //Solve the spring matrix.
      t.Start();
      SpringMatrix O;
      while(SpringSolver::Simplify(S, O))
      {
        S = O;
      }
      prim::c >> "Time: " << t.Stop();
    }
    
    /**List of copied stamps which are prepended to non-initial systems. Each
    System has a RepeatedElements. Since the stamps are deep copies.*/
    struct RepeatedElements
    {
      class Element
      {
        const graph::Token* t;
        prim::Pointer<modern::Stamp> s;

        public:
        
        Element() : t(0) {}
        
        Element(const graph::Token* t) : t(t)
        {
          if(t)
          {
            graph::MusicNode* i = 0;
            t->Find<graph::MusicNode>(i, graph::ID(mica::TokenLink),
              prim::Link::Directions::Backwards);
            prim::Pointer<Stamp> Other = i->Typesetting;
            s = new Stamp(*Other);
          }
        }
        
        Element& operator = (const Element& Other)
        {
          t = Other.t;
          s = Other.s;
          return *this;
        }
        
        Element(const Element& Other)
        {
          *this = Other;
        }
        
        const graph::Token* GetToken() const
        {
          return t;
        }
        
        Stamp* GetStamp() const
        {
          return s.Raw();
        }
        
        static bool IsRepeatable(const graph::Token* Other)
        {
          if(dynamic_cast<const graph::KeySignatureToken*>(Other)) return true;
          else if(dynamic_cast<const graph::ClefToken*>(Other)) return true;
          else if(dynamic_cast<const graph::BarlineToken*>(Other)) return true;   
          return false;
        }
        
        bool IsSameTypeAs(const graph::Token* Other) const
        {
          if(dynamic_cast<const graph::KeySignatureToken*>(t) &&
            dynamic_cast<const graph::KeySignatureToken*>(Other)) return true;
          else if(dynamic_cast<const graph::ClefToken*>(t) &&
            dynamic_cast<const graph::ClefToken*>(Other)) return true;
          else if(dynamic_cast<const graph::BarlineToken*>(t) &&
            dynamic_cast<const graph::BarlineToken*>(Other)) return true;
          return false;
        }
      };
      
      private:
      
      prim::List<Element> Elements;
      
      public:
      
      prim::count n() const {return Elements.n();}
      
      Element& operator [] (prim::count i)
      {
        return Elements[i];
      }

      const Element& operator [] (prim::count i) const
      {
        return Elements[i];
      }
      
      RepeatedElements() {}
      
      RepeatedElements(const RepeatedElements& Other) {*this = Other;}

      RepeatedElements& operator = (const RepeatedElements& Other)
      {
        //Deep-copy the stamps.
        Elements.RemoveAll();
        for(prim::count i = 0; i < Other.Elements.n(); i++)
          Elements.Add() = Element(Other.Elements[i].GetToken());
        return *this;
      }
      
      void ReplaceOrAppend(graph::Token* New)
      {
        //Check to make sure the item makes sense to repeat.
        if(!Element::IsRepeatable(New))
          return;
        
        //Try to replace the item if it already exists.
        for(prim::count i = 0; i < Elements.n(); i++)
        {
          if(Elements[i].IsSameTypeAs(New))
          {
            //            prim::c >> "Replacing..";
            Elements[i] = Element(New);
            return;
          }
        }
        
        //Append the element since its type does not exist yet.
        Elements.Add() = Element(New);
        //        prim::c >> "Adding..";
      }
    };
    
    struct System
    {
      RepeatedElements InitialElements;
      prim::Array<prim::Pointer<Stamp> > StampList;
    
      prim::count Start;
      prim::count End;
      prim::number ExtraSpace;
      prim::number Width;
      prim::number SystemWidth;
      
      System() : Start(0), End(0), ExtraSpace(0),
        Width(0), SystemWidth(0) {}
    };
    
    void Prepare(prim::Array<Piece::System>& Systems,
      prim::number FirstSystemWidth, prim::number MainSystemWidth)
    {
      //Typeset any graphics that need it.
      Typeset();
      
      //Clear any old systems.
      Systems.Clear();
      
      //--
      prim::number LineSpaceScale = 0.05;
      prim::number StartingY = 1.1;
      prim::number SystemSpacing = 0.5;
      prim::number NonInitialOffset = 0.02;
      //--
      
      //Crop each system of music.
      for(prim::count i = 0; i < InstantStates.n(); i++)
      {
        //Initialize state variables.
        prim::number Left = (i == 0 ? 0.0 : NonInitialOffset);
        prim::number MaxSize = (Systems.n() ?
          MainSystemWidth : FirstSystemWidth);
        prim::number BarlineExtent = 0.0;
        prim::count Barline_i = i;
        
        //Add a system and set its start index.
        Systems.Add();
        Systems.z().Start = i;
        Systems.z().SystemWidth = MaxSize;
        
        //Copy the repeatable elements from the previous system.
        if(Systems.n() > 1)
          Systems.z().InitialElements = Systems.z(1).InitialElements;
        
        //prim::c >> Systems.z().InitialElements.n();
        /*Advance the width by the initial elements and add them to the stamp
        list.*/
        for(prim::count j = 0; j < Systems.z().InitialElements.n(); j++)
        {
          Systems.z().StampList.Add() =
            Systems.z().InitialElements[j].GetStamp();
          
          if(!j)
            Left += Systems.z().StampList.z()->Bounds().Right();
          else
            Left += Systems.z().StampList.z()->Bounds().Width();
        }
        
        //Loop while there are islands to traverse and instants to use.
        while(i < InstantStates.n())
        {
          //Advance the width.
          Left += InstantStates[i].Width() + InstantStates[i].ExtraBuffer;
          
          //If past the edge of the page then discard current measure.
          if(Left > MaxSize)
            break;
          
          /*If a barline is reached or the last item in the score, then bookmark
          it as the last thing.*/
          if(InstantStates[i].IsBreakable || i == InstantStates.n() - 1)
          {
            //Set the new extents.
            Barline_i = i;
            BarlineExtent = Left;
          }
          
          //Increment the instant index.
          i++;
        }
        
        //Subtract out the buffer following the last item of the system.
        BarlineExtent -= InstantStates[Barline_i].ExtraBuffer;
        Systems.z().Width = BarlineExtent;
        
        //Set the end index of the system.
        Systems.z().End = i = Barline_i;
        
        //Hopefully, there were at least two things in the system.
        if(Systems.z().End - Systems.z().Start < 1)
          continue;
        
        //Calculate the extra space needed.
        Systems.z().ExtraSpace = (MaxSize - BarlineExtent) /
          (prim::number)(Systems.z().End - Systems.z().Start);
      }
      
      //Make the last system take up only the necessary width.
      Systems.z().ExtraSpace = 0.0;
      Systems.z().SystemWidth = Systems.z().Width;
      
      //For each system, prepare the spacing.
      for(prim::count SystemIndex = 0; SystemIndex < Systems.n(); SystemIndex++)
      {
        graph::MusicNode* m = dynamic_cast<graph::MusicNode*>(Music->GetTop());
        prim::number j = StartingY;
        while(m)
        {
          //Draw the island
          {
            //Initialize state variables.
            prim::count i = 0;
            prim::number Left = (SystemIndex == 0 ? 0.0 : NonInitialOffset);
            prim::number Center = 0.0;

            //Set part traversal node and traverse part-wise.
            graph::MusicNode* n = m;
            while(n)
            {
              //Skip over any instants not part of this system.
              if(i >= Systems[SystemIndex].Start &&
                i <= Systems[SystemIndex].End)
              {
                //Get the stamp for this island.
                if(prim::Pointer<Stamp> s = n->Typesetting)
                {
                  //Get the center of the stamp.
                  Center = Left - InstantStates[i].Left;
                  
                  //Create the affine transformation for the stamp.
                  s->Context =
                    Affine::Translate(prim::planar::Vector(Center, j))
                    * Affine::Scale(LineSpaceScale);
                  
                  //Add the stamp to the stamp list for this system.
                  Systems[SystemIndex].StampList.Add() = s;
                  
                  //Add the token to the initial elements.
                  graph::Token* t = 0;
                  n->Find<graph::Token>(t, graph::ID(mica::TokenLink));
                  Systems[SystemIndex].InitialElements.ReplaceOrAppend(t);
                }
                
                //Calculate the new left.
                Left += InstantStates[i].Width() + InstantStates[i].ExtraBuffer
                  + Systems[SystemIndex].ExtraSpace;
              }                
              
              //Go to the next island and increment instant index.
              n->Find<graph::MusicNode>(n, graph::ID(mica::PartWiseLink));
              i++;
              
              //No need to continue if beyond the system.
              if(i > Systems[SystemIndex].End)
                break;
            }
          }
  
          //Go to next part.
          m->Find<graph::MusicNode>(m, graph::ID(mica::InstantWiseLink));
          j -= SystemSpacing;
        }
      }
    }

    ///Draws the music.
    void Paint(Painter* Painter, Piece::System& SystemObject)
    {
      //--
      prim::number LineSpaceScale = 0.05;
      prim::number StartingY = 1.1;
      prim::number SystemSpacing = 0.5;
      prim::number LineThickness = LineSpaceScale * 0.14;
      //--
      prim::number SystemWidth = SystemObject.SystemWidth;
      
      prim::number j = StartingY;
      graph::MusicNode* m = dynamic_cast<graph::MusicNode*>(Music->GetTop());
      
      while(m)
      {
        //Draw staff lines
        {
          Painter->SetFill(Colors::black);
          graph::MusicNode* next_m;
          m->Find<graph::MusicNode>(next_m, graph::ID(mica::InstantWiseLink));
          if(next_m)
          {
            Path p;
            Shapes::AddLine(p, prim::planar::Vector(0.0, j),
              prim::planar::Vector(SystemWidth - LineThickness / 2.0, j),
              LineThickness);
            for(prim::number k = -2.0; k <= 2.0; k++)
              Painter->Draw(p, Affine::Translate(
                prim::planar::Vector(0.0, k * LineSpaceScale)));
          }
        }
      
        //Add stamp graphics to click manager.
        {
          prim::count i = 0;
          graph::MusicNode* n = m;
          while(n)
          {
            if(i >= SystemObject.Start && i <= SystemObject.End)
            {
              if(prim::Pointer<Stamp> s = n->Typesetting)
              {
                //Apply selection coloring.
                for(prim::count k = 0; k < s->Graphics.n(); k++)
                  s->Graphics[k]->ApplySelectionColoring(GetSelectionIndex());
                
                //Paint
                s->Paint(Painter);
                
                ///Add each constituent stamp graphic to the click through.
                for(prim::count k = 0; k < s->Graphics.n(); k++)
                  AddStampGraphicForConsideration(s->Graphics[k]);
              }
            }
            
            n->Find<graph::MusicNode>(n, graph::ID(mica::PartWiseLink));
            i++;
            if(i > SystemObject.End)
              break;
          }
        }
        
        //Go to next part
        m->Find<graph::MusicNode>(m, graph::ID(mica::InstantWiseLink));
        j -= SystemSpacing;
      }
      
#if 0 //Debug painting of stamp graphic bounding boxes.
      PaintBoundingBoxes(Painter);
#endif
    }
    
    private:

    ///Stores the instant states.
    prim::Array<InstantState> InstantStates;
    
    prim::Array<Instant> Instants;
  };
}}
#endif

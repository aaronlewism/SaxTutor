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

#ifndef BELLEBONNESAGE_MODERN_PIECE2_H
#define BELLEBONNESAGE_MODERN_PIECE2_H

#include "Directory.h"
#include "Island.h"
#include "Selection.h"

namespace bellebonnesage { namespace modern
{
  ///Stores a piece of music existing on the same timeline.
  struct Piece2 : public ClickThroughManager
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
    Piece2() : Music(0), h(0), c(0), t(0), f(0) {}
    
    ///Constructor to initialize typesetting objects.
    Piece2(graph::MusicGraph* Music, const House& h, const Cache& c,
      const Typeface& t, const Font& f) : Music(Music), h(&h), c(&c), t(&t),
      f(&f) {}
    
    ~Piece2()
    {
      delete Music;
    }
    
    ///Returns whether the class is initialized.
    bool Initialized() {return Music && h && c && t && f;}
    
    ///Initialize the typesetting objects.
    void Initialize(graph::MusicGraph* Music, const House& h, const Cache& c,
      const Typeface& t, const Font& f)
    {
      Piece2::Music = Music;
      Piece2::h = &h;
      Piece2::c = &c;
      Piece2::t = &t;
      Piece2::f = &f;
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
            s->Clear(n);
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
          else if(!prim::Pointer<Stamp>(n->Typesetting))
            prim::c >> "Error: Non-Stamp class detected while clearing.";
          
          n->Find<graph::MusicNode>(n, graph::ID(mica::PartWiseLink));
        }
        m->Find<graph::MusicNode>(m, graph::ID(mica::InstantWiseLink));
      }
    }
    
    ///Typesets the islands.
    void Typeset(bool ClearAll = false)
    {
      //Clear or initialize the typesetting.
      if(ClearAll)
        ClearTypesetting();
      else
        InitializeTypesetting();
      
      //Typeset the remaining stamps.
      TypesetRemaining();
      
      //Parse the music graph geometry.
      GraphGeometry.Parse(*Music);
      
      //Set instant properties.
      graph::Instant::SetDefaultProperties(*Music);
    }

    ///Retypesets all the islands.
    void ClearAndTypeset() {Typeset(true);}
    
    ///Displays the information gathered from the graph parse.
    void DisplayGeometryInfo()
    {
      //Display the extents.
      prim::c >> "" >> "Part Count: " << GraphGeometry.GetNumberOfParts();
      prim::c >> "" >> "Part Extents:";
      for(prim::count i = 0; i < GraphGeometry.GetNumberOfParts(); i++)
        prim::c >> "Part " << i << ": " << GraphGeometry.GetPartRange(i);
      
      prim::c >> "" >> "Instant Count: " << GraphGeometry.GetNumberOfInstants();
      prim::c >> "" >> "Instant Part Membership:";
      for(prim::count i = 0; i < GraphGeometry.GetNumberOfInstants(); i++)
      {
        prim::List<prim::count> l;
        GraphGeometry.GetPartListForInstant(i, l);
        prim::c >> "Instant " << i << ": " << l <<
          (GraphGeometry.IsInstantComplete(i) ? " (complete) " : " ");
      }
    }
    
    ///Displays the instant properties.
    void DisplayInstantProperties()
    {
      graph::MusicNode* n = dynamic_cast<graph::MusicNode*>(Music->GetTop());
      prim::count i = 0;
      while(n)
      {
        graph::Token* m = 0;
        n->Find<graph::Token>(m, graph::ID(mica::TokenLink));
        prim::c >> i << ": ";
        if(m)
          prim::c << m->ToString();
        graph::Instant::PrintProperties(n);
        prim::c++;
        i++;
        n->Find<graph::MusicNode>(n, graph::ID(mica::PartWiseLink));
      }
    }
    
    //--------------------------------------------------------------------------

    /**Calculates a new left-justified leading edge from an edge and an instant.
    The new leading edge is returned as an array, the new instant origin is
    updated, and*/
    static prim::number CalculateNextLeadingEdge(const StampInstant& Instant,
      prim::Array<prim::number>& LeadingEdge, prim::number& InstantOrigin)
    {
      ///Array to store new leading edge.
      prim::Array<prim::number> NewLeadingEdge;
      
      //Make sure the leading edge is correctly sized.
      if(LeadingEdge.n() != Instant.n())
      {
        prim::c >> "Error: Leading edge length (" << LeadingEdge.n() << ") "
          << "does not match number of parts (" << Instant.n() << ").";
        return 0.0;
      }
      
      //Calculate the new origin.
      for(prim::count i = 0; i < LeadingEdge.n(); i++)
      {
        //If the stamp does not exist, then it is not considered.
        if(!Instant[i] || Instant[i]->Bounds().IsEmpty())
          continue;
        
        /*Note that in the future it is possible that collision detection could
        be used here from Source/Optics.h instead of using coarse bounding
        boxes.*/
        InstantOrigin = prim::Max(InstantOrigin, LeadingEdge[i] -
          Instant[i]->Bounds().Left());
      }
      
      //Calculate the new leading edge.
      NewLeadingEdge.n(LeadingEdge.n());
      NewLeadingEdge.Zero();
      prim::number FurthestRight = 0.0;
      for(prim::count i = 0; i < LeadingEdge.n(); i++)
      {
        //If the stamp exists on this part then increase the leading edge.
        if(Instant[i] && !Instant[i]->Bounds().IsEmpty())
          NewLeadingEdge[i] = InstantOrigin + Instant[i]->Bounds().Right();
        
        //Keep track of the furthest-right point.
        FurthestRight = prim::Max(FurthestRight, NewLeadingEdge[i]);
      }
      
      //Copy the new leading edge back to the caller array.
      LeadingEdge = NewLeadingEdge;

      //Return the furthest right point on the leading edge.
      return FurthestRight;
    }
    
    ///Describes a list of stamp instants and their positions.
    struct System
    {
      ///Stamp instant list describing the graphics that make up the instant.
      prim::List<StampInstant> Instants;

      ///List of instant positions.
      prim::List<prim::number> InstantPositions;
      
      ///The current leading edge.
      prim::Array<prim::number> LeadingEdge;
      
      ///Staff heights.
      prim::Array<prim::number> StaffHeights;
      
      ///Staff uses staff lines.
      prim::Array<bool> HasStaffLines;
      
      ///Total height of the box that would bound the system.
      prim::number SystemHeight;
      
      ///Remembers the minimum system width.
      prim::number MinimumSystemWidth;
      
      ///The final bounds of the system.
      prim::planar::Rectangle Bounds;
      
      ///Gets the last known instant position.
      prim::number LastInstantPosition() const
      {
        if(InstantPositions.n())
          return InstantPositions.z();
        else
          return 0.0;
      }
      
      /**Distribute the staves. If the system width is zero, then it is left at
      minimum spacing.*/
      void CalculateSpacing(prim::number SpaceBetweenSystems,
        prim::number SystemWidth = 0.0)
      {
        //If there are no instants then return.
        if(!Instants.n())
        {
          prim::c >> "Error: No instants in this system.";
          return;
        }
        
        /*Get the number of parts in this system. Note this does not yet take
        into account unused staves like it should.*/
        prim::count PartCount = Instants.a().n();
        StaffHeights.n(PartCount);
        
        //Equally space the systems.
        for(prim::count i = 0; i < PartCount; i++)
          StaffHeights[i] = ((prim::number)(PartCount - 1 - i)) *
            SpaceBetweenSystems;
        
        //Get bounds of the system (without adjusted height).
        prim::planar::Rectangle Bound;
        for(prim::count i = 0; i < Instants.n(); i++)
          for(prim::count j = 0; j < Instants[i].n(); j++)
            if(prim::Pointer<Stamp> s = Instants[i][j])
              Bound += s->Bounds(Affine::Translate(prim::planar::Vector(
                InstantPositions[i], StaffHeights[j])));
        
        //Check the bounds to make sure they are sensible.
        if(Bound.IsEmpty())
        {
          prim::c >> "Error: System bound is empty.";
          return;
        }
        
        //Save the system width and height.
        MinimumSystemWidth = Bound.Right();
        SystemHeight = Bound.Height();
        
        //Space the elements.
        if(SystemWidth > 0.0)
          EqualSpace(SystemWidth);
        
        /*Adjust the staff heights to bring the bottom-most element flush with
        the x-axis.*/
        for(prim::count i = 0; i < StaffHeights.n(); i++)
          StaffHeights[i] -= Bound.Bottom();
        
        //Position the stamps in the system.
        for(prim::count i = 0; i < Instants.n(); i++)
          for(prim::count j = 0; j < Instants[i].n(); j++)
            if(prim::Pointer<Stamp> s = Instants[i][j])
              s->Context = Affine::Translate(prim::planar::Vector(
                InstantPositions[i], StaffHeights[j]));
        
        //Adjust stamps which are different due to repeating or non-repeating.
        AdjustStamps();
        
        //Calculate the bounds of the system.
        prim::planar::Rectangle NewBounds;
        for(prim::count i = 0; i < Instants.n(); i++)
          for(prim::count j = 0; j < Instants[i].n(); j++)
            if(prim::Pointer<Stamp> s = Instants[i][j])
              NewBounds += s->BoundsInContext();
        Bounds = NewBounds;
      }
      
      ///Gets the line-space of a coordinate given in spaces.
      mica::UUID CoordinateToLineSpace(prim::count StaffOnSystem,
        prim::planar::Vector SpacesRelativeToSystem)
      {
        //Get the relative line space position.
        prim::number LSPosition =
          SpacesRelativeToSystem.y - StaffHeights[StaffOnSystem];
        
        /*Note this line assumes integer spacing for lines. In reality, this is
        needs to ask the house style since spaces above the staff can be
        condensed.*/
        prim::integer LSIndex = prim::Round(LSPosition * 2.0);
        
        return mica::item(mica::LineSpaces, LSIndex, mica::LS0);
      }
      
      ///Gets the line space of a coordinate in inches.
      mica::UUID CoordinateToLineSpace(prim::count StaffOnSystem,
        Inches RelativeToSystem, const House& h)
      {
        return CoordinateToLineSpace(StaffOnSystem, RelativeToSystem /
          h.SpaceHeight);
      }
      
      ///Gets the line space of a coordinate in inches.
      mica::UUID CoordinateToLineSpace(graph::NoteNode* nn,
        Inches RelativeToSystem, const House& h)
      {
        if(!nn) return mica::Undefined;
        graph::Island* i = nn->ParentIsland();
        if(!i || !i->Typesetting) return mica::Undefined;
        return CoordinateToLineSpace(i->Typesetting->PartID, RelativeToSystem,
          h);
      }
      
      private:
      
      ///Temporary hacks to adjust differences in repeated/non-repeated stamps.
      void AdjustStamps()
      {
        //If there are no instants, then just return.
        if(!Instants.n()) return;
        
        /*Resize non-repeating clefs to be smaller. Note the starting index is
        not technically general. There could be a score with no barline or key
        signature with a clef [0] and then immediate change of clef [1]. A
        better approach would be to determine if the stamp is dependent or
        independent.*/
        for(prim::count i = 2; i < Instants.n(); i++)
        {
          for(prim::count j = 0; j < Instants[i].n(); j++)
          {
            if(prim::Pointer<Stamp> s = Instants[i][j])
            {
              graph::ClefToken* ct = 0;
              if(s->Parent->Find(ct, graph::ID(mica::TokenLink)))
              {
                //Hack: constant should come from house style.
                for(prim::count k = 0; k < s->Graphics.n(); k++)
                  s->Graphics[k]->a *= Affine::Scale(0.8);
              }
            }
          }
        }
        
        //Check to see if the staff will take staff lines.
        HasStaffLines.n(Instants.a().n());
        HasStaffLines.Zero();
        for(prim::count j = 0; j < HasStaffLines.n(); j++)
        {
          for(prim::count i = 0; i < Instants.n(); i++)
          {
            if(prim::Pointer<Stamp> s = Instants[i][j])
            {
              graph::ChordToken* ct = 0;
              if(s->Parent->Find(ct, graph::ID(mica::TokenLink)))
              {
                HasStaffLines[j] = true;
                break;
              }
            }
          }
        }
      }
      
      ///Temporary hack to space the instants.
      void EqualSpace(prim::number SystemWidth)
      {
        //Make sure there are more than one instants to space.
        if(InstantPositions.n() <= 1) return;
        
        //Calculate extra space per instant.
        prim::number ExtraSpace = SystemWidth - MinimumSystemWidth;
        prim::number ExtraSpacePerInstant = ExtraSpace /
          (prim::number)(InstantPositions.n() - 1);
        
        //Add equal extra space.
        for(prim::count i = 0; i < InstantPositions.n(); i++)
        {
          InstantPositions[i] += ((prim::number)i) * ExtraSpacePerInstant;
        }
      }
      
      public:
      
      ///Paints the system.
      void Paint(Painter& Painter, prim::planar::Vector BottomLeftPosition =
        prim::planar::Vector(), prim::number SpaceHeight = 1.0,
        ClickThroughManager* CTM = 0)
      {
        prim::number LineThickness = 0.15;
      
        //If there are no instants, then just return.
        if(!Instants.n()) return;
        
        //Translate and scale the system.
        Painter.Translate(BottomLeftPosition);
        Painter.Scale(SpaceHeight);
        
        /*Quickly calculate the bounds to determine where staff lines should be
        painted. Note the i += Instants.n() - 1 optimization.*/
        prim::planar::Rectangle Bounds;
        prim::count Increment = (Instants.n() > 1 ? Instants.n() - 1 : 1);
        for(prim::count i = 0; i < Instants.n(); i += Increment)
          for(prim::count j = 0; j < Instants[i].n(); j++)
            if(prim::Pointer<Stamp> s = Instants[i][j])
              Bounds += s->BoundsInContext();
        
        //Paint the staff lines.
        Painter.SetFill(Colors::black);
        Path p;
        Shapes::AddLine(p, prim::planar::Vector(LineThickness * 0.5, 0),
          prim::planar::Vector(Bounds.Right() - LineThickness * 0.5, 0),
          LineThickness);
        for(prim::count i = 0; i < Instants.a().n(); i++)
        {
          if(!HasStaffLines[i]) continue;
          for(prim::count j = -2; j <= 2; j++)
            Painter.Draw(p, Affine::Translate(prim::planar::Vector(0,
              StaffHeights[i] + (prim::number)j)));
        }
        
        //Paint the stamps in the system.
        for(prim::count i = 0; i < Instants.n(); i++)
        {
          for(prim::count j = 0; j < Instants[i].n(); j++)
          {
            if(prim::Pointer<Stamp> s = Instants[i][j])
            {
              if(CTM)
              {
                //Apply selection coloring.
                for(prim::count k = 0; k < s->Graphics.n(); k++)
                  s->Graphics[k]->ApplySelectionColoring(
                    CTM->GetSelectionIndex());
              }

              //Paint
              s->Paint(&Painter);
               
              if(CTM)
              {
                //Add each constituent stamp graphic to the click through.
                for(prim::count k = 0; k < s->Graphics.n(); k++)
                  CTM->AddStampGraphicForConsideration(s->Graphics[k]);
              }
            }
          }
        }
        
#if 0
        //Paint the bounding box.
        {
          Path b;
          Shapes::AddRectangleFromLines(b, System::Bounds, 0.1);
          Painter.Draw(b);
        }
#endif
        
        //Undo the transformation.
        Painter.Revert(2);
      }
      
      //Prints the instant positions of the system.
      operator prim::String () const
      {
        prim::String s;
        s << InstantPositions;
        return s;
      }
    };
    
    ///Breaks the music up into systems.
    void CreateSystems(prim::List<System>& Systems,
      prim::number FirstSystemWidth, prim::number RemainingSystemWidth)
    {
      //Initialize a list of systems.
      Systems.RemoveAll();

      //Cache the part and instant count for reference.
      const prim::count PartCount = GraphGeometry.GetNumberOfParts();
      const prim::count InstantCount = GraphGeometry.GetNumberOfInstants();
      
      /*Each system is delineated by a start and end instant, that is to say a
      system contains a continuous range of instants from the total group of
      instants.*/
      prim::count StartInstant = 0, NextStartInstant = 0;

      /*Keep track of repeated instants. Repeated instants are things like 
      clefs, key signatures and barlines.*/
      RepeatedInstants Repeated;
      
      /*While there are instants to still consider create systems and place
      instants on them.*/
      while(NextStartInstant < InstantCount)
      {
        //Consider all the instants in the previous system for repeating.
        if(Systems.n())
        {
          for(prim::count i = 0; i < Systems.z().Instants.n(); i++)
          {
            prim::Debug >> "Considering for repeat: " << i;
            Repeated.Consider(Systems.z().Instants[i]);
          }
        }
        
        //Start new system and create entries for the leading edge.
        System& Current = Systems.Add();
        Current.LeadingEdge.n(PartCount);
        Current.LeadingEdge.Zero();
        
        /*Deep copy all the repeated elements to the front of the system. The
        stamps need to be deep copied because repeated elements are technically
        different stamps since they may have a different position.*/
        prim::Debug >> "Repeated: " << Repeated.n();
        for(prim::count i = 0; i < Repeated.n(); i++)
        {
          //Deep copy the instant.
          Current.Instants.Add().DeepCopyFrom(Repeated[i]);
          
          //Advance leading edge.
          CalculateNextLeadingEdge(Current.Instants.z(), Current.LeadingEdge,
            Current.InstantPositions.Add(Current.LastInstantPosition()));
        }
        
        /*The furthest wrap point is the right side of the wrap point and is
        exclusive to the current system (the beginning of the next system).*/
        prim::count FurthestWrapPoint = Current.Instants.n();
        
        //Add as many instants to system as will fit.
        for(prim::count i = StartInstant; i < InstantCount; i++)
        {
          //Create the stamp instant from the graph instant.
          Current.Instants.Add() = StampInstant(
            GraphGeometry.TopMostIslandInInstant(i), PartCount);
          
          //Advance leading edge.
          prim::number FurthestRight =
            CalculateNextLeadingEdge(Current.Instants.z(), Current.LeadingEdge,
            Current.InstantPositions.Add(Current.LastInstantPosition()));
          
          prim::Debug >> i << ": " << FurthestRight;
          
          //Debug print
          for(prim::count j = 0; j < Current.Instants.z().n(); j++)
          {
            if(Current.Instants.z()[j])
            {
              graph::MusicNode* p = Current.Instants.z()[j]->Parent;
              if(!p)
              {
                prim::c >> "Warning: Stamp with no parent";
                continue;
              }
              prim::Array<graph::Token*> t;
              p->FindAll(t, graph::ID(mica::TokenLink));
              for(prim::count k = 0; k < t.n(); k++)
                prim::Debug >> "  " << j << ": " << t[k]->ToString();
            }
          }
          
          /*If the system width has been exceeded, remove anything beyond the
          wrap point and break.*/
          prim::number MaximumSystemWidth =
            (Systems.n() == 1 ? FirstSystemWidth : RemainingSystemWidth);
          if(FurthestRight > MaximumSystemWidth)
          {
            prim::Debug >> "Removing from: " << FurthestWrapPoint;
            Current.Instants.RemoveFrom(FurthestWrapPoint);
            Current.InstantPositions.RemoveFrom(FurthestWrapPoint);
            prim::Debug >> "Instants kept: " << Current.Instants.n();
            
            if(StartInstant == NextStartInstant)
            {
              prim::c >> "Error: Could not break music";
              Systems.RemoveAll();
              return;
            }
            
            StartInstant = NextStartInstant;
            prim::Debug >> "Next start: " << StartInstant;
            break;
          }
          
          /*If this instant is a potential break point, or this is the last
          instant of the last system, then update the wrap point and starting
          instant for the next system (or signal effectively that the main while
          loop should break).*/
          if(Current.Instants.z().Properties.IsOptionalBreak() ||
            i == InstantCount - 1)
          {
            prim::Debug >> "Break point: " << Current.Instants.n();
            prim::Debug >> "  i: " << i;
            FurthestWrapPoint = Current.Instants.n();
            NextStartInstant = i + 1;
          }
        }
      }
    }
    
    void Prepare(prim::List<System>& Systems, prim::number FirstSystemWidth,
      prim::number RemainingSystemWidth)
    {
      //Typeset any graphics that need it.
      Typeset();
      
      //Create the systems.
      prim::number FirstSystemWidthSpaces = FirstSystemWidth / h->SpaceHeight;
      prim::number RemainingSystemWidthSpaces =
        RemainingSystemWidth / h->SpaceHeight;
      prim::number BreathingRoom = 1.5;
      CreateSystems(Systems, FirstSystemWidthSpaces / BreathingRoom,
        RemainingSystemWidthSpaces / BreathingRoom);
      
      //Space the systems.
      for(prim::count i = 0; i < Systems.n(); i++)
      {
        prim::number DesiredSystemWidth;
        if(i == 0 && i != Systems.n() - 1)
          DesiredSystemWidth = FirstSystemWidthSpaces;
        else if(i < Systems.n() - 1)
          DesiredSystemWidth = RemainingSystemWidthSpaces;
        else
          DesiredSystemWidth = 0.0; //Use minimum spacing
        
        Systems[i].CalculateSpacing(h->StaffDistance, DesiredSystemWidth);
        
        if(i == Systems.n() - 1)
          Systems[i].CalculateSpacing(h->StaffDistance,
            Systems[i].Bounds.Width() * BreathingRoom);
      }
    }
  };
}}
#endif

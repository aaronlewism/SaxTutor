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

//------------------------------------------------------------------------------

/*
Tutorial 3: Using MICA, the Music Information and Concept Archive

This tutorial explains how musical information is encoded in Belle using the
MICA library, which is a separate standalone library developed by author with
the specific aim of providing a means to differentiate and relate musical
concepts in a general way.

On Mac/Linux you can build and run from the terminal using:
Scripts/MakeAndRun Tutorial3

For more information related to building, see the README.
*/

//------------------------------------------------------------------------------

/*Include Belle, Bonne, Sage and compile it in this .cpp file. See the previous
tutorials for an explanation.*/
#define BELLEBONNESAGE_COMPILE_INLINE
#include "../BelleBonneSage.h"
using namespace prim;

/*MICA lives in the mica namespace. There are thousands of identifiers (the
'concepts') in the namespace so it is very important to not use
'using namespace mica' as this will pollute the global namespace with these
thousands of concepts no doubt causing name conflicts. See below for the 
recommended usage of the mica:: namespace.*/

//------------------------------------------------------------------------------

int main()
{
  //-------------------//
  //Part 1 - Using MICA//
  //-------------------//
  
  /*Create a MICA concept and assign it. Concept is the only end-user type in
  MICA.*/
  mica::Concept x = mica::GMajor;

  /*Get the record name. This is an overload for the const char* operator. You
  can also call name(x) directly.*/
  c >> "[GMajor]: " << x;
  c >> "[GMajor] Name: " << name(x); //e.g. mica::name(x)
  
  //Get the hex value of the record.
  c >> "[GMajor] Hex:  " << hex(x); //e.g. mica::hex(x)
  
  /*Note in the previous two examples that the function actually called was
  mica::name(x) and mica::hex(x). Because of argument-dependent name lookup, in
  many cases it is unnecessary to specify the namespace to the function. This is
  a C++ language feature, not a MICA feature.*/
  
  //You can also get the high and low 64-bit halves of the UUID directly.
  c >> "[GMajor] High: " << x.high;
  c >> "[GMajor] Low:  " << x.low;
  
  /*You can also convert a text string to a Concept. Note that since named()
  takes a const char string, you will need the mica:: namespace to call the
  function unlike the other mica functions.*/
  
  //Case insensitive searches work.
  c >> "'gmajor': " << mica::named("gmajor");
  
  /*But a case sensitive search is always performed first, so that exact matches
  will return.*/
  c >> "'f': " << mica::named("f"); //Means the dynamic marking for forte
  c >> "'F': " << mica::named("F"); //Means the pitch letter F
  
  /*Note of caution: in MICA a concept is defined by its underlying UUID, not by
  its name (though in many cases the UUID has been generated from the name using
  a hashing algorithm, but this is not generally so due to name changes). You
  should not rely on named() except within a single known revision of the
  library as names can change without notice in order to improve the library.*/
  
  //Determine the key signature of G Major.
  c >> "[GMajor KeySignature]: " << map(x, mica::KeySignature);
  
  //Note this is an unordered map so the order of arguments does not matter.
  c >> "[KeySignature GMajor]: " << map(mica::KeySignature, x);
  
  //If a mapping does not exist, Undefined is returned.
  c >> "[GMajor FMajor]: " << map(mica::GMajor, mica::FMajor);

  /*If a mapping does not exist directly, chained mapping is used to see if it
  can be created through a series of mappings. In this example, D Flat Major
  is not an explicit map, but [D Flat] and [DFlat Major] are. The rule for
  solving chained mappings is that the largest left-anchored sequence is
  solved if possible and then the process continues. For example,
  [D Flat Major] -> [DFlat Major] -> DFlatMajor.*/
  c >> "[D Flat]: " << map(mica::D, mica::Flat);
  c >> "[DFlat Major]: " << map(mica::DFlat, mica::Major);
  c >> "[D Flat Major]: " << map(mica::D, mica::Flat, mica::Major);

  /*There are a lot of neat mapping opportunities using chained mappings. For
  example, the following takes a key and transposes the key signature up an
  interval and takes that key signature and applies a different mode to it.*/
  c >> "[CMajor KeySignature MajorSecond Up Dorian]: ";
  c << map(mica::CMajor, mica::KeySignature, mica::MajorSecond, mica::Up,
    mica::Dorian);

  /*Some concepts in MICA are special in that they define sequences of other
  concepts. Sequences have a non-zero length and may be cyclic.*/
  c >> "[Letters] Sequence? " << sequence(mica::Letters);
  c >> "[Letters] Cyclic?   " << cyclic(mica::Letters);
  c >> "[Letters] Length?   " << length(mica::Letters);
  
  /*Items in a sequence are indexable through the item() method. The indexing
  is zero-based. The following enumerates the Letters sequence*/
  c >> "[Letters]:";
  for(count i = 0; i < length(mica::Letters); i++)
    c << " " << item(mica::Letters, i);
  
  /*For many potential reasons, you may want to get the ith item relative to
  another item in the sequence. For example, you may want to start on C in
  Letters instead. Note that the sequence automatically wraps around to A after
  G because this sequence is cyclic.*/
  c >> "[Letters] relative to C:";
  for(count i = 0; i < length(mica::Letters); i++)
  {
    /*Equivalent to item(mica::Letters, mica::C, i). The Sequence is always
    given first. The order of the other arguments does not matter.*/
    c << " " << item(mica::Letters, i, mica::C);
  }
  
  /*Cyclic wrapping occurs modulo the length of the sequence and works with
  negative numbers.*/
  c >> "Letters[-100]: " << item(mica::Letters, -100);
  c >> "Letters[100]: " << item(mica::Letters,  100);
  
  //You may also want to lookup the index of an item in a sequence.
  c >> "Letters[C]: " << index(mica::Letters, mica::C);
  
  /*If the concept does not belong to the sequence, then mica::UndefinedIndex is
  returned, which is defined as the most negative value for a 32-bit int.*/
  c >> "Letters[GMajor]: " << index(mica::Letters, mica::GMajor);
  
  //You can test directly for UndefinedIndex...
  if(index(mica::Letters, mica::GMajor) == mica::UndefinedIndex)
    c >> "Letters[GMajor] is undefined";
  else
    c >> "Letters[GMajor] is defined -- oops";

  /*Or you can simply test using the operator overload to mica::Undefined to
  keep things consistent.*/
  if(index(mica::Letters, mica::GMajor) == mica::Undefined)
    c >> "Letters[GMajor] is undefined";
  else
    c >> "Letters[GMajor] is defined -- oops";
  
  //MICA has safeguards to prevent interpretting UndefinedIndex as an index.
  c >> "Letters[Letters[C]]: " <<
    item(mica::Letters, index(mica::Letters, mica::C));
  c >> "Letters[Letters[GMajor]]: " <<
    item(mica::Letters, index(mica::Letters, mica::GMajor));

  /*There is no way to create an error in MICA. If a mapping or indexing
  fails to find a matching concept, Undefined is returned. Undefined will
  propogate through MICA calls silently which means that you only need to check
  the result, not the intermediate steps. You should never assume that a mapping
  succeeded if the input is arbitary. Always check to see if it is Undefined.*/
  for(count i = 0; i < 10; i++)
  {
    mica::Concept y = mica::DynamicStandard;
    mica::Concept z = item(y, i);
    mica::Concept v = map(z, mica::MIDIVelocity);
    if(v != mica::Undefined)
      c >> "[DynamicStandard[" << i << "] MIDIVelocity]: " << z << " " << v;
    else
      c >> "No dynamic for " << i;
  }
  
  //---------------------------//
  //Part 2 - Practical Examples//
  //---------------------------//
  
  //-------//
  //Letters//
  //-------//
  
  /*Get index of letter relative to another letter. If either of the concepts
  does not exist in the sequence, then UndefinedIndex (Undefined) is returned.*/
  c >> "Letters[E, C]: " << index(mica::Letters, mica::E, mica::C);
  if(index(mica::Letters, mica::GMajor, mica::C) == mica::Undefined)
    c >> "Letters[GMajor, C] is undefined";
  else
    c >> "Letters[GMajor, C] is defined -- oops";
  
  //Get 100th letter above another letter.
  c >> "Letters[100, C]: " << item(mica::Letters, 100, mica::C);
               
  //-----------//
  //Accidentals//
  //-----------//
  
  //Get index of accidental relative to another accidental.
  c >> "AccidentalsBySemitone[Flat, Natural]: " <<
    index(mica::AccidentalsBySemitone, mica::Flat, mica::Natural);
  
  //Get 3rd quarter-tone accidental above another accidental.
  c >> "AccidentalsByQuarterTone[3, Natural]: " <<
    item(mica::AccidentalsByQuarterTone, 3, mica::Natural);

  //-------//
  //Octaves//
  //-------//
  
  //Get index of octave.
  c >> "Octaves[Octave4]: " << index(mica::Octaves, mica::Octave4);
  
  /*Get full range index of octave relative to octave 0. Notice how the concept
  Octave-1 is transcribed as Octave_1 in C++ since the hyphen can not be part
  of the identifier.*/
  c >> "OctavesExtended[Octave-1, Octave0]: " <<
    index(mica::OctavesExtended, mica::Octave_1, mica::Octave0);

  //-----//
  //Notes//
  //-----//
  
  //Get the notename given a letter and accidental.
  c >> "[D Flat]: " << map(mica::D, mica::Flat);
  
  //Get a note given a letter, accidental, and octave number.
  c >> "[D Flat Octave4]: " << map(mica::D, mica::Flat, mica::Octave4);
  
  //Get a 'tonal note' (note with no accidental).
  c >> "[D Octave4]: " << map(mica::D, mica::Octave4);
  
  //Convert note into tonal note (remove accidental).
  c >> "[DFlat4 TonalNote]: " << map(mica::DFlat4, mica::TonalNote);
  
  //Get notename of note (or tonal note).
  c >> "[DFlat4 NoteName]: " << map(mica::DFlat4, mica::NoteName);
  
  //Get accidental of note (or tonal note).
  c >> "[DFlat4 Accidental]: " << map(mica::DFlat4, mica::Accidental);
  
  //Get letter of note (or tonal note).
  c >> "[DFlat4 Letter]: " << map(mica::DFlat4, mica::Letter);
  
  //Get octave of note (or tonal note).
  c >> "[DFlat4 Octave]: " << map(mica::DFlat4, mica::Octave);
  
  //Get accidental of notename (not implemented yet)
  //c >> "[DFlat Accidental]: " << map(mica::DFlat, mica::Accidental);

  //Get letter of notename (not implemented yet)
  //c >> "[DFlat Letter]: " << map(mica::DFlat, mica::Letter);

  //-------------//
  //Staffed Notes//
  //-------------//

  //Get line-space of tonal note given clef.
  c >> "[D4 TrebleClef]: " << map(mica::D4, mica::TrebleClef);
  
  //Get line-space of any note given clef.
  c >> "[DSharp4 TonalNote BassClef]: " <<
    map(mica::DSharp4, mica::TonalNote, mica::BassClef);
  
  //Get tonal note from line-space in given clef.
  c >> "[LS0 TrebleClef]: " << map(mica::LS0, mica::TrebleClef);
  
  //Get note from line-space in given clef with accidental.
  c >> "[LS0 TrebleClef Flat]: " <<
    map(mica::LS0, mica::TrebleClef, mica::Flat);

  //---------//
  //Intervals//
  //---------//
  
  //Combine distance with quality.
  c >> "[Fourth Perfect]: " << map(mica::Fourth, mica::Perfect);
  
  //Key signature transposition.
  c >> "[TwoSharps MinorSecond Up]: " <<
    map(mica::TwoSharps, mica::MinorSecond, mica::Up);

  //Distance of interval.
  c >> "[PerfectFourth Distance]: " << map(mica::PerfectFourth, mica::Distance);
  
  //Quality of interval.
  c >> "[PerfectFourth Quality]: " << map(mica::PerfectFourth, mica::Quality);
  
  //Transpose note up interval.
  c >> "[D4 MinorThird Up]: " << map(mica::D4, mica::MinorThird, mica::Up);
  
  //Transpose note down interval.
  c >> "[D4 MinorThird Down]: " << map(mica::D4, mica::MinorThird, mica::Down);
  
  //Within-octave interval between two pitches.
  c >> "[D5 B4]: " << map(mica::D5, mica::B4);
               
  //----//
  //Keys//
  //----//
  
  //Create key from notename and mode (Major).
  c >> "[DFlat Major]: " << map(mica::DFlat, mica::Major);

  //Create key from notename and mode (Dorian).
  c >> "[DFlat Dorian]: " << map(mica::DFlat, mica::Dorian);

  //Get key signature from key (Major).
  c >> "[DFlatMajor KeySignature]: " <<
    map(mica::DFlatMajor, mica::KeySignature);

  //Get key signature from key (Dorian).
  c >> "[DFlatDorian KeySignature]: " <<
    map(mica::DFlatDorian, mica::KeySignature);
  
  //Get mode from key.
  c >> "[DFlatMajor Mode]: " << map(mica::DFlatMajor, mica::Mode);

  //Get mode from key.
  c >> "[DFlatDorian Mode]: " << map(mica::DFlatDorian, mica::Mode);
               
  //--------//
  //Dynamics//
  //--------//

  //Get combined dynamic level and articulation as dynamic.
  c >> "[ff StrongMarcatoTenuto]: " << map(mica::ff, mica::StrongMarcatoTenuto);
  
  //Get dynamic level of sforzato.
  c >> "[sffz DynamicLevel]: " << map(mica::sffz, mica::DynamicLevel);
  
  //Get accent of sforzato.
  c >> "[sffz Accent]: " << map(mica::sffz, mica::Accent);
  
  //Get simple approximate MIDI velocity of dynamic.
  c >> "[ff MIDIVelocity]: " << map(mica::ff, mica::MIDIVelocity);
  
  //Get fourth dynamic in sequence.
  c >> "DynamicStandard[4]: " << item(mica::DynamicStandard, 4);
  
  //Get fourth dynamic in sequence.
  c >> "DynamicStandardWithMezzoPiano[4]: " <<
    item(mica::DynamicStandardWithMezzoPiano, 4);
  
  //Get fourth dynamic in sequence.
  c >> "DynamicExtended[4]: " << item(mica::DynamicExtended, 4);
  
  //Get fourth dynamic in sequence.
  c >> "DynamicExtendedWithMezzoPiano[4]: " <<
    item(mica::DynamicExtendedWithMezzoPiano, 4);
  
  //Get index of MIDI value.
  c >> "MIDIValues[MIDIValue42]: " <<
    index(mica::MIDIValues, mica::MIDIValue42);

  //Get index of dynamic.
  c >> "DynamicStandard[ff]: " <<
    index(mica::DynamicStandard, mica::ff);

  //----------//
  //Set Theory//
  //----------//

  //PCs to PCSet
  c >> "[PC0 PC1 PC4]: " << map(mica::PC0, mica::PC1, mica::PC4);

  //Add a blank line so that the terminal begins on a new line upon quit.
  c++;
  
  return 0;
}


/*
When it comes to the encoding of musical information digitally, it becomes
quickly apparent that there are a lot of concepts involved in music. There are
also relationships between these concepts. For a simple example, consider that
the two concepts "pitch C4" and "pitch D4" are related by a third concept,
a "major second".

The interesting thing about musical concepts is that they tend to define
things semantically. For example, C-Sharp 4 and C-Flat 4 denote different
concepts. However, C-Natural 4 and C4 do not. Or, put another way, the
difference between them is so small, that to differentiate between them would
require introducing a concept in which an accidental is expressed or not
expressed, which is not necessarily useful.

This can lead to the choice of label (or variable name in a programming
language) being quite arbitrary. For example, one could construct arguments
both for and against ad infinitum about the virtues of using C-Natural 4 over
C4, and vice versa. Therefore, the concept name in MICA is seen as a
placeholder, as even if there were a "best" name for the concept, it would
still be monolingual.

Therefore, MICA asserts concept identity through IDs. All concepts which need
to be expressed are assigned a UUID.

For example, C4 (or whatever you want to call it) is defined as the following
record:

Record
------
Name:     C4
Hex:      bdfae27a-4341-3edc-8dee-bebea409f1d9
Sequence: false

The concept has been assigned a unique identifier. To keep things simple,
MICA uses a type of UUID generation which hashes a string, in this case the
string of the concept name.

However, it should be stressed that in MICA, a concept name does not represent
a concept, it is only a semantic key to look up the concept using the current
name to ID mapping. For example, if in the future it was decided that
CNatural4 was really a better name for this concept, then the ID would not
change (because the underlying concept has not changed, only its description):

Record
------
Name:     CNatural4 (hypothetically speaking)
Hex:      bdfae27a-4341-3edc-8dee-bebea409f1d9
Sequence: false

There are some types of concepts which have a subtype of "sequence". Sequences
are ordered and indexable collections of other concepts. For example, the
Letters sequence is defined:

Record
------
Name:     Letters
Hex:      c0a07ac1-2fe5-3b5c-a01c-a97dd48ffcea
Sequence: true
Length:   7 (cyclic)

A B C D E F G 

It is called cyclic because if you ask for the 7th item you would get G, and
if you ask for the 8th item you get A, and so on, modulo the length of the
sequence.

Concepts can also be related to one another via maps. For example:

C4 D4

Is mapped to:

Record
------
Name:     MajorSecond
Hex:      f92265ac-e77b-30e6-960c-efbbb733edc5
Sequence: false

It is called an unordered map, because the order of the arguments does not
matter. D4 C4 also returns major second. Some mappings will be ordered and thus
only give their result for a particular ordering of the arguments.

In the event that two things do not have a relationship, the special concept
Undefined is returned. For example:

TrebleClef BassClef

has no mapping and results in:

Record
------
Name:     Undefined
Hex:      00000000-0000-0000-0000-000000000000
Sequence: false

Undefined is special in that its UUID is set to null, which is convenient for
many situations in programming such as error detection and compression.
*/

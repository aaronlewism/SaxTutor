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

//[Headers] You can add your own extra header files here...
#define BELLEBONNESAGE_COMPILE_INLINE
#include "../bbs/Examples/Resources.h"
#include "tinyxml2.h"
#include "SaxTypes.h"
//[/Headers]

#include "SaxTutorComponent.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace bbs;
using namespace tinyxml2;
bbs::String DetermineResourcePath();
void LoadResources(Score*);
bool LoadSong();

//[/MiscUserDefs]

//==============================================================================
SaxTutorComponent::SaxTutorComponent ()
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 600);

    //[Constructor] You can add your own custom stuff here..
		//TODO: Clean this shit up, for testing only
		LoadSong();

		//Load fonts and graphs
		LoadResources(&myScore);

		myScore.Canvases.z()->Dimensions = bbs::Measurement<bbs::Units::Point>(800,600);
		myScore.UpdatePiece();
    //[/Constructor]
}

SaxTutorComponent::~SaxTutorComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SaxTutorComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]
    
    g.fillAll (Colours::white);

    //bbsJuce.paint(&myScore, &bbsJuceProperties);
    
    //[UserPaint] Add your own custom painting code here..
		bbsJuceProperties.PageArea = bbs::RectangleInt(0,0,getWidth(),getHeight());
    bbsJuceProperties.PageVisibility = bbs::RectangleInt(0,0,getWidth(), getHeight());
    bbsJuceProperties.PageDimensions = myScore.Canvases.z()->Dimensions;
    bbsJuceProperties.IndexOfCanvas = 0;
    bbsJuceProperties.ComponentContext = this;
    bbsJuceProperties.GraphicsContext = &g;

		myScore.Create<bbs::JUCE>(bbsJuceProperties);
    //[/UserPaint]
}

void SaxTutorComponent::resized()
{
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
bbs::String DetermineResourcePath()
{
  bbs::String dummy;
  if(bbs::File::Read("./GentiumBasicRegular.bellefont", dummy))
    return "./";
  else if(bbs::File::Read("../Resources/GentiumBasicRegular.bellefont", dummy))
    return "../Resources/";
  else if(bbs::File::Read("./Resources/GentiumBasicRegular.bellefont", dummy))
    return "./Resources/";
	else if(bbs::File::Read("/home/amlewis/SaxTutor/gui/Resources/GentiumBasicRegular.bellefont", dummy))
		return "/home/amlewis/SaxTutor/gui/Resources/";
  else
    bbs::c >> "Path to resources could not be determined. (SaxTutorComponent.cpp::DetermineResourcePath()";
  return "";
}


void LoadResources(Score* myScore)
{   
  //Load some typefaces into the font.
  myScore->myFont.Add(bbs::Font::Special1)->ImportFromArray((bbs::byte*)Resources::joie_bellefont);

  //Load a graph.
	bbs::File::Read(DetermineResourcePath() << "tempBBS.xml", myScore->graphXML);

	myScore->LoadTypeface();
}

bool LoadSong()
{
	XMLDocument doc;
	//TODO: Choose what song to load
	if (doc.LoadFile(DetermineResourcePath() << "Songs/DrunkenLullabies.xml")) {
		//TODO: Error
		//Some XML file reading error occured.
		return false;
	}

	//Find the tenr part for the song
	//TODO: Confirm it's P4 for all songs
	XMLElement* partRoot = NULL;
	for (XMLElement* cur = doc.FirstChildElement("score-partwise")->FirstChildElement("part");
			 cur != NULL; cur = cur->NextSiblingElement("part")) {
		if (cur->Attribute("id", "P4")) {
			partRoot = cur;
			break; //found Tenrz
		}
	}

	//Tenr part not found
	if (partRoot == NULL) {
		//TODO: error
		return false;
	}

  //Parse part;
	std::vector<sax::Measure> measures;
	double quarterDuration = 256;
	int beat = 4;
	int beatType = 4;
	std::string key = "CMajor";
	for (XMLElement* curMeasure = partRoot->FirstChildElement("measure");
			 curMeasure != NULL;
			 curMeasure = curMeasure->NextSiblingElement("measure")) {
		bbs::c >> "Measure #: " << curMeasure->IntAttribute("number") << "\n";

		//Parse Measure
		//Check for attributes.
		XMLElement* attributes = curMeasure->FirstChildElement("attributes");
		if (attributes != NULL) {
			//Update quarterDuration
			XMLElement* curDivisions = attributes->FirstChildElement("divisions");
			if (curDivisions != NULL) {
				int val = atoi(curDivisions->GetText());
				if (val > 0) {
					quarterDuration = val;
				} else {
					bbs::c >> "Parse error! Divisions needs positive text.\n";
				}
			}

			//Update key
			XMLElement* curKey = attributes->FirstChildElement("key");
			if (curKey != NULL) {
				XMLElement* curFifths = curKey->FirstChildElement("fifths");
				if (curFifths != NULL) {
					int val = atoi(curFifths->GetText());
					key = sax::fifthToMajorString(val);
				} else {
					bbs::c >> "Parse error! Key needs fifth.\n";
				}
			}

			//Update time signature
			XMLElement* curTime = attributes->FirstChildElement("time");
			if (curTime != NULL) {
				XMLElement* curBeats = curTime->FirstChildElement("beats");
				XMLElement* curBeatType = curTime->FirstChildElement("beat-type");
				if (curBeats != NULL && curBeatType != NULL) {
					int val1 = atoi(curBeats->GetText());
					int val2 = atoi(curBeatType->GetText());
					if (val1 > 0 && val2 > 0) {
						beat = val1;
						beatType = val2;
					} else {
						bbs::c >> "Parse error! Time needs positive beat and beatType.\n";
					}
				} else {
					bbs::c >> "Parse error! Time needs both beat and beatType.\n";
				}
			}
		}

		sax::Measure meas(key, beat, beatType, quarterDuration);
		//Parse notes of current measure
		for (XMLElement* curNote = curMeasure->FirstChildElement("note");
				 curNote != NULL;
				 curNote = curNote->NextSiblingElement("note")) {
			sax::NOTES pitch;
			double duration;
			int alter = 0;
	
			//Parse out the pitch
			XMLElement* curRest = curNote->FirstChildElement("rest");
			XMLElement* curPitch = curNote->FirstChildElement("pitch");
			if (curRest != NULL && curPitch == NULL) {
				pitch = sax::REST;
			} else if (curPitch != NULL) {
				XMLElement* curStep = curPitch->FirstChildElement("step");
				XMLElement* curOctave = curPitch->FirstChildElement("octave");
				if (curStep != NULL && curOctave != NULL) {
					char letter = curStep->GetText()[0];
					int oct = atoi(curOctave->GetText());
					
					int val = 0;
					switch (letter) {
						case 'C':
							val = -6;
							break;
						case 'D':
							val = -5;
							break;
						case 'E':
							val = -4;
							break;
						case 'F':
							val = -3;
							break;
						case 'G':
							val = -2;
							break;
						case 'A':
							val = -1;
							break;
						case 'B':
							val = 0;
							break;
					};
					val += 7 * (oct - 4);

					if (letter >= 'A' && letter <= 'G' && oct >= 3 && oct <= 6 && 
							val >= -9 && val <= 15) {
						pitch = (sax::NOTES)val;
					} else {
						pitch = sax::REST;
						bbs::c >> "Parse error! Step and octave are not within bounds.\n";
					}
				} else {
					bbs::c >> "Parse error! Pitch needs both step and octave.\n";
				}

				XMLElement* curAlter = curPitch->FirstChildElement("alter");
				if (curAlter != NULL) {
					alter = atoi(curAlter->GetText());
				}
			} else {
				bbs::c >> "Parse error! Note needs either rest or pitch.\n";
			}
			
			//Parse out the duration. Trust type over duration element
			XMLElement* curType = curNote->FirstChildElement("type");
			XMLElement* curDuration = curNote->FirstChildElement("duration");
			if (curType != NULL) {
				double baseVal = sax::typeToFraction(curType->GetText());
				
				if (baseVal <= 0) {
					bbs::c >> "Parse error! Duration type not found.\n";
					continue;
				}

				double baseMultiplier = 1.0;
				int numDots = 0;
				for (XMLElement* curDot = curNote->FirstChildElement("dot");
						 curDot != NULL;
						 curDot = curDot->NextSiblingElement("dot")) {
					numDots++;
					baseMultiplier += 1.0 / (1 << numDots);
				}
	
				duration = baseMultiplier * baseVal * quarterDuration * 4;
			} else if (curDuration != NULL) {
				duration = atoi(curDuration->GetText());
				if (duration <= 0) {
					continue;
					bbs::c >> "Parse error! Duration needs positive value.\n";
				}
			} else {
				bbs::c >> "Parse error! Note needs either type or duration!\n";
				continue;
			}

			meas.notes.push_back(sax::Note(sax::NotePitch(pitch, alter), duration));
		}

		//Check measure for accuracy?
		measures.push_back(meas);
	}

	//Fill first measure with rests
	double usedDur = 0;
	double restDur = (measures[0].quarterDuration * 4) / measures[0].beatType; 
	for (int i = 0; i < measures[0].notes.size(); ++i) {
		usedDur += measures[0].notes[i].duration;
	}
	usedDur = measures[0].quarterDuration * 4 - usedDur;
	while (usedDur != 0) {
		double curRest = usedDur > restDur ? restDur : usedDur;
		measures[0].notes.insert(measures[0].notes.begin(), 
														 sax::Note(sax::NotePitch(sax::REST, 0),curRest));
		usedDur -= curRest;
	}

	//Add a measure of metronome rests
	sax::Measure metro = measures[0];
	metro.notes.clear();
	for (int i = 0; i < metro.beat; ++i) {
		metro.notes.push_back(sax::Note(sax::NotePitch(sax::REST, 0), restDur));
	}
	measures.insert(measures.begin(), metro);

	//Woooo! Now time to write music out in bbs format.
	char buffer[128];
	XMLDocument bbsDoc;
	XMLElement* bbsScore = bbsDoc.NewElement("score");
	XMLElement* prevChord = NULL;
	bbsDoc.InsertFirstChild(bbsScore);
	int item_id = 1;
	int island_id = 0;
	for (int i = 0; i < measures.size(); ++i) {
		XMLElement* curIsland;
		if (i == 0) { //First barline and clef, key and time
			curIsland = bbsDoc.NewElement("island");
			sprintf(buffer, "island:0,%d", island_id++);
			curIsland->SetAttribute("id", buffer);
			sprintf(buffer, "island:0,%d", island_id);
			curIsland->SetAttribute("across", buffer);
			sprintf(buffer, "island:1,%d", island_id-1);
			curIsland->SetAttribute("down", buffer);
			XMLElement* curBarline = bbsDoc.NewElement("barline");
			sprintf(buffer, "barline:%d", item_id++);
			curBarline->SetAttribute("id", buffer);
			curBarline->SetAttribute("value", "StandardBarline");
			curIsland->InsertFirstChild(curBarline);
			bbsScore->InsertEndChild(curIsland);

			curIsland = bbsDoc.NewElement("island");
			sprintf(buffer, "island:0,%d", island_id++);
			curIsland->SetAttribute("id", buffer);
			sprintf(buffer, "island:0,%d", island_id);
			curIsland->SetAttribute("across", buffer);
			sprintf(buffer, "island:1,%d", island_id-1);
			curIsland->SetAttribute("down", buffer);
			XMLElement* curClef = bbsDoc.NewElement("clef");
			sprintf(buffer, "clef:%d", item_id++);
			curClef->SetAttribute("id", buffer);
			curClef->SetAttribute("value", "TrebleClef");
			curIsland->InsertFirstChild(curClef);
			bbsScore->InsertEndChild(curIsland);

			curIsland = bbsDoc.NewElement("island");
			sprintf(buffer, "island:0,%d", island_id++);
			curIsland->SetAttribute("id", buffer);
			sprintf(buffer, "island:0,%d", island_id);
			curIsland->SetAttribute("across", buffer);
			sprintf(buffer, "island:1,%d", island_id-1);
			curIsland->SetAttribute("down", buffer);
			XMLElement* curKey = bbsDoc.NewElement("key");
			sprintf(buffer, "key:%d", item_id++);
			curKey->SetAttribute("id", buffer);
			curKey->SetAttribute("key", measures[i].key.c_str());
			curIsland->InsertFirstChild(curKey);
			bbsScore->InsertEndChild(curIsland);

			curIsland = bbsDoc.NewElement("island");
			sprintf(buffer, "island:0,%d", island_id++);
			curIsland->SetAttribute("id", buffer);
			sprintf(buffer, "island:0,%d", island_id);
			curIsland->SetAttribute("across", buffer);
			sprintf(buffer, "island:1,%d", island_id-1);
			curIsland->SetAttribute("down", buffer);
			XMLElement* curMeter = bbsDoc.NewElement("meter");
			sprintf(buffer, "meter:%d", item_id++);
			curMeter->SetAttribute("id", buffer);
			curMeter->SetAttribute("value", 
				sax::timeFromBeat(measures[i].beat, measures[i].beatType).c_str());
			curIsland->InsertFirstChild(curMeter);
			bbsScore->InsertEndChild(curIsland);
		} else {
			//Add key and/or time if either of them changed since last element
			if (measures[i].key.compare(measures[i-1].key) != 0) {
				curIsland = bbsDoc.NewElement("island");
				sprintf(buffer, "island:0,%d", island_id++);
				curIsland->SetAttribute("id", buffer);
				sprintf(buffer, "island:0,%d", island_id);
				curIsland->SetAttribute("across", buffer);
				sprintf(buffer, "island:1,%d", island_id-1);
				curIsland->SetAttribute("down", buffer);
				XMLElement* curKey = bbsDoc.NewElement("key");
				sprintf(buffer, "key:%d", item_id++);
				curKey->SetAttribute("id", buffer);
				curKey->SetAttribute("value", measures[i].key.c_str());
				curIsland->InsertFirstChild(curKey);
				bbsScore->InsertEndChild(curIsland);
			}
		
			if (measures[i].beat != measures[i-1].beat ||
					measures[i].beatType != measures[i-1].beatType) {
				curIsland = bbsDoc.NewElement("island");
				sprintf(buffer, "island:0,%d", island_id++);
				curIsland->SetAttribute("id", buffer);
				sprintf(buffer, "island:0,%d", island_id);
				curIsland->SetAttribute("across", buffer);
				sprintf(buffer, "island:1,%d", island_id-1);
				curIsland->SetAttribute("down", buffer);
				XMLElement* curMeter = bbsDoc.NewElement("meter");
				sprintf(buffer, "meter:%d", item_id++);
				curMeter->SetAttribute("id", buffer);
				curMeter->SetAttribute("value", 
					sax::timeFromBeat(measures[i].beat, measures[i].beatType).c_str());
				curIsland->InsertFirstChild(curMeter);
				bbsScore->InsertEndChild(curIsland);
			}
		}

		//Add notes
		double netDuration = 0;
		for (int j = 0; j < measures[i].notes.size(); ++j) {
			sax::Note note = measures[i].notes[j];
			double md = 4 * measures[i].quarterDuration;
	
			//Base island
			curIsland = bbsDoc.NewElement("island");
			sprintf(buffer, "island:0,%d", island_id++);
			curIsland->SetAttribute("id", buffer);
			sprintf(buffer, "island:0,%d", island_id);
			curIsland->SetAttribute("across", buffer);
			sprintf(buffer, "island:1,%d", island_id-1);
			curIsland->SetAttribute("down", buffer);

			//Base chord
			XMLElement* curChord = bbsDoc.NewElement("chord");
			sprintf(buffer, "chord:%d", item_id++);
			curChord->SetAttribute("id", buffer);
			if (prevChord != NULL) {
				prevChord->SetAttribute("next", buffer);
			}
			sprintf(buffer, "%d/64", sax::fracTo64(note.duration/md));
			curChord->SetAttribute("duration", buffer);
			sprintf(buffer, "%d/64", sax::fracTo64(netDuration/md));
			curChord->SetAttribute("beat", buffer);
			sprintf(buffer, "%d/64", sax::fracTo64(note.duration/md));
			curChord->SetAttribute("instant", buffer);

			//Note
			XMLElement* curNote = bbsDoc.NewElement("note");
			sprintf(buffer, "note:%d", item_id++);
			curNote->SetAttribute("id", buffer);
			if (note.pitch.basePitch == sax::REST) {
				curNote->SetAttribute("position", "LS0");
				curNote->SetAttribute("modifier", "Rest");
			} else {
				sprintf(buffer, "LS%d", note.pitch.basePitch);
				curNote->SetAttribute("position", buffer);
				if (note.pitch.alter == -1) {
					curNote->SetAttribute("modifier", "Flat");
				} else if (note.pitch.alter == 1) {
					curNote->SetAttribute("modifier", "Sharp");
				} else {
					curNote->SetAttribute("modifier", "Natural");
				}
			}
			measures[i].notes[j].island_id = island_id - 1;

			//Add elements to each other.
			curChord->InsertFirstChild(curNote);
			curIsland->InsertFirstChild(curChord);
			bbsScore->InsertEndChild(curIsland);

			//Prep for next
			netDuration += note.duration;
			prevChord = curChord;
		}

		//Closing barline
		if (i == measures.size() - 1) {
			curIsland = bbsDoc.NewElement("island");
			sprintf(buffer, "island:0,%d", island_id++);
			curIsland->SetAttribute("id", buffer);
			sprintf(buffer, "island:1,%d", island_id-1);
			curIsland->SetAttribute("down", buffer);
			XMLElement* curBarline = bbsDoc.NewElement("barline");
			sprintf(buffer, "barline:%d", item_id++);
			curBarline->SetAttribute("id", buffer);
			curBarline->SetAttribute("value", "FinalDoubleBarline");
			curIsland->InsertFirstChild(curBarline);
			bbsScore->InsertEndChild(curIsland);
		} else {
			curIsland = bbsDoc.NewElement("island");
			sprintf(buffer, "island:0,%d", island_id++);
			curIsland->SetAttribute("id", buffer);
			sprintf(buffer, "island:0,%d", island_id);
			curIsland->SetAttribute("across", buffer);
			sprintf(buffer, "island:1,%d", island_id-1);
			curIsland->SetAttribute("down", buffer);
			XMLElement* curBarline = bbsDoc.NewElement("barline");
			sprintf(buffer, "barline:%d", item_id++);
			curBarline->SetAttribute("id", buffer);
			curBarline->SetAttribute("value", "StandardBarline");
			curIsland->InsertFirstChild(curBarline);
			bbsScore->InsertEndChild(curIsland);
		}
	}

	for (int i = 0; i < island_id; ++i) {
		XMLElement* curIsland = bbsDoc.NewElement("island");
		sprintf(buffer, "island:1,%d", i);
		curIsland->SetAttribute("id", buffer);
		if (i < island_id - 1) {
			sprintf(buffer, "island:1,%d", i+1);
			curIsland->SetAttribute("across", buffer);
		}
		bbsScore->InsertEndChild(curIsland);
	}

	//TODO: Fine actual output file.
	bbsDoc.SaveFile((DetermineResourcePath() << "tempBBS.xml"));

	return true;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SaxTutorComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="800" initialHeight="600">
  <BACKGROUND backgroundColour="ffffffff"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  5 Dec 2012 8:04:44pm

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
#include <pthread.h>
//[/Headers]

#include "SaxTutorComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace bbs;
using namespace tinyxml2;
bbs::String DetermineResourcePath();
bbs::String convertString(juce::String);
juce::String convertString(bbs::String);
void LoadResources(Score*);
void ReloadGraph(Score*);
bool LoadSong(std::vector<sax::Measure>&, bbs::String path);
bool writeSongToBBSXml(std::vector<sax::Measure>&, bbs::String path);
void* playSong(void* ptr);
//[/MiscUserDefs]

//==============================================================================
SaxTutorComponent::SaxTutorComponent ()
    : tempoSlider (0),
      playButton (0),
      tempoLabel (0),
      fileButton (0)
{
    addAndMakeVisible (tempoSlider = new Slider ("Tempo Slider"));
    tempoSlider->setRange (40, 300, 1);
    tempoSlider->setSliderStyle (Slider::LinearHorizontal);
    tempoSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    tempoSlider->addListener (this);

    addAndMakeVisible (playButton = new TextButton ("Play Button"));
    playButton->setButtonText ("Play");
    playButton->addListener (this);

    addAndMakeVisible (tempoLabel = new Label ("Tempo  Label",
                                               "Tempo:"));
    tempoLabel->setFont (juce::Font (15.0000f, juce::Font::plain));
    tempoLabel->setJustificationType (Justification::centredRight);
    tempoLabel->setEditable (false, false, false);
    tempoLabel->setColour (TextEditor::textColourId, Colours::black);
    tempoLabel->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (fileButton = new TextButton ("file button"));
    fileButton->setButtonText ("Load File");
    fileButton->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 600);


    //[Constructor] You can add your own custom stuff here..
		boolLock  = PTHREAD_MUTEX_INITIALIZER;
		playThread.comp = this;
		saxThread.startThread();

		//Default tempo
		tempoSlider->setValue(132);

		//TODO: Clean this shit up, for testing only
		//TODO: Choose what song to load.
		//TODO: Error handling
		LoadSong(song, DetermineResourcePath() << "Songs/RubySoho.xml");
		writeSongToBBSXml(song, DetermineResourcePath() << "tempBBS.xml");

		//Load fonts and graphs
		LoadResources(&myScore);

		myScore.Canvases.z()->Dimensions = bbs::Measurement<bbs::Units::Point>(800,600);
		myScore.UpdatePiece();
    //[/Constructor]
}

SaxTutorComponent::~SaxTutorComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
		playThread.signalThreadShouldExit();
		playThread.waitForThreadToExit(2000);
		playThread.stopThread(2000);
		saxThread.signalThreadShouldExit();
		saxThread.waitForThreadToExit(2000);
		saxThread.stopThread(2000);
    //[/Destructor_pre]

    deleteAndZero (tempoSlider);
    deleteAndZero (playButton);
    deleteAndZero (tempoLabel);
    deleteAndZero (fileButton);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SaxTutorComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

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
    tempoSlider->setBounds (proportionOfWidth (0.1270f), proportionOfHeight (0.0105f), proportionOfWidth (0.2290f), proportionOfHeight (0.0395f));
    playButton->setBounds (proportionOfWidth (0.3991f), proportionOfHeight (0.0105f), proportionOfWidth (0.1996f), proportionOfHeight (0.0395f));
    tempoLabel->setBounds (proportionOfWidth (0.0045f), proportionOfHeight (0.0105f), proportionOfWidth (0.1202f), proportionOfHeight (0.0395f));
    fileButton->setBounds (proportionOfWidth (0.6391f), proportionOfHeight (0.0105f), proportionOfWidth (0.1991f), proportionOfHeight (0.0394f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SaxTutorComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == tempoSlider)
    {
        //[UserSliderCode_tempoSlider] -- add your slider handling code here..
        //[/UserSliderCode_tempoSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void SaxTutorComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == playButton)
    {
        //[UserButtonCode_playButton] -- add your button handler code here..
				if (!playThread.isThreadRunning()) {
					playButton->setButtonText("Stop Song");
					playThread.startThread();
				} else {
					playButton->setButtonText("Play Song");
					playThread.signalThreadShouldExit();
				}
        //[/UserButtonCode_playButton]
    }
    else if (buttonThatWasClicked == fileButton)
    {
        //[UserButtonCode_fileButton] -- add your button handler code here..
				juce::FileChooser chooser("Please select the song you want to load...",
					juce::File(convertString(DetermineResourcePath() << "Songs/")),
					"*.xml");
				if (chooser.browseForFileToOpen()) {
					bbs::String filePath = 
						convertString(chooser.getResult().getFullPathName());
					if (LoadSong(song, filePath)) {
						writeSongToBBSXml(song, DetermineResourcePath() << "tempBBS.xml");
						ReloadGraph(&myScore);
						myScore.UpdatePiece();
						repaint();
					} else {
						bbs::c >> "Couldn't load song: " << filePath << "\n";
					}
				}
        //[/UserButtonCode_fileButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
const std::vector<sax::Measure> SaxTutorComponent::getSong() {
	return song;
}

double SaxTutorComponent::getTempo() {
	return tempoSlider->getValue();
}

void SaxTutorComponent::colorNote(sax::Note n, bbs::Color c) {
	//Find islandId
	static int island_id = 0;
	static int id = 0;
	static int sysInd = 0;

	if (island_id != n.island_id) {
		if (island_id < n.island_id) {
			id += n.island_id - island_id;
		} else {
			id = n.island_id;
			sysInd = 0;
		}
		island_id = n.island_id;
		while (id >= myScore.Systems[sysInd].Instants.n()) {
			id -= myScore.Systems[sysInd].Instants.n();
			id += 3; //barline, clef, key
			sysInd++;
		}
	}

	//TODO Race risk?
	//pthread_mutex_lock(&(myScore.systemsLock));
	bbs::Pointer<bbs::modern::Stamp> stamp = myScore.Systems[sysInd].Instants[id][0];
	for (bbs::count k = 0; k < stamp->Graphics.n(); k++)
		stamp->Graphics[k]->c = c;
	//pthread_mutex_unlock(&(myScore.systemsLock));
}

int SaxTutorComponent::getCurrentSaxPitchValue() {
	return saxThread.note;
}

void PlaySongThread::run() {
	const std::vector<sax::Measure> song = comp->getSong();
	double tempo = comp->getTempo();
	double beat = 1000 * (60 / tempo); //Milliseconds per beat;
	double beatBase = 0;
	int mIndex = 0;
	int nIndex = 0;
	int netBeats = 0;
	double nextNoteBeat = 0;
	double lastNoteBeat = 0;
	int curBeatCorrectTicks = 0;
	int curBeatTotalTicks = 0;

	//Black out the entire song;
	for (int i = 0; i < song.size(); ++i)
		for (int j = 0; j < song[i].notes.size(); ++j)
			comp->colorNote(song[i].notes[j], bbs::Colors::black);

	//Start playing the song
	comp->colorNote(song[mIndex].notes[nIndex], bbs::Colors::blue);
	{
		const MessageManagerLock mmLock;
		comp->repaint();
	}
	nextNoteBeat += (song[0].notes[0].duration / song[0].quarterDuration);
	double startTime = juce::Time::getMillisecondCounterHiRes();
	while(true) {
		if (threadShouldExit()) { //Check for exit
			return;
		}

		{
			const MessageManagerLock mmLock;
			double curTime = juce::Time::getMillisecondCounterHiRes();

			if (comp->getTempo() != tempo) {//Check for new tempo
				//TODO: Error with how beat is calculated (based on last time sample);
				beatBase += (curTime - startTime) / beat;
				startTime = curTime;
				tempo = comp->getTempo();
				beat = 1000 * (60 / tempo); //Milliseconds per beat;
			}
		}

		double curTime = juce::Time::getMillisecondCounterHiRes();
		double curBeat = beatBase + (curTime-startTime)/ beat;
		if (curBeat >= nextNoteBeat) {
			//Color note based on correctness
			bbs::float64 fred = 0.0;
			bbs::float64 fblue = 0.0;
			bbs::float64 fgreen = 0.0;;
			if (curBeatCorrectTicks > curBeatTotalTicks/2.0) {
				double diff = (2.0*(curBeatCorrectTicks - (curBeatTotalTicks/2.0)) / curBeatTotalTicks);
				fblue = diff;
				fgreen = (1 - diff);
				if (fgreen > fblue) {
					fblue /= fgreen;
					fgreen = 1;
				} else {
					fgreen /= fblue;
					fblue = 1;
				}
			} else {
				double diff = (-2.0*(curBeatCorrectTicks - (curBeatTotalTicks/2.0)) / curBeatTotalTicks);
				fred = diff;
				fgreen = (1 - diff);
				if (fgreen > fred) {
					fred /= fgreen;
					fgreen = 1;
				} else {
					fgreen /= fred;
					fred = 1;
				}
			}
			comp->colorNote(song[mIndex].notes[nIndex], bbs::Color(fred, fgreen, fblue));

			while (curBeat >= nextNoteBeat) {
				//Increment note (checking for end of song
				nIndex++;
				if (nIndex >= song[mIndex].notes.size()) {
					//Measure has ended
					nIndex = 0;
					mIndex++;
					if (mIndex >= song.size()) {
						//Song has ended
						{
							const MessageManagerLock mmLock;
							comp->repaint();
						}
						break;
					}
				}

				//Find when this beat ends.
				lastNoteBeat = nextNoteBeat;
				nextNoteBeat += (song[mIndex].notes[nIndex].duration /
				song[mIndex].quarterDuration);
			}

			if (mIndex >= song.size()) {
				break;
			}
			if (threadShouldExit()) { //Check for exit
				return;
			}


			curBeatCorrectTicks = 0;
			curBeatTotalTicks = 0;

			comp->colorNote(song[mIndex].notes[nIndex], bbs::Colors::blue);
			{
				const MessageManagerLock mmLock;
				comp->repaint();
			}
		}

		//TODO Find correctness
		curBeatCorrectTicks += comp->getCurrentSaxPitchValue() == 0;
		curBeatTotalTicks++;

		//Color note based on correctness
		if (curBeatTotalTicks % 5 == 0) {
			bbs::float64 fred = 0.0;
			bbs::float64 fblue = 0.0;
			bbs::float64 fgreen = 0.0;
			double fracPlayed = (curBeat - lastNoteBeat) / (nextNoteBeat - lastNoteBeat);
			fgreen += (1 - fracPlayed);
			if (curBeatCorrectTicks > curBeatTotalTicks/2.0) {
				double diff = (2.0*(curBeatCorrectTicks - (curBeatTotalTicks/2.0)) / curBeatTotalTicks);
				fblue += fracPlayed * diff;
				fgreen += fracPlayed * (1 - diff);
				if (fgreen > fblue) {
					fblue /= fgreen;
					fgreen = 1;
				} else {
					fgreen /= fblue;
					fblue = 1;
				}
			} else {
				double diff = (-2.0*(curBeatCorrectTicks - (curBeatTotalTicks/2.0)) / curBeatTotalTicks);
				fred += fracPlayed * diff;
				fgreen += fracPlayed * (1 - diff);
				if (fgreen > fred) {
					fred /= fgreen;
					fgreen = 1;
				} else {
					fgreen /= fred;
					fred = 1;
				}
			}
			if (threadShouldExit()) { //Check for exit
				return;
			}
			comp->colorNote(song[mIndex].notes[nIndex], bbs::Color(fred, fgreen, fblue));
			{
				const MessageManagerLock mmLock;
				comp->repaint();
			}
		}
	}
	return;
}

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

bbs::String convertString(juce::String str) {
	bbs::String ret = "";
	for (int i = 0; i < str.length(); ++i) {
		ret = ret << prim::unicode(str[i]);
	}
	return ret;
}

juce::String convertString(bbs::String str) {
	juce::String ret = "";
	for (bbs::count i = 0; i < str.c(); ++i) {
		ret += wchar_t(str[i]);
	}
	return ret;
}


void LoadResources(Score* myScore)
{
  //Load some typefaces into the font.
  myScore->myFont.Add(bbs::Font::Special1)->ImportFromArray((bbs::byte*)Resources::joie_bellefont);

  //Load a graph.
	bbs::File::Read(DetermineResourcePath() << "tempBBS.xml", myScore->graphXML);

	myScore->LoadTypeface();
}

void ReloadGraph(Score* myScore) {
	bbs::File::Read(DetermineResourcePath() << "tempBBS.xml", myScore->graphXML);
}

bool LoadSong(std::vector<sax::Measure>& measures, bbs::String path)
{
	XMLDocument doc;
	if (doc.LoadFile(path)) {
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
	measures.clear();
	bool isRepeating = false;
	int repeatStart = 0;
	int repeatEnd = 0;
	double quarterDuration = 256;
	int beat = 4;
	int beatType = 4;
	std::string key = "CMajor";
	for (XMLElement* curMeasure = partRoot->FirstChildElement("measure");
			 curMeasure != NULL;
			 curMeasure = curMeasure->NextSiblingElement("measure")) {
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

			//Check for repeat
			XMLElement* curStyle = attributes->FirstChildElement("measure-style");
			if (curStyle != NULL) {
				XMLElement* curRepeat = curStyle->FirstChildElement("measure-repeat");
				if (curRepeat != NULL) {
					if (curRepeat->Attribute("type", "start")) {
						int val = atoi(curRepeat->GetText());
						repeatEnd = measures.size();
						repeatStart = repeatEnd - val;
						isRepeating = true;
					} else {
						isRepeating = false;
					}
				}
			}
		}

		sax::Measure meas(key, beat, beatType, quarterDuration);
		//Parse notes of current measure
		if (isRepeating) {
			int measNum = measures.size();
			int repeatFrom = ((measNum - repeatEnd) % (repeatEnd - repeatStart)) +
											 repeatStart;
			meas.notes = measures[repeatFrom].notes;
		} else {
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

	return true;
}

bool writeSongToBBSXml(std::vector<sax::Measure>& measures, bbs::String path)
{
	//TODO Error handling
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
	bbsDoc.SaveFile(path);

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
  <SLIDER name="Tempo Slider" id="3e0f570e11323395" memberName="tempoSlider"
          virtualName="" explicitFocusOrder="0" pos="12.771% 1.127% 22.944% 3.944%"
          min="40" max="160" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="Play Button" id="c5cdc5f1f4c52113" memberName="playButton"
              virtualName="" explicitFocusOrder="0" pos="39.827% 1.127% 19.913% 3.944%"
              buttonText="Play" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Tempo  Label" id="cf3337e4bb19765b" memberName="tempoLabel"
         virtualName="" explicitFocusOrder="0" pos="0.433% 1.127% 12.121% 3.944%"
         edTextCol="ff000000" edBkgCol="0" labelText="Tempo:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="file button" id="9414e03ce07246a0" memberName="fileButton"
              virtualName="" explicitFocusOrder="0" pos="63.853% 1.127% 19.913% 3.944%"
              buttonText="Load File" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

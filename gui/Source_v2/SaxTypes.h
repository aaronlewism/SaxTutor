#ifndef __SAX_TUTOR_TYPES
#define __SAX_TUTOR_TYPES

#include <string>
#include <vector>

namespace sax {

	//New Enums
	enum NOTES { //Values are linked to location on staff
		REST = -100,
		G3 = -9,
		A3 = -8,
		B3 = -7,
		C4 = -6,
		D4 = -5,
		E4 = -4,
		F4 = -3,
		G4 = -2,
		A4 = -1,
		B4 = 0,
		C5,
		D5,
		E5,
		F5,
		G5,
		A5,
		B5,
		C6,
		D6,
		E6,
		F6,
		G6,
		A6,
		B6
	};

	//New structures and classes
	struct NotePitch {
		NOTES basePitch;
		double alter; //True alter, for the sound (i.e. uses the key)

		NotePitch(NOTES b, double a)
			:basePitch(b),
			alter(a) {
		}

		//Returns a comparable value for the pitch (so A# equals Bb)
		double pitchValue() {
			//Assumes B4 = 0;
			int base = (basePitch / 7) * 12;
			base = (basePitch < 0) ? base - 12 : base;
			switch (basePitch % 7) {
				case 6:
					base += 4;
					break;
				case 5:
					base += 3;
					break;
				case 4:
				case 3:
					base += 2;
					break;
				case 2:
					base += 1;
					break;
					//Don't need anything for 0 or 1
			};

			return base + alter;
		}
	};

	//Combines NotePitch with duration
	struct Note{
		NotePitch pitch;
		double duration;

		Note(NotePitch p, double d) 
			:pitch(p),
			duration(d) {
		}
	};

	//Largely a list of notes, plus key information
	struct Measure {
		std::string key;
		int beat; //The Top of the time signature
		int beatType; //The bottom of the time signature
		double quarterDuration; //Duration of a quarter note.
		
		std::vector<Note> notes;

		Measure(std::string k, int b, int bT, double qD)
			:key(k),
			beat(b),
			beatType(bT),
			quarterDuration(qD) {
		}
	};

	//New functions
	std::string numToWord(int num) {
		switch (num) {
			case 1:
				return "One";
			case 2:
				return "Two";
			case 3:
				return "Three";
			case 4:
				return "Four";
			case 5:
				return "Five";
			case 6:
				return "Six";
			case 7:
				return "Seven";
			case 8:
				return "Eight";
			case 9:
				return "Nine";
			case 10:
				return "Ten";
			case 11:
				return "Eleven";
			case 12:
				return "Twelve";
			case 13:
				return "Thirteen";
			case 14:
				return "Fourteen";
			case 15:
				return "Fifteen";
			case 16:
				return "Sixteen";
		};

		return "";
	}

	std::string timeFromBeat(int beat, int beatType) {
		return numToWord(beat) + numToWord(beatType);
	}

	std::string fifthToMajorString(int fifth) {
		switch (fifth) {
			case -7:
				return "BMajor"; //TODO: Doesn't Really Work
			case -6:
				return "GFlatMajor";
			case -5:
				return "DFlatMajor";
			case -4:
				return "AFlatMajor";
			case -3:
				return "EFlatMajor";
			case -2:
				return "BFlatMajor";
			case -1:
				return "FMajor";
			case 0:
				return "CMajor";
			case 1:
				return "GMajor";
			case 2:
				return "DMajor";
			case 3:
				return "AMajor";
			case 4:
				return "EMajor";
			case 5:
				return "BMajor";
			case 6:
				return "FSharpMajor";
			case 7:
				return "DFlatMajor"; //TODO: Doesn't Really Work
		}
		
		return "";
	}

	double typeToFraction(std::string text) {
		if (text.compare("1024th") == 0) {
			return 1.0/1024.0;
		} else if (text.compare("512th") == 0) {
			return 1.0/512.0;
		} else if (text.compare("256th") == 0) {
			return 1.0/256.0;
		} else if (text.compare("128th") == 0) {
			return 1.0/128.0;
		} else if (text.compare("64th") == 0) {
			return 1.0/64.0;
		} else if (text.compare("32nd") == 0) {
			return 1.0/32.0;
		} else if (text.compare("16th") == 0) {
			return 1.0/16.0;
		} else if (text.compare("eighth") == 0) {
			return 1.0/8.0;
		} else if (text.compare("quarter") == 0) {
			return 1.0/4.0;
		} else if (text.compare("half") == 0) {
			return 1.0/2.0;
		} else if (text.compare("whole") == 0) {
			return 1.0;
		} else {
			return -1;
		}
	}
};

#endif

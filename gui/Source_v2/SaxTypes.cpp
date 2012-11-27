#include "SaxTypes.h"

namespace sax {

	double NotePitch::pitchValue() const {
		//Assumes B4 = 0;
		int base = (basePitch / 7) * 12;
		base = (basePitch < 0) ? base - 12 : base;
		int note = basePitch % 7;
		note = note < 0 ? note + 7 : note;
		switch (note) {
			case 6:
				note += 4;
				break;
			case 5:
				note += 3;
				break;
			case 4:
			case 3:
				note += 2;
				break;
			case 2:
				note += 1;
				break;
				//Don't need anything for 0 or 1
		};
	
		return base + note + alter;
	}

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

	int fracTo64(double frac) {
		return ((int)(frac*128) + 1) / 2;
	}


};

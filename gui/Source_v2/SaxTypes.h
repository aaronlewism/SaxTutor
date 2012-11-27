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
		double pitchValue() const;
	};

	//Combines NotePitch with duration
	struct Note{
		NotePitch pitch;
		double duration;
		int island_id;

		Note(NotePitch p, double d) 
			:pitch(p),
			duration(d) {
			island_id = 0;
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
	std::string numToWord(int num);	std::string timeFromBeat(int beat, int beatType);

	std::string fifthToMajorString(int fifth);

	double typeToFraction(std::string text);

	int fracTo64(double frac);
};

#endif

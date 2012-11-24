#ifndef __SAX_TUTOR_CONSTANTS
#define __SAX_TUTOR_CONSTANTS

namespace sax {
	enum NOTES { //Values are linked to location on staff
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

	enum ACCIDENTALS {
		EMPTY,
		NATURAL,
		SHARP,
		FLAT
	};

	struct Note {
		NOTES pitch;
		double alter; //True alter, for the sound (i.e. uses the key)
		ACCIDENTALS accidental; //Accidental for display
	};

	double pitchValue(Note note) { //For comparing notes
		//Assumes B4 = 0;
		int base = (note.pitch / 7) * 12;
		base = (note.pitch < 0) ? base - 12 : base;
		switch (note.pitch % 7) {
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

		return base + note.alter;
	}
};

#endif

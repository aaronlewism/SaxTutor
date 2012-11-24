#ifndef __SAX_TUTOR_TYPES
#define __SAX_TUTOR_TYPES

namespace sax {

	//New Enums
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
		EMPTY = 0,
		NATURAL,
		SHARP,
		FLAT
	};

	//New structures and classes
	struct NotePitch {
		NOTES basePitch;
		double alter; //True alter, for the sound (i.e. uses the key)
		ACCIDENTALS accidental; //Accidental for display

		NotePitch() {
			basePitch = C5;
			alter = 0;
			accidental = EMPTY;
		}
	};


	//New functions
	double pitchValue(NotePitch note) { //For comparing notes
		//Assumes B4 = 0;
		int base = (note.basePitch / 7) * 12;
		base = (note.basePitch < 0) ? base - 12 : base;
		switch (note.basePitch % 7) {
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

#include <cstring>
#include "hid.h"
#include "../bbs/BelleBonneSage.h"
#include "SaxUSBThread.h"

#define VENDOR_ID 0x16C0
#define PRODUCT_ID 0x0480
#define RAWHID_USAGE_PAGE	0xFFAB
#define RAWHID_USAGE 0x0200

namespace bbs {
	using namespace prim;
	using namespace prim::planar;
	using namespace bellebonnesage;
	using namespace bellebonnesage::painters;
}


void SaxUSBThread::run() {
	int count = 0;
	int t;
	int32_t _note;
	uint8_t buffer[64];

	bbs::c >> "Sax USB Thread started\n";
	
	while (!hid::rawhid_open(1, VENDOR_ID, PRODUCT_ID, 
													 RAWHID_USAGE_PAGE, RAWHID_USAGE)) {
		if (threadShouldExit()) break;
	}
	bbs::c >> "Raw HID opened\n";

	while (true) {
		if (threadShouldExit()) {
			break;
		}

		while (hid::rawhid_send(0, buffer, 64, 0) <= 0) {
			if (threadShouldExit()) {
				break;
			}
		}

		t = hid::rawhid_recv(0, buffer, 64, 10); //TODO Timeout
		if (t != 64) {
			bbs::c >> "Recv timeout " << t << "\n";
			continue;
		}
		memcpy(&_note, buffer, sizeof(int32_t));
		note = _note;
		//count++;
		//if (0 == (count % 100))
		//	bbs::c >> "Note: " << note << "\n";
	}

	hid::rawhid_close(0);
	bbs::c >> "Sax USB Thread ended\n";
	return;
}

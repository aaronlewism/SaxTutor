#ifndef __SAXTUTOR_SAX_USB_THREAD__
#define __SAXTUTOR_SAX_USB_THREAD__

#include "JuceHeader.h"

class SaxUSBThread : public juce::Thread {
	public:
		SaxUSBThread() : juce::Thread("Sax USB Thread") {}

	private:
		void run();
};

#endif

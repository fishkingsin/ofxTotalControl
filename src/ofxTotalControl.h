#include "p9813.h"
#include "ofMain.h"
class ofxTotalControl
{
public:
	int open(int nStrands, int pixelsPerStrand)
	{
		totalPixels = nStrands * pixelsPerStrand;
		i           = totalPixels * sizeof(TCpixel);
		if(NULL == (pixelBuf = (TCpixel *)malloc(i)))
		{
			printf("Could not allocate space for %d pixels (%d bytes).\n",
				   totalPixels,i);
			return 1;
		}
		
		/* Initialize library, open FTDI device.  Baud rate errors
		 are non-fatal; program displays a warning but continues. */
		if((i = TCopen(nStrands,pixelsPerStrand)) != TC_OK)
		{
			TCprintError(i);
			if(i < TC_ERR_DIVISOR) return 1;
		}
		
		/* Initialize statistics structure before use. */
		TCinitStats(&stats);
	}
	void refresh(ofPixels pix)
	{
		
		if((i = TCrefresh(pixelBuf,NULL,&stats)) != TC_OK)
			TCprintError(i);
		
		/* Update statistics once per second. */
		if((t = time(NULL)) != prev)
		{
#ifdef CYGWIN
			printf("\E[2J");
#else
			system("clear");
#endif
			TCprintStats(&stats);
			prev = t;
		}
	}
	void close()
	{
		TCclose();
		free(pixelBuf);

	}
protected:
	TCstats       stats;
	TCpixel       *pixelBuf;
	
};
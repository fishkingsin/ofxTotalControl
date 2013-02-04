#include "ofMain.h"



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "p9813.h"

class ofxTotalControl
{
public:
	ofxTotalControl()
	{
		
		nStrands           = 1;
		pixelsPerStrand    = 25;
		
		prev = 0;
	}
	int open(int _nStrands, int _pixelsPerStrand)
	{
		nStrands           = _nStrands;
		pixelsPerStrand    = _pixelsPerStrand;
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
//			TCprintError(i);
			if(i < TC_ERR_DIVISOR) return 1;
		}
		
		/* Initialize statistics structure before use. */
		TCinitStats(&stats);
	}
	void refresh(ofPixels pix)
	{
		
		for(i=0;i<totalPixels;i++)
		{
			ofColor c = pix.getColor(i, 0);
			int r   = (int)((c.r *0.5) * 127.5);
			int g   = (int)((c.g *0.5) * 127.5);
			int b   = (int)((c.b *0.5) * 127.5);
			pixelBuf[i] = TCrgb(r,g,b);

		}
		
		if((i = TCrefresh(pixelBuf,NULL,&stats)) != TC_OK)
//			TCprintError(i);
		
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
	int           i,totalPixels,nStrands,pixelsPerStrand;
	TCstats       stats;
	TCpixel       *pixelBuf;
	time_t        t,prev;
	
};


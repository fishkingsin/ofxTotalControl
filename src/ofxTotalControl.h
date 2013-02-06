#include "ofMain.h"

#ifdef TARGET_LINUX_ARM

#include "tclled.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

#else

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "p9813.h"

#endif

//#define RAINBOW_TEST

class ofxTotalControl
{
public:
	ofxTotalControl()
	{
#ifdef TARGET_LINUX_ARM
#ifdef RAINBOW_TEST
		i = 0 ;
		h=  0;
		r = 0;
		g = 0;
		b = 0;
		
#endif
#else
		nStrands           = 1;
		pixelsPerStrand    = 25;
		
		prev = 0;
#endif
	}
#ifdef TARGET_LINUX_ARM
	
	void setup(int _numLEDs , string _device = "/dev/spidev0.0")
	
#else
	void setup(int _nStrands, int _pixelsPerStrand)
	
#endif
	{
#ifdef TARGET_LINUX_ARM
		leds = _numLEDs;
		device = _device;
		/* Open the device file using Low-Level IO */
		fd = open(device.c_str(),O_WRONLY);
		if(fd<0) {
			fprintf(stderr,"Error %d: %s\n",errno,strerror(errno));
			std::exit(1);
		}
		
		/* Initialize the SPI bus for Total Control Lighting */
		return_value = spi_init(fd);
		if(return_value==-1) {
			fprintf(stderr,"SPI initialization error %d: %s\n",errno, strerror(errno));
			std::exit(1);
		}
		
		/* Initialize pixel buffer */
		if(tcl_init(&buf,leds)<0) {
			fprintf(stderr,"Pixel buffer initialization error: Not enough memory.\n");
			std::exit(1);
		}
		
		/* Set the gamma correction factors for each color */
		set_gamma(2.2,2.2,2.2);
		
		/* Blank the pixels */
		for(int i=0;i<leds;i++) {
			write_gamma_color(&buf.pixels[i],0x00,0x00,0x00);
		}
		
		send_buffer(fd,&buf);
#else
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
#endif
	}
	void setPixel(int index ,ofColor color)
	{
#ifdef TARGET_LINUX_ARM
		if(index<leds)
		{
			write_gamma_color(&buf.pixels[index], color.r, color.g, color.b);
		}
		else
		{
			ofLogWarning("ofxTotalControl")<< index <<" out of bounds " << leds;
		}
#else
		if(index<totalPixels)
		{
			
			
			int r   = (int)((color.r *0.5) * 127.5);
			int g   = (int)((color.g *0.5) * 127.5);
			int b   = (int)((color.b *0.5) * 127.5);
			pixelBuf[i] = TCrgb(r,g,b);
			
			
			
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
#endif
	}
	void update(ofPixels pix)
	{
#ifdef TARGET_LINUX_ARM
#ifdef RAINBOW_TEST
		h+=2.0;
		if(h>=360.0) h=0.0;
		memmove(buf.pixels+1,buf.pixels,sizeof(tcl_color)*(leds-2));
		HSVtoRGB(h,1.0,1.0,&r,&g,&b);
		write_gamma_color(buf.pixels,(int)(r*255.0),(int)(g*255.0),(int)floor(b*255.0));
#else
		for(int i = 0 ;i < leds ; i++)
		{
			int index = i*3;
			write_gamma_color(&buf.pixels[i],pix[index],pix[index+1],pix[index+2]);
			//			write_gamma_color(buf.pixels,pix[index],pix[index+1],pix[index+2]);
		}
#endif
		send_buffer(fd,&buf);
#else
		
		for(int i=0;i<totalPixels;i++)
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
#endif
	}
	void exit()
	{
#ifdef TARGET_LINUX_ARM
		for(int i=0;i<leds;i++) {
			write_gamma_color(&buf.pixels[i],0x00,0x00,0x00);
		}
		
		send_buffer(fd,&buf);
		
		tcl_free(&buf);
		close(fd);
#else
		TCclose();
		free(pixelBuf);
#endif
		
	}
#ifdef RAINBOW_TEST
	/* Convert hsv values (0<=h<360, 0<=s<=1, 0<=v<=1) to rgb values (0<=r<=1, etc) */
	void HSVtoRGB(double h, double s, double v, double *r, double *g, double *b) {
		int i;
		double f, p, q, t;
		
		if( s < 1.0e-6 ) {
			/* grey */
			*r = *g = *b = v;
			return;
		}
		
		h /= 60.0;              /* Divide into 6 regions (0-5) */
		i = (int)floor( h );
		f = h - (double)i;      /* fractional part of h */
		p = v * ( 1.0 - s );
		q = v * ( 1.0 - s * f );
		t = v * ( 1.0 - s * ( 1.0 - f ) );
		
		switch( i ) {
			case 0:
				*r = v;
				*g = t;
				*b = p;
				break;
			case 1:
				*r = q;
				*g = v;
				*b = p;
				break;
			case 2:
				*r = p;
				*g = v;
				*b = t;
				break;
			case 3:
				*r = p;
				*g = q;
				*b = v;
				break;
			case 4:
				*r = t;
				*g = p;
				*b = v;
				break;
			default:    // case 5:
				*r = v;
				*g = p;
				*b = q;
				break;
		}
	}
#endif
	int numLEDs()
	{
#ifdef TARGET_LINUX_ARM
		return leds;
#else
		return totalPixels;
#endif
	}
	
protected:
#ifdef TARGET_LINUX_ARM
	tcl_buffer buf;
	int fd;
	int return_value;
	tcl_color *p;
	
#ifdef RAINBOW_TEST
	int i;
	
	double h, r, g, b;
#endif
	string device;
	int leds;
#else
	int           i,totalPixels,nStrands,pixelsPerStrand;
	TCstats       stats;
	TCpixel       *pixelBuf;
	time_t        t,prev;
	
#endif
	
};


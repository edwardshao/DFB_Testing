/*
   DirectFB Tutorials

   (c) Copyright 2000-2002  convergence integrated media GmbH.
   (c) Copyright 2002       convergence GmbH.
   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de> and
              Sven Neumann <neo@directfb.org>.
              
   This file is subject to the terms and conditions of the MIT License:

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without restriction,
   including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so,
   subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 * image.c
 *
 * Image loading and displaying
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include <directfb.h>

#include "mytime.h"

/*
 * (Globals)
 */
static IDirectFB *dfb = NULL;
static IDirectFBSurface *primary = NULL;
static int screen_width  = 0;
static int screen_height = 0;
static int fps = 100;
#define DFBCHECK(x...)                                         \
  {                                                            \
    DFBResult err = x;                                         \
                                                               \
    if (err != DFB_OK)                                         \
      {                                                        \
        fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ ); \
        DirectFBErrorFatal( #x, err );                         \
      }                                                        \
  }

/*
 * The image is to be loaded into a surface that we can blit from.
 */
static IDirectFBSurface *logo = NULL;

void scale(unsigned int *frames)
{
	double b = 0;
	int runs = 400;

	while (runs--) {
		double f;
		DFBRectangle rect;

		primary->FillRectangle( primary, 0, 0, screen_width, screen_height);

		f = cos(b) * 30  +  sin(b+0.5) * 40;

		rect.w = (int)((sin(f*cos(f/10.0))/2 + 1.2)*800);
		rect.h = (int)((sin(f*sin(f/10.0)) + 1.2)*300);
		
		rect.x = (screen_width - rect.w) / 2;
		rect.y = (screen_height - rect.h) / 2;

		primary->StretchBlit( primary, logo, NULL, &rect );

		b += .001;

		/* flip display */
		DFBCHECK(primary->Flip( primary, NULL, DSFLIP_WAITFORSYNC ));

		(*frames)++;
	}	
}

void slide(unsigned int *frames, int step)
{
	int i;
        int logo_width, logo_height;
	time_t start, end, diff;
	int remain_us;
	float interval;

	DFBCHECK(logo->GetSize(logo, &logo_width, &logo_height));

	interval = (float)1000 / (float)fps;
	printf("interval: %f ms\n", interval);

	for (i = -logo_width; i < screen_width; i += step) {
		/*
		 * Clear the screen.
		 */
		start = get_current_time();
		DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));

		/*
		 * Blit the logo vertically centered with "i" as the X coordinate.
		 * NULL means that we want to blit the whole surface.
		 */
		DFBCHECK (primary->Blit (primary, logo, NULL, i, (screen_height - logo_height) / 2));

		/*
		 * Flip the front and back buffer, but wait for the vertical retrace to
		 * avoid tearing.
		 */
		DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));
		end = get_current_time();
		diff = diff_time(&start, &end);
		remain_us = (interval * 1000 - (float)diff);
//		printf("diff: %d us, remain:%d\n", diff, remain_us);
		if (remain_us > 0) {
			usleep(remain_us);
		}

		(*frames)++;
	}
}

void usage(int argc, char *argv[])
{
	fprintf(stderr, "usage: %s [image file name] [step]\n", argv[0]);
}

int main (int argc, char **argv)
{
  int i;
  char *file_name;
  int step;

  if (argc < 2) {
	usage(argc, argv);
	exit(0);
  }

  file_name = argv[1];

  if (argc >= 3)
	step = atoi(argv[2]);
  else
	step = 1;

  if (argc >= 4)
	fps = atoi(argv[3]);

  fprintf(stdout, "image: %s, step: %d, fps: %d\n", file_name, step, fps);

  /*
   * (Locals)
   */
  DFBSurfaceDescription dsc;

  /*
   * Loading an image is done with an Image Provider.
   */
  IDirectFBImageProvider *provider;

  /*
   * (Initialize)
   */
  DFBCHECK (DirectFBInit (&argc, &argv));
  DFBCHECK (DirectFBCreate (&dfb));
  DFBCHECK (dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN));
  dsc.flags = DSDESC_CAPS;
  dsc.caps  = DSCAPS_PRIMARY | DSCAPS_FLIPPING;
  DFBCHECK (dfb->CreateSurface( dfb, &dsc, &primary ));
  DFBCHECK (primary->GetSize (primary, &screen_width, &screen_height));

  /*
   * First we need to create an Image Provider by passing a filename.
   * DirectFB will find (or not) an Image Provider for the file type.
   */
  DFBCHECK (dfb->CreateImageProvider (dfb, file_name, &provider));

  /*
   * Get a surface description from the provider. It will contain the width,
   * height, bits per pixel and the flag for an alphachannel if the image has
   * one. If the image has no alphachannel the bits per pixel is set to the
   * bits per pixel of the primary layer to use simple blitting without pixel
   * format conversion.
   */
  DFBCHECK (provider->GetSurfaceDescription (provider, &dsc));

  /*
   * Create a surface based on the description of the provider.
   */
  DFBCHECK (dfb->CreateSurface( dfb, &dsc, &logo ));
  
  /*
   * Let the provider render to our surface. Image providers are supposed to
   * support every destination pixel format and size. If the size differs the
   * image will be scaled (bilinear). The last parameter allows to specify
   * an optional destination rectangle. We use NULL here so that our image
   * covers the whole logo surface.
   */
  DFBCHECK (provider->RenderTo (provider, logo, NULL));

  /*
   * Release the provider, we don't need it anymore.
   */
  provider->Release (provider);

  /*
   * We want to let the logo slide in on the left and slide out on the right.
   */
 unsigned int frames;
 time_t start, end, diff;
  while (1) {
	frames = 0;

	start = get_current_time();

	slide(&frames, step);
	//scale(&frames);	

	end = get_current_time();
	diff = diff_time(&start, &end);

	printf("screen: (%d, %d), image: (%d, %d)\n", screen_width, screen_height, dsc.width, dsc.height);
	printf("frames: %d (%d ~ %d, %d)\n", frames, start, end, diff);
	printf("FPS: %f\n", ((double)frames / (double)diff) * (double)1000000);

	DFBCHECK (primary->FillRectangle (primary, 0, 0, screen_width, screen_height));
	DFBCHECK (primary->Flip (primary, NULL, DSFLIP_WAITFORSYNC));
	sleep(3);
    }

  /*
   * Release the image.
   */
  logo->Release (logo);

  /*
   * (Release)
   */
  primary->Release (primary);
  dfb->Release (dfb);
  
  return 23;
}


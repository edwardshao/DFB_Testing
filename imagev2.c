#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <directfb.h>

#include "mytime.h"

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

  fprintf(stdout, "image: %s, step: %d\n", file_name, step);

  IDirectFB *dfb;
  DFBGraphicsDeviceDescription dev_desc;
  DFBSurfaceDescription surface_desc;
  DFBWindowDescription window_desc;
  IDirectFBImageProvider *provider;
  IDirectFBDisplayLayer *layer;
  DFBDisplayLayerConfig layer_config;
  IDirectFBWindow *image_window;
  IDirectFBSurface *image_surface;

  DFBCHECK (DirectFBInit(&argc, &argv));
  DFBCHECK (DirectFBCreate(&dfb));

  dfb->GetDeviceDescription(dfb, &dev_desc);
  DFBCHECK(dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &layer));
  layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE);

  if (!((dev_desc.blitting_flags & DSBLIT_BLEND_ALPHACHANNEL) &&
	(dev_desc.blitting_flags & DSBLIT_BLEND_COLORALPHA  )))
     {
          layer_config.flags = DLCONF_BUFFERMODE;
          layer_config.buffermode = DLBM_BACKSYSTEM;

          layer->SetConfiguration(layer, &layer_config);
     }  

  layer->GetConfiguration(layer, &layer_config);
  DFBCHECK(dfb->CreateImageProvider(dfb, file_name, &provider));

  window_desc.flags = ( DWDESC_POSX | DWDESC_POSY |
			DWDESC_WIDTH | DWDESC_HEIGHT);
  window_desc.posx = 0;
  window_desc.posy = 0;
  window_desc.width = layer_config.width;
  window_desc.height = layer_config.height;
  DFBCHECK(layer->CreateWindow(layer, &window_desc, &image_window));
  image_window->GetSurface(image_window, &image_surface);

  // render to surface
  provider->RenderTo(provider, image_surface, NULL);
  provider->Release(provider);

//  image_surface->Flip(image_surface, NULL, 0);

  image_window->SetOpacity(image_window, 0xFF);
  image_window->RaiseToTop(image_window);

  unsigned int frames;
  time_t start, end, diff;
  while (1) {
	frames = 0;
	start = get_current_time(); 
	for (i = 0; i < layer_config.width; i += step) {
		image_surface->Flip(image_surface, NULL, DSFLIP_WAITFORSYNC);
		image_window->MoveTo(image_window, i, 0);
		frames++;
	}
	end = get_current_time();
	diff = diff_time(&start, &end);
	printf("frames: %d (%d ~ %d, %d)\n", frames, start, end, diff);
	printf("FPS: %f\n", ((double)frames / (double)diff) * (double)1000000);
  }

  return 23;
}


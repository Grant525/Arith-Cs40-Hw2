#ifndef IMAGES_H
#define IMAGES_H

#include <stdio.h>
#include <pnm.h>

Pnm_ppm read_ppm(FILE *input);

Pnm_ppm trim(Pnm_ppm pixmap);

#endif


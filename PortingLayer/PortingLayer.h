#ifndef PORTINGLAYER_H_
#define PORTINGLAYER_H_

#include "./GlobalDefine.h"

#include "../Driver/SpiDriver/SpiDriver.h"
#include "../Driver/PixelBase/PixelBase.h"

#include "spi.h"


void createPixelBase(void);
void createSpiMaster(void);


#endif


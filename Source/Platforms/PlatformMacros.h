//
//  PlatformMacros.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-10.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef GameDev2D_PlatformMacros_h
#define GameDev2D_PlatformMacros_h

#if WIN32 && _MSC_PLATFORM_TOOLSET_v110
#define fminf(a,b) ((a) < (b) ? (a) : (b))
#define fmaxf(a,b) ((a) > (b) ? (a) : (b))
#endif

#define RoundToInt(value) ((value > 0.0f) ? (value + 0.5f) : (value - 0.5f))

#define SafeDelete(p)            do { if(p) { delete (p); (p) = nullptr; } } while(0)
#define SafeDeleteArray(p)       do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define SafeFree(p)              do { if(p) { free(p); (p) = 0; } } while(0)

#endif

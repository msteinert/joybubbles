/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Forward type declarations
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_X11_TYPES_H
#define JOY_X11_TYPES_H

#include "joy/device/types.h"
#include "joy/iterator/types.h"
#include "joy/types.h"

G_BEGIN_DECLS

typedef struct JoyX11Application_ JoyX11Application;

typedef struct JoyX11Mouse_ JoyX11Mouse;

typedef struct JoyX11Keyboard_ JoyX11Keyboard;

typedef struct JoyX11Screen_ JoyX11Screen;

typedef struct JoyX11Source_ JoyX11Source;

typedef struct JoyX11Window_ JoyX11Window;

G_END_DECLS

#endif // JOY_X11_TYPES_H

#ifndef VR_INCLUDED
#define VR_INCLUDED

#include "main.h"
#include "render.h"

bool vr_set_enabled( bool enabled );
bool vr_is_enabled( void );
bool vr_init( render_info_t *info );
void vr_shutdown( void );
void vr_update( render_info_t *info );

#endif

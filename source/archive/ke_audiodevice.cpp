//
//  ke_audiodevice.cpp
//
//  Created by Shogun3D on 5/23/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "ke_audiodevice.h"
#include "ke_al_audiodevice.h"
#ifdef _WIN32
/* TODO: XAudio2 */
#endif




/*
 * Creates an audio device based on the user's API preferences 
 */
ke_audiodevice_t* ke_create_audiodevice( ke_audiodevice_desc_t* audiodevice_desc )
{
    if( audiodevice_desc->device_type == KE_AUDIODEVICE_OPENAL )
        return new ke_al_audiodevice_t( audiodevice_desc );
    
#ifdef _WIN32
	/* TODO: XAudio2 */
#endif
    
    return NULL;
}
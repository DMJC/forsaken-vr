#include "vr.h"
#include "util.h"

#ifdef USE_OPENVR
#include <openvr_capi.h>
#endif

static bool vr_enabled = false;
static bool vr_initialized = false;

#ifdef USE_OPENVR
static struct VR_IVRSystem_FnTable *vr_system = NULL;
static struct VR_IVRCompositor_FnTable *vr_compositor = NULL;
#endif

bool vr_set_enabled( bool enabled )
{
	vr_enabled = enabled;
	return true;
}

bool vr_is_enabled( void )
{
	return vr_enabled;
}

bool vr_init( render_info_t *info )
{
	if ( !vr_enabled )
		return true;

#ifdef USE_OPENVR
	EVRInitError init_error = EVRInitError_VRInitError_None;
	uint32_t render_w = 0;
	uint32_t render_h = 0;
	float ipd_meters;

	if ( !VR_IsHmdPresent() )
	{
		Msg("VR requested but no HMD was detected.");
		return false;
	}

	VR_InitInternal( &init_error, EVRApplicationType_VRApplication_Scene );
	if ( init_error != EVRInitError_VRInitError_None )
	{
		Msg("VR initialization failed with error %d.", (int)init_error);
		return false;
	}

	vr_system = VR_GetGenericInterface( IVRSystem_Version, &init_error );
	if ( init_error != EVRInitError_VRInitError_None || !vr_system )
	{
		Msg("VR system interface initialization failed with error %d.", (int)init_error);
		VR_ShutdownInternal();
		return false;
	}

	vr_compositor = VR_GetGenericInterface( IVRCompositor_Version, &init_error );
	if ( init_error != EVRInitError_VRInitError_None || !vr_compositor )
	{
		Msg("VR compositor interface initialization failed with error %d.", (int)init_error);
		VR_ShutdownInternal();
		vr_system = NULL;
		return false;
	}

	vr_system->GetRecommendedRenderTargetSize( &render_w, &render_h );
	ipd_meters = vr_system->GetFloatTrackedDeviceProperty(
		k_unTrackedDeviceIndex_Hmd,
		ETrackedDeviceProperty_Prop_UserIpdMeters_Float,
		NULL
	);

	if ( render_w && render_h )
	{
		info->default_mode.w = (int)( render_w * 2 );
		info->default_mode.h = (int)render_h;
	}

	info->stereo_enabled = true;
	info->stereo_mode = STEREO_MODE_HALF_WIDTH;
	info->stereo_eye_sep = ipd_meters * 1000.0f;
	if ( info->stereo_eye_sep <= 0.0f )
		info->stereo_eye_sep = 63.0f;
	info->stereo_focal_dist = 750.0f;

	vr_initialized = true;
	DebugPrintf(
		"VR initialized. Recommended target: %ux%u per eye, ipd=%.2fmm\n",
		render_w,
		render_h,
		info->stereo_eye_sep
	);
	return true;
#else
	(void) info;
	Msg("VR support was requested but this build was made without USE_OPENVR.");
	return false;
#endif
}

void vr_shutdown( void )
{
	if ( !vr_initialized )
		return;

#ifdef USE_OPENVR
	VR_ShutdownInternal();
	vr_system = NULL;
	vr_compositor = NULL;
#endif
	vr_initialized = false;
}

void vr_update( render_info_t *info )
{
	if ( !vr_initialized || !info )
		return;

#ifdef USE_OPENVR
	TrackedDevicePose_t tracked_device_pose[k_unMaxTrackedDeviceCount];

	if ( vr_compositor )
	{
		vr_compositor->WaitGetPoses(
			tracked_device_pose,
			k_unMaxTrackedDeviceCount,
			NULL,
			0
		);
	}
#endif

	info->stereo_enabled = true;
}

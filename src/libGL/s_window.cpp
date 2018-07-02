#include "s_events.h"
#include "s_globals.h"

ALLEGRO_DISPLAY*        al_mainWindow;

//-----------------------------------------------------------------------------
//
// Swap buffers for displaying screen
void lib_swapBuffers()
//-----------------------------------------------------------------------------
{
	al_flip_display ();
}

//-----------------------------------------------------------------------------
//
// Shutdown the library windowing system
void lib_destroyWindow()
//-----------------------------------------------------------------------------
{
	//al_destroy_display ( al_mainWindow);
}

//-----------------------------------------------------------------------------
//
// Get the version of the window library
bool lib_getVersion()
//-----------------------------------------------------------------------------
{
	uint32_t version = al_get_allegro_version();
	int al_major = version >> 24;
	int al_minor = (version >> 16) & 255;
	int al_revision = (version >> 8) & 255;
	int al_release = version & 255;

	io_logToFile("Allegro Version: %i.%i.%i.%i", al_major, al_minor, al_revision, al_release);
	io_logToFile("CPUs: %i RAM: %i", al_get_cpu_count (), al_get_ram_size ());
	io_logToFile("Video Adapters : %i", al_get_num_video_adapters());

	ALLEGRO_MONITOR_INFO    monitorInfo;
	for (int i = 0; i != al_get_num_video_adapters(); i++)
	{
		al_get_monitor_info(i, &monitorInfo);
		io_logToFile("Monitor [ %i ] Location [ %i %i %i %i ]", i, monitorInfo.x1, monitorInfo.x2, monitorInfo.y1, monitorInfo.y2);
	}

	return true;
}

//-----------------------------------------------------------------------------
//
// Resize the current displayed window
void lib_resizeWindow ( int newWidth, int newHeight )
//-----------------------------------------------------------------------------
{
	al_resize_display(al_mainWindow, newWidth, newHeight);

	winWidth = newWidth;
	winHeight = newHeight;
}

//-----------------------------------------------------------------------------
//
// Start the GL Helper library and open the window
bool lib_openWindow()
//-----------------------------------------------------------------------------
{
	if (!al_install_system (ALLEGRO_VERSION_INT, nullptr)) // NOLINT
		return false;

	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL_3_0 | ALLEGRO_OPENGL_FORWARD_COMPATIBLE );

	al_set_new_display_option (ALLEGRO_OPENGL_MAJOR_VERSION, 3, ALLEGRO_SUGGEST);
	al_set_new_display_option (ALLEGRO_OPENGL_MINOR_VERSION, 3, ALLEGRO_SUGGEST);

	al_mainWindow = al_create_display(winWidth, winHeight);
	if ( nullptr == al_mainWindow)
	{
		io_logToFile("Allegro error : Unable to start display.");
		return false;
	}

	if ( !al_install_mouse())
	{
		al_show_native_message_box (al_mainWindow, "Error", "Error", "Failed to start mouse driver", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

	if ( !al_install_keyboard ())
	{
		al_show_native_message_box (al_mainWindow, "Error", "Error", "Failed to start keyboard driver", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		return false;
	}

	printf("Requested opengl [ 4.3 ] - got [ %i.%i ]\n", al_get_display_option (al_mainWindow, ALLEGRO_OPENGL_MAJOR_VERSION),	al_get_display_option (al_mainWindow, ALLEGRO_OPENGL_MINOR_VERSION));

	if (!evt_registerUserEventSetup())
		return false;

	return true;
}

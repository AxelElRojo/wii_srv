#include <iostream>
#include <map>
#include <gccore.h>
#include <fat.h>
#include <wiiuse/wpad.h>
#include "util.hpp"
#include "server.hpp"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;
void* init();

int main(){
	xfb = init();
	if(fatInitDefault()){
		Server srv(readConfig());
		if(srv.setup())
			std::cout << "Can't setup server\n";
		else
			srv.run();
		fatUnmount(0);
	}else
		std::cout << "Can't init FAT fs\n";
}
void* init(){
	void* framebuffer;
	VIDEO_Init();
	WPAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	framebuffer = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(framebuffer, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(framebuffer);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode & VI_NON_INTERLACE)
		VIDEO_WaitVSync();
	return framebuffer;
}
#ifndef FRAMEBUFFER_EFFECTS_H
#define FRAMEBUFFER_EFFECTS_H

#include "ultra64.h"

extern s32 gPauseFrameBuffer;
extern s32 gBlurFrameBuffer;
extern s32 gReusableFrameBuffer;

void FB_CreateFramebuffers(void);
void FB_CopyToFramebuffer(Gfx** gfxp, s32 fb_src, s32 fb_dest, u8 oncePerFrame, u8* hasCopied);
void FB_DrawFromFramebuffer(Gfx** gfxp, s32 fb, u8 alpha);
void FB_DrawFromFramebufferScaled(Gfx** gfxp, s32 fb, u8 alpha, float scaleX, float scaleY);

#endif // FRAMEBUFFER_EFFECTS_H

#include <ultra64.h>

#include "game_init.h"

#define gfx_ptr gDisplayListHead

void sprite_draw(
	const void *timg, int fmt, int siz, int bilerp,
	unsigned int tw, unsigned int th,
	int x, int y, unsigned int w, unsigned int h
)
{
	unsigned int ty;
	unsigned int tx;
	unsigned int dsdx = (0x400*4*tw + w/2) / w;
	unsigned int dtdy = (0x400*4*th + h/2) / h;
	unsigned int cw = 16 * (0x2448 >> (4*siz) & 15);
	unsigned int ch = 16 * (0x2244 >> (4*siz) & 15);
	int siz_load = siz;
	int shift = 0;
	if (siz == G_IM_SIZ_4b) siz_load = G_IM_SIZ_8b, shift = 1;
	if (bilerp) cw -= 2 << shift, ch -= 2 << shift;
	gDPSetTextureImage(gfx_ptr++, fmt, siz_load, tw >> shift, timg);
	for (ty = 0; ty < th; ty += ch)
	{
		int t;
		int yl;
		int yh;
		unsigned int ult = ty;
		unsigned int lrt = ty + ch;
		if (lrt > th) lrt = th;
		yl = y + ult*h/th;
		yh = y + lrt*h/th;
		if (yh < 0 || yl > 4096) continue;
		t = ult << 5;
		if (yl < 0) t -= (yl*dtdy) >> 7, yl = 0;
		if (bilerp)
		{
			if (ult >  0) ult -= 1 << shift;
			if (lrt < th) lrt += 1 << shift;
			t -= 16;
		}
		for (tx = 0; tx < tw; tx += cw)
		{
			int s;
			int xl;
			int xh;
			unsigned int uls = tx;
			unsigned int lrs = tx + cw;
			unsigned int line;
			if (lrs > tw) lrs = tw;
			xl = x + uls*w/tw;
			xh = x + lrs*w/tw;
			if (xh < 0 || xl > 4096) continue;
			s = uls << 5;
			if (xl < 0) s -= (xl*dsdx) >> 7, xl = 0;
			if (bilerp)
			{
				if (uls >  0) uls -= 1 << shift;
				if (lrs < tw) lrs += 1 << shift;
				s -= 16;
			}
			line = (((lrs-uls) << siz)+14) >> 4;
			gDPPipeSync(gfx_ptr++);
			gDPSetTile(
				gfx_ptr++, fmt, siz_load, line, 0, 0, 0, 0, 0, 0, 0, 0, 0
			);
			gDPLoadSync(gfx_ptr++);
			gDPLoadTile(
				gfx_ptr++, 0,
				(uls  ) << G_TEXTURE_IMAGE_FRAC >> shift,
				(ult  ) << G_TEXTURE_IMAGE_FRAC,
				(lrs-1) << G_TEXTURE_IMAGE_FRAC >> shift,
				(lrt-1) << G_TEXTURE_IMAGE_FRAC
			);
			gDPSetTile(
				gfx_ptr++, fmt, siz, line, 0, 0, 0,
				G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0
			);
			gDPSetTileSize(
				gfx_ptr++, 0,
				(uls  ) << G_TEXTURE_IMAGE_FRAC,
				(ult  ) << G_TEXTURE_IMAGE_FRAC,
				(lrs-1) << G_TEXTURE_IMAGE_FRAC,
				(lrt-1) << G_TEXTURE_IMAGE_FRAC
			);
			gSPTextureRectangle(gfx_ptr++, xl, yl, xh, yh, 0, s, t, dsdx, dtdy);
		}
	}
}

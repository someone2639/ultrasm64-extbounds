#include <ultra64.h>
#include "game_init.h"
#define gfx_ptr gDisplayListHead

void sprite_draw(
    const void *timg, int fmt, int siz, int bilerp,
    unsigned int tw, unsigned int th,
    int x, int y, unsigned int w, unsigned int h
)
{
    static const unsigned char size[] = {128, 64, 64, 32, 64, 64, 32, 32};
    unsigned int ty;
    unsigned int tx;
    unsigned int dsdx = (0x400*4*tw + w/2) / w;
    unsigned int dtdy = (0x400*4*th + h/2) / h;
    unsigned int cw = size[0+siz];
    unsigned int ch = size[4+siz];
    if (bilerp) cw -= 2, ch -= 2;
    gDPSetTextureImage(gfx_ptr++, fmt, siz, tw, timg);
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
            if (ult >  0) --ult;
            if (lrt < th) ++lrt;
            t -= 16;
        }
        for (tx = 0; tx < tw; tx += cw)
        {
            int s;
            int xl;
            int xh;
            unsigned int uls = tx;
            unsigned int lrs = tx + cw;
            if (lrs > tw) lrs = tw;
            xl = x + uls*w/tw;
            xh = x + lrs*w/tw;
            if (xh < 0 || xl > 4096)  continue;
            s = uls << 5;
            if (xl < 0) s -= (xl*dsdx) >> 7, xl = 0;
            if (bilerp)
            {
                if (uls >  0) --uls;
                if (lrs < tw) ++lrs;
                s -= 16;
            }
            gDPPipeSync(gfx_ptr++);
            gDPSetTile(
                gfx_ptr++, fmt, siz, (((lrs-uls) << siz >> 1)+7) >> 3, 0, 0, 0,
                G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD,
                G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD
            );
            gDPLoadSync(gfx_ptr++);
            gDPLoadTile(
                gfx_ptr++, 0,
                (uls  ) << G_TEXTURE_IMAGE_FRAC,
                (ult  ) << G_TEXTURE_IMAGE_FRAC,
                (lrs-1) << G_TEXTURE_IMAGE_FRAC,
                (lrt-1) << G_TEXTURE_IMAGE_FRAC
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

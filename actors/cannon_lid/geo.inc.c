// Empty geo script

const GeoLayout cannon_lid_geo[] = {
    GEO_CULLING_RADIUS(300),
    GEO_OPEN_NODE(),
        GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, cannon_lid_seg8_dl_080048E0),
        GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

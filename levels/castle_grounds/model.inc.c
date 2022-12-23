Lights1 castle_grounds_dl_Material_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Vtx castle_grounds_dl_Cube_mesh_layer_1_vtx_cull[8] = {
	{{ {-1000, -100, 1000}, 0, {-16, -16}, {0, 0, 0, 0} }},
	{{ {-1000, 100, 1000}, 0, {-16, -16}, {0, 0, 0, 0} }},
	{{ {-1000, 100, -1000}, 0, {-16, -16}, {0, 0, 0, 0} }},
	{{ {-1000, -100, -1000}, 0, {-16, -16}, {0, 0, 0, 0} }},
	{{ {1000, -100, 1000}, 0, {-16, -16}, {0, 0, 0, 0} }},
	{{ {1000, 100, 1000}, 0, {-16, -16}, {0, 0, 0, 0} }},
	{{ {1000, 100, -1000}, 0, {-16, -16}, {0, 0, 0, 0} }},
	{{ {1000, -100, -1000}, 0, {-16, -16}, {0, 0, 0, 0} }},
};

Vtx castle_grounds_dl_Cube_mesh_layer_1_vtx_0[24] = {
	{{ {-1000, -100, 1000}, 0, {368, 1008}, {129, 0, 0, 255} }},
	{{ {-1000, 100, 1000}, 0, {624, 1008}, {129, 0, 0, 255} }},
	{{ {-1000, 100, -1000}, 0, {624, 752}, {129, 0, 0, 255} }},
	{{ {-1000, -100, -1000}, 0, {368, 752}, {129, 0, 0, 255} }},
	{{ {-1000, -100, -1000}, 0, {368, 752}, {0, 0, 129, 255} }},
	{{ {-1000, 100, -1000}, 0, {624, 752}, {0, 0, 129, 255} }},
	{{ {1000, 100, -1000}, 0, {624, 496}, {0, 0, 129, 255} }},
	{{ {1000, -100, -1000}, 0, {368, 496}, {0, 0, 129, 255} }},
	{{ {1000, -100, -1000}, 0, {368, 496}, {127, 0, 0, 255} }},
	{{ {1000, 100, -1000}, 0, {624, 496}, {127, 0, 0, 255} }},
	{{ {1000, 100, 1000}, 0, {624, 240}, {127, 0, 0, 255} }},
	{{ {1000, -100, 1000}, 0, {368, 240}, {127, 0, 0, 255} }},
	{{ {1000, -100, 1000}, 0, {368, 240}, {0, 0, 127, 255} }},
	{{ {1000, 100, 1000}, 0, {624, 240}, {0, 0, 127, 255} }},
	{{ {-1000, 100, 1000}, 0, {624, -16}, {0, 0, 127, 255} }},
	{{ {-1000, -100, 1000}, 0, {368, -16}, {0, 0, 127, 255} }},
	{{ {-1000, -100, -1000}, 0, {112, 496}, {0, 129, 0, 255} }},
	{{ {1000, -100, -1000}, 0, {368, 496}, {0, 129, 0, 255} }},
	{{ {1000, -100, 1000}, 0, {368, 240}, {0, 129, 0, 255} }},
	{{ {-1000, -100, 1000}, 0, {112, 240}, {0, 129, 0, 255} }},
	{{ {1000, 100, -1000}, 0, {624, 496}, {0, 127, 0, 255} }},
	{{ {-1000, 100, -1000}, 0, {880, 496}, {0, 127, 0, 255} }},
	{{ {-1000, 100, 1000}, 0, {880, 240}, {0, 127, 0, 255} }},
	{{ {1000, 100, 1000}, 0, {624, 240}, {0, 127, 0, 255} }},
};

Gfx castle_grounds_dl_Cube_mesh_layer_1_tri_0[] = {
	gsSPVertex(castle_grounds_dl_Cube_mesh_layer_1_vtx_0 + 0, 24, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
	gsSPEndDisplayList(),
};

Gfx mat_castle_grounds_dl_Material[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPSetLights1(castle_grounds_dl_Material_lights),
	gsSPEndDisplayList(),
};

Gfx castle_grounds_dl_Cube_mesh_layer_1[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(castle_grounds_dl_Cube_mesh_layer_1_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_castle_grounds_dl_Material),
	gsSPDisplayList(castle_grounds_dl_Cube_mesh_layer_1_tri_0),
	gsSPEndDisplayList(),
};

Gfx castle_grounds_dl_material_revert_render_settings[] = {
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};


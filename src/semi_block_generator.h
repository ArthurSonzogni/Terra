#ifndef SEMI_BLOCK_GENERATOR_E05ZI460
#define SEMI_BLOCK_GENERATOR_E05ZI460

extern float* semi_block_vertice[256];
extern int semi_block_n_vertice[256];
extern int* semi_block_face[256];
extern int* semi_block_face_id[256];
extern int semi_block_n_face[256];
extern int semi_block_face_id_get_opposite_id[256];
extern int semi_block_face_id_get_opposite_rel_x[256];
extern int semi_block_face_id_get_opposite_rel_y[256];
extern int semi_block_face_id_get_opposite_rel_z[256];
extern bool semi_block_valid[256];
extern int get_vertice[2][2][2];


#define VERTICE_xyz	1
#define VERTICE_xyZ 2
#define VERTICE_xYz	4
#define VERTICE_xYZ	8
#define VERTICE_Xyz	16
#define VERTICE_XyZ	32
#define VERTICE_XYz	64
#define VERTICE_XYZ	128


#endif /* end of include guard: SEMI_BLOCK_GENERATOR_E05ZI460 */

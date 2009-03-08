#include "AS3.h"
AS3_Val gg_lib = NULL;

#line 3 "libtcod_wrapper.gg"

#include <stdio.h>
#include <libtcod.h>

#define MASK_IS_VISIBLE					0x80
#define MASK_IS_SEEN						0x40
#define MASK_IS_TRANSPARENT			0x20
#define MASK_MOVEMENT_COST			0x1F
#define MOVEMENT_BLOCKED				0x1F

AS3_Val zero_param = NULL;

typedef struct _MapData
{
	unsigned char* bytes;
	TCOD_map_t tcod_map;
	int num_bytes;
	int width;
	int height;
} MapData;

typedef struct _CellValue
{
	bool is_visible;
	bool is_seen;
	bool is_transparent;
	bool is_walkable;
	int movement_cost;
} CellValue;

extern void sztrace(char* msg);
static char trace_str[1024];
void mytrace() { sztrace(trace_str); }

int md_index(MapData* map, int x, int y)
{
	return y * map->width + x;
}

void md_read_cell(MapData* map, int x, int y, CellValue* cell)
{
	unsigned char byte = map->bytes[md_index(map, x, y)];

	cell->is_seen = (byte & MASK_IS_SEEN) > 0 ? true : false;
	cell->is_visible = (byte & MASK_IS_VISIBLE) > 0 ? true : false;
	cell->is_transparent = (byte & MASK_IS_TRANSPARENT) > 0 ? true : false;
	cell->movement_cost = byte & MASK_MOVEMENT_COST;
	cell->is_walkable = cell->movement_cost == MOVEMENT_BLOCKED ? false : true;
}

void md_write_cell(MapData* map, int x, int y, CellValue* cell)
{
	unsigned char byte = 0;
	
	byte |= cell->is_visible == true ? MASK_IS_VISIBLE : 0;
	byte |= cell->is_seen == true ? MASK_IS_SEEN : 0;
	byte |= cell->is_transparent == true ? MASK_IS_TRANSPARENT : 0;
	byte |= cell->is_walkable == false
		? MOVEMENT_BLOCKED
		: cell->movement_cost & MASK_MOVEMENT_COST;

	map->bytes[md_index(map, x, y)] = byte;
}

MapData* _map_new(int width, int height)
{
	MapData* map = malloc(sizeof(MapData));

	map->width = width;
	map->height = height;

	map->num_bytes = map->width * map->height;
	map->bytes = malloc(map->num_bytes);

	map->tcod_map = TCOD_map_new(map->width, map->height);

	return map;
}

void _map_read_bytearray(MapData* map, AS3_Val byte_array)
{
	AS3_SetS(byte_array, "position", zero_param);
	AS3_ByteArray_readBytes(map->bytes, byte_array, map->num_bytes);

	int x;
	int y;
	CellValue cell;
	for (y = 0; y < map->height; y++) {
		for (x = 0; x < map->width; x++) {
			md_read_cell(map, x, y, &cell);
			TCOD_map_set_properties(map->tcod_map, x, y, cell.is_transparent, cell.is_walkable);
		}
	}
}

void _map_write_bytearray(MapData* map, AS3_Val byte_array)
{
	int x;
	int y;
	CellValue cell;
	bool visible;

	for (y = 0; y < map->height; y++) {
		for (x = 0; x < map->width; x++) {
			md_read_cell(map, x, y, &cell);
			visible = TCOD_map_is_in_fov(map->tcod_map, x, y);
			if (visible) {
				cell.is_visible = true;
				cell.is_seen = true;
			}
			else {
				cell.is_visible = false;
			}

			md_write_cell(map, x, y, &cell);
		}
	}

	AS3_SetS(byte_array, "position", zero_param);
	AS3_ByteArray_writeBytes(byte_array, map->bytes, map->num_bytes);	
}

void _map_clear_visible(MapData* map)
{
	int x;
	int y;
	CellValue cell;
	for (y = 0; y < map->height; y++) {
		for (x = 0; x < map->width; x++) {
			md_read_cell(map, x, y, &cell);
			cell.is_visible = false;
			md_write_cell(map, x, y, &cell);
		}
	}
}

void _map_clear_seen(MapData* map)
{
	int x;
	int y;
	CellValue cell;
	for (y = 0; y < map->height; y++) {
		for (x = 0; x < map->width; x++) {
			md_read_cell(map, x, y, &cell);
			cell.is_seen = false;
			md_write_cell(map, x, y, &cell);
		}
	}
}

void _map_delete(MapData* map)
{
	TCOD_map_delete(map->tcod_map);
	free(map->bytes);
	free(map);
}


#line 175 "libtcod_wrapper.gg"
static unsigned impl_map_new(int width, int height) {

#line 177 "libtcod_wrapper.gg"

	return _map_new(width, height);
}
static AS3_Val thunk_map_new(void *gg_clientData, AS3_Val gg_args) {
	int width;
	int height;
	AS3_ArrayValue(gg_args, "IntType, IntType",  &width,  &height);
	
#line 176 "libtcod_wrapper.gg"
return AS3_Int(
#line 176 "libtcod_wrapper.gg"
(MapData*)impl_map_new(
#line 175 "libtcod_wrapper.gg"
 width, 
#line 176 "libtcod_wrapper.gg"
 height));
}

#line 181 "libtcod_wrapper.gg"
static void impl_map_delete(MapData* map) {

#line 182 "libtcod_wrapper.gg"

	_map_delete(map);
}
static AS3_Val thunk_map_delete(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	AS3_ArrayValue(gg_args, "IntType",  &map);
	impl_map_delete(
#line 181 "libtcod_wrapper.gg"
(MapData*) map);
	return NULL;
}

#line 186 "libtcod_wrapper.gg"
static void impl_map_read_bytearray(MapData* map, AS3_Val byteArray) {

#line 188 "libtcod_wrapper.gg"

	_map_read_bytearray(map, byteArray);
}
static AS3_Val thunk_map_read_bytearray(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	AS3_Val byteArray;
	AS3_ArrayValue(gg_args, "IntType, AS3ValType",  &map,  &byteArray);
	impl_map_read_bytearray(
#line 186 "libtcod_wrapper.gg"
(MapData*) map, 
#line 187 "libtcod_wrapper.gg"
(AS3_Val) byteArray);
	return NULL;
}

#line 192 "libtcod_wrapper.gg"
static void impl_map_write_bytearray(MapData* map, AS3_Val byteArray) {

#line 194 "libtcod_wrapper.gg"

	_map_write_bytearray(map, byteArray);
}
static AS3_Val thunk_map_write_bytearray(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	AS3_Val byteArray;
	AS3_ArrayValue(gg_args, "IntType, AS3ValType",  &map,  &byteArray);
	impl_map_write_bytearray(
#line 192 "libtcod_wrapper.gg"
(MapData*) map, 
#line 193 "libtcod_wrapper.gg"
(AS3_Val) byteArray);
	return NULL;
}

#line 198 "libtcod_wrapper.gg"
static void impl_map_compute_fov(MapData* map, int x, int y, int max_radius, int light_walls, TCOD_fov_algorithm_t algo) {

#line 204 "libtcod_wrapper.gg"

	TCOD_map_compute_fov(map->tcod_map, x, y, max_radius, light_walls, algo);
}
static AS3_Val thunk_map_compute_fov(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	int x;
	int y;
	int max_radius;
	int light_walls;
	int algo;
	AS3_ArrayValue(gg_args, "IntType, IntType, IntType, IntType, IntType, IntType",  &map,  &x,  &y,  &max_radius,  &light_walls,  &algo);
	impl_map_compute_fov(
#line 198 "libtcod_wrapper.gg"
(MapData*) map, 
#line 199 "libtcod_wrapper.gg"
 x, 
#line 200 "libtcod_wrapper.gg"
 y, 
#line 201 "libtcod_wrapper.gg"
 max_radius, 
#line 202 "libtcod_wrapper.gg"
(int) light_walls, 
#line 203 "libtcod_wrapper.gg"
(TCOD_fov_algorithm_t) algo);
	return NULL;
}

#line 208 "libtcod_wrapper.gg"
static unsigned impl_map_get_tcod_map(MapData* map) {

#line 209 "libtcod_wrapper.gg"

	return map->tcod_map;
}
static AS3_Val thunk_map_get_tcod_map(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	AS3_ArrayValue(gg_args, "IntType",  &map);
	
#line 208 "libtcod_wrapper.gg"
return AS3_Int(
#line 208 "libtcod_wrapper.gg"
(TCOD_map_t)impl_map_get_tcod_map(
#line 208 "libtcod_wrapper.gg"
(MapData*) map));
}

#line 213 "libtcod_wrapper.gg"
static void impl_map_clear_visible(MapData* map) {

#line 214 "libtcod_wrapper.gg"

	_map_clear_visible(map);
}
static AS3_Val thunk_map_clear_visible(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	AS3_ArrayValue(gg_args, "IntType",  &map);
	impl_map_clear_visible(
#line 213 "libtcod_wrapper.gg"
(MapData*) map);
	return NULL;
}

#line 218 "libtcod_wrapper.gg"
static void impl_map_clear_seen(MapData* map) {

#line 219 "libtcod_wrapper.gg"

	_map_clear_seen(map);
}
static AS3_Val thunk_map_clear_seen(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	AS3_ArrayValue(gg_args, "IntType",  &map);
	impl_map_clear_seen(
#line 218 "libtcod_wrapper.gg"
(MapData*) map);
	return NULL;
}

#line 228 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_map_new(void *gg_clientData, AS3_Val gg_args) {
	int width;
	int height;
	AS3_ArrayValue(gg_args, "IntType, IntType",  &width,  &height);
	
#line 228 "libtcod_wrapper.gg"
return AS3_Int(
#line 228 "libtcod_wrapper.gg"
(TCOD_map_t)TCOD_map_new(
#line 228 "libtcod_wrapper.gg"
 width, 
#line 228 "libtcod_wrapper.gg"
 height));
}

#line 231 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_map_clear(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	AS3_ArrayValue(gg_args, "IntType",  &map);
	TCOD_map_clear(
#line 231 "libtcod_wrapper.gg"
(TCOD_map_t) map);
	return NULL;
}

#line 234 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_map_copy(void *gg_clientData, AS3_Val gg_args) {
	unsigned source;
	unsigned dest;
	AS3_ArrayValue(gg_args, "IntType, IntType",  &source,  &dest);
	TCOD_map_copy(
#line 234 "libtcod_wrapper.gg"
(TCOD_map_t) source, 
#line 234 "libtcod_wrapper.gg"
(TCOD_map_t) dest);
	return NULL;
}

#line 237 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_map_set_properties(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	int x;
	int y;
	int is_transparent;
	int is_walkable;
	AS3_ArrayValue(gg_args, "IntType, IntType, IntType, IntType, IntType",  &map,  &x,  &y,  &is_transparent,  &is_walkable);
	TCOD_map_set_properties(
#line 237 "libtcod_wrapper.gg"
(TCOD_map_t) map, 
#line 237 "libtcod_wrapper.gg"
 x, 
#line 237 "libtcod_wrapper.gg"
 y, 
#line 237 "libtcod_wrapper.gg"
(int) is_transparent, 
#line 237 "libtcod_wrapper.gg"
(int) is_walkable);
	return NULL;
}

#line 240 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_map_delete(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	AS3_ArrayValue(gg_args, "IntType",  &map);
	TCOD_map_delete(
#line 240 "libtcod_wrapper.gg"
(TCOD_map_t) map);
	return NULL;
}

#line 243 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_map_compute_fov(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	int player_x;
	int player_y;
	int max_radius;
	int light_walls;
	int algo;
	AS3_ArrayValue(gg_args, "IntType, IntType, IntType, IntType, IntType, IntType",  &map,  &player_x,  &player_y,  &max_radius,  &light_walls,  &algo);
	TCOD_map_compute_fov(
#line 243 "libtcod_wrapper.gg"
(TCOD_map_t) map, 
#line 243 "libtcod_wrapper.gg"
 player_x, 
#line 243 "libtcod_wrapper.gg"
 player_y, 
#line 243 "libtcod_wrapper.gg"
 max_radius, 
#line 243 "libtcod_wrapper.gg"
(int) light_walls, 
#line 243 "libtcod_wrapper.gg"
(TCOD_fov_algorithm_t) algo);
	return NULL;
}

#line 246 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_map_is_in_fov(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	int x;
	int y;
	AS3_ArrayValue(gg_args, "IntType, IntType, IntType",  &map,  &x,  &y);
	
#line 246 "libtcod_wrapper.gg"
return AS3_Int(
#line 246 "libtcod_wrapper.gg"
TCOD_map_is_in_fov(
#line 246 "libtcod_wrapper.gg"
(TCOD_map_t) map, 
#line 246 "libtcod_wrapper.gg"
 x, 
#line 246 "libtcod_wrapper.gg"
 y));
}

#line 249 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_map_is_transparent(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	int x;
	int y;
	AS3_ArrayValue(gg_args, "IntType, IntType, IntType",  &map,  &x,  &y);
	
#line 249 "libtcod_wrapper.gg"
return AS3_Int(
#line 249 "libtcod_wrapper.gg"
TCOD_map_is_transparent(
#line 249 "libtcod_wrapper.gg"
(TCOD_map_t) map, 
#line 249 "libtcod_wrapper.gg"
 x, 
#line 249 "libtcod_wrapper.gg"
 y));
}

#line 251 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_map_is_walkable(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	int x;
	int y;
	AS3_ArrayValue(gg_args, "IntType, IntType, IntType",  &map,  &x,  &y);
	
#line 251 "libtcod_wrapper.gg"
return AS3_Int(
#line 251 "libtcod_wrapper.gg"
TCOD_map_is_walkable(
#line 251 "libtcod_wrapper.gg"
(TCOD_map_t) map, 
#line 251 "libtcod_wrapper.gg"
 x, 
#line 251 "libtcod_wrapper.gg"
 y));
}

#line 253 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_map_get_width(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	AS3_ArrayValue(gg_args, "IntType",  &map);
	
#line 253 "libtcod_wrapper.gg"
return AS3_Int(
#line 253 "libtcod_wrapper.gg"
TCOD_map_get_width(
#line 253 "libtcod_wrapper.gg"
(TCOD_map_t) map));
}

#line 255 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_map_get_height(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	AS3_ArrayValue(gg_args, "IntType",  &map);
	
#line 255 "libtcod_wrapper.gg"
return AS3_Int(
#line 255 "libtcod_wrapper.gg"
TCOD_map_get_height(
#line 255 "libtcod_wrapper.gg"
(TCOD_map_t) map));
}

#line 257 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_map_get_nb_cells(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	AS3_ArrayValue(gg_args, "IntType",  &map);
	
#line 257 "libtcod_wrapper.gg"
return AS3_Int(
#line 257 "libtcod_wrapper.gg"
TCOD_map_get_nb_cells(
#line 257 "libtcod_wrapper.gg"
(TCOD_map_t) map));
}

#line 264 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_path_new_using_map(void *gg_clientData, AS3_Val gg_args) {
	unsigned map;
	double diagonalCost;
	AS3_ArrayValue(gg_args, "IntType, DoubleType",  &map,  &diagonalCost);
	
#line 265 "libtcod_wrapper.gg"
return AS3_Int(
#line 265 "libtcod_wrapper.gg"
(TCOD_path_t)TCOD_path_new_using_map(
#line 264 "libtcod_wrapper.gg"
(TCOD_map_t) map, 
#line 265 "libtcod_wrapper.gg"
 diagonalCost));
}

#line 268 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_path_new_using_function(void *gg_clientData, AS3_Val gg_args) {
	int map_width;
	int map_height;
	AS3_Val func;
	unsigned user_data;
	double diagonalCost;
	AS3_ArrayValue(gg_args, "IntType, IntType, AS3ValType, IntType, DoubleType",  &map_width,  &map_height,  &func,  &user_data,  &diagonalCost);
	
#line 272 "libtcod_wrapper.gg"
return AS3_Int(
#line 272 "libtcod_wrapper.gg"
(TCOD_path_t)TCOD_path_new_using_function(
#line 268 "libtcod_wrapper.gg"
 map_width, 
#line 269 "libtcod_wrapper.gg"
 map_height, 
#line 270 "libtcod_wrapper.gg"
 func, 
#line 271 "libtcod_wrapper.gg"
(void*) user_data, 
#line 272 "libtcod_wrapper.gg"
(float) diagonalCost));
}

#line 275 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_path_compute(void *gg_clientData, AS3_Val gg_args) {
	unsigned path;
	int ox;
	int oy;
	int dx;
	int dy;
	AS3_ArrayValue(gg_args, "IntType, IntType, IntType, IntType, IntType",  &path,  &ox,  &oy,  &dx,  &dy);
	
#line 277 "libtcod_wrapper.gg"
return AS3_Int(
#line 277 "libtcod_wrapper.gg"
TCOD_path_compute(
#line 275 "libtcod_wrapper.gg"
(TCOD_path_t) path, 
#line 276 "libtcod_wrapper.gg"
 ox, 
#line 276 "libtcod_wrapper.gg"
 oy, 
#line 277 "libtcod_wrapper.gg"
 dx, 
#line 277 "libtcod_wrapper.gg"
 dy));
}

#line 280 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_path_walk(void *gg_clientData, AS3_Val gg_args) {
	unsigned path;
	int x;
	int y;
	int recalculate_when_needed;
	AS3_ArrayValue(gg_args, "IntType, IntType, IntType, IntType",  &path,  &x,  &y,  &recalculate_when_needed);
	
#line 283 "libtcod_wrapper.gg"
return AS3_Int(
#line 283 "libtcod_wrapper.gg"
TCOD_path_walk(
#line 280 "libtcod_wrapper.gg"
(TCOD_path_t) path, 
#line 281 "libtcod_wrapper.gg"
(AS3_Val) x, 
#line 282 "libtcod_wrapper.gg"
(AS3_Val) y, 
#line 283 "libtcod_wrapper.gg"
(int) recalculate_when_needed));
}

#line 286 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_path_is_empty(void *gg_clientData, AS3_Val gg_args) {
	unsigned path;
	AS3_ArrayValue(gg_args, "IntType",  &path);
	
#line 286 "libtcod_wrapper.gg"
return AS3_Int(
#line 286 "libtcod_wrapper.gg"
TCOD_path_is_empty(
#line 286 "libtcod_wrapper.gg"
(TCOD_path_t) path));
}

#line 289 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_path_size(void *gg_clientData, AS3_Val gg_args) {
	unsigned path;
	AS3_ArrayValue(gg_args, "IntType",  &path);
	
#line 289 "libtcod_wrapper.gg"
return AS3_Int(
#line 289 "libtcod_wrapper.gg"
TCOD_path_size(
#line 289 "libtcod_wrapper.gg"
(TCOD_path_t) path));
}

#line 292 "libtcod_wrapper.gg"
static AS3_Val thunk_path_get(void *gg_clientData, AS3_Val gg_args) {
	unsigned path;
	int index;
	int x;
	int y;
	AS3_ArrayValue(gg_args, "IntType, IntType, IntType, IntType",  &path,  &index,  &x,  &y);
	TCOD_path_get(
#line 292 "libtcod_wrapper.gg"
(TCOD_path_t) path, 
#line 293 "libtcod_wrapper.gg"
 index, 
#line 294 "libtcod_wrapper.gg"
(AS3_Val) x, 
#line 295 "libtcod_wrapper.gg"
(AS3_Val) y);
	return NULL;
}

#line 298 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_path_get_origin(void *gg_clientData, AS3_Val gg_args) {
	unsigned path;
	int x;
	int y;
	AS3_ArrayValue(gg_args, "IntType, IntType, IntType",  &path,  &x,  &y);
	TCOD_path_get_origin(
#line 298 "libtcod_wrapper.gg"
(TCOD_path_t) path, 
#line 299 "libtcod_wrapper.gg"
(AS3_Val) x, 
#line 300 "libtcod_wrapper.gg"
(AS3_Val) y);
	return NULL;
}

#line 303 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_path_get_destination(void *gg_clientData, AS3_Val gg_args) {
	unsigned path;
	int x;
	int y;
	AS3_ArrayValue(gg_args, "IntType, IntType, IntType",  &path,  &x,  &y);
	TCOD_path_get_destination(
#line 303 "libtcod_wrapper.gg"
(TCOD_path_t) path, 
#line 304 "libtcod_wrapper.gg"
(AS3_Val) x, 
#line 305 "libtcod_wrapper.gg"
(AS3_Val) y);
	return NULL;
}

#line 308 "libtcod_wrapper.gg"
static AS3_Val thunk_tcod_path_delete(void *gg_clientData, AS3_Val gg_args) {
	unsigned path;
	AS3_ArrayValue(gg_args, "IntType",  &path);
	TCOD_path_delete(
#line 308 "libtcod_wrapper.gg"
(TCOD_path_t) path);
	return NULL;
}
AS3_Val gg_string(const char *str) {
	AS3_Val result = AS3_String(str);
	free((void *)str);
	return result;
}
void gg_reg(AS3_Val lib, const char *name, AS3_ThunkProc p) {
	AS3_Val fun = AS3_Function(NULL, p);
	AS3_SetS(lib, name, fun);
	AS3_Release(fun);
}
void gg_reg_async(AS3_Val lib, const char *name, AS3_ThunkProc p) {
	AS3_Val fun = AS3_FunctionAsync(NULL, p);
	AS3_SetS(lib, name, fun);
	AS3_Release(fun);
}
int main(int argc, char **argv) {
#if defined(GGINIT_DEFINED)
	ggInit();
#endif
	gg_lib = AS3_Object("");
	gg_reg(gg_lib, "map_new", thunk_map_new);
	gg_reg(gg_lib, "map_delete", thunk_map_delete);
	gg_reg(gg_lib, "map_read_bytearray", thunk_map_read_bytearray);
	gg_reg(gg_lib, "map_write_bytearray", thunk_map_write_bytearray);
	gg_reg(gg_lib, "map_compute_fov", thunk_map_compute_fov);
	gg_reg(gg_lib, "map_get_tcod_map", thunk_map_get_tcod_map);
	gg_reg(gg_lib, "map_clear_visible", thunk_map_clear_visible);
	gg_reg(gg_lib, "map_clear_seen", thunk_map_clear_seen);
	gg_reg(gg_lib, "tcod_map_new", thunk_tcod_map_new);
	gg_reg(gg_lib, "tcod_map_clear", thunk_tcod_map_clear);
	gg_reg(gg_lib, "tcod_map_copy", thunk_tcod_map_copy);
	gg_reg(gg_lib, "tcod_map_set_properties", thunk_tcod_map_set_properties);
	gg_reg(gg_lib, "tcod_map_delete", thunk_tcod_map_delete);
	gg_reg(gg_lib, "tcod_map_compute_fov", thunk_tcod_map_compute_fov);
	gg_reg(gg_lib, "tcod_map_is_in_fov", thunk_tcod_map_is_in_fov);
	gg_reg(gg_lib, "tcod_map_is_transparent", thunk_tcod_map_is_transparent);
	gg_reg(gg_lib, "tcod_map_is_walkable", thunk_tcod_map_is_walkable);
	gg_reg(gg_lib, "tcod_map_get_width", thunk_tcod_map_get_width);
	gg_reg(gg_lib, "tcod_map_get_height", thunk_tcod_map_get_height);
	gg_reg(gg_lib, "tcod_map_get_nb_cells", thunk_tcod_map_get_nb_cells);
	gg_reg(gg_lib, "tcod_path_new_using_map", thunk_tcod_path_new_using_map);
	gg_reg(gg_lib, "tcod_path_new_using_function", thunk_tcod_path_new_using_function);
	gg_reg(gg_lib, "tcod_path_compute", thunk_tcod_path_compute);
	gg_reg(gg_lib, "tcod_path_walk", thunk_tcod_path_walk);
	gg_reg(gg_lib, "tcod_path_is_empty", thunk_tcod_path_is_empty);
	gg_reg(gg_lib, "tcod_path_size", thunk_tcod_path_size);
	gg_reg(gg_lib, "path_get", thunk_path_get);
	gg_reg(gg_lib, "tcod_path_get_origin", thunk_tcod_path_get_origin);
	gg_reg(gg_lib, "tcod_path_get_destination", thunk_tcod_path_get_destination);
	gg_reg(gg_lib, "tcod_path_delete", thunk_tcod_path_delete);
	AS3_LibInit(gg_lib);
	return 1;
}

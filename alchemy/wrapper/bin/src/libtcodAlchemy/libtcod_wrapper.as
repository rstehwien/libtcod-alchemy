package libtcodAlchemy {
	public class libtcod_wrapper {
		import cmodule.libtcod_wrapper.CLibInit;
		protected static const _lib_init:cmodule.libtcod_wrapper.CLibInit = new cmodule.libtcod_wrapper.CLibInit();
		protected static const _lib:* = _lib_init.init();
		import flash.utils.ByteArray;
		static public function map_new(width:int, height:int):uint {
			return _lib.map_new(width, height);
		}
		static public function map_delete(map:uint):void {
			_lib.map_delete(map);
		}
		static public function map_read_bytearray(map:uint, byteArray:ByteArray):void {
			_lib.map_read_bytearray(map, byteArray);
		}
		static public function map_write_bytearray(map:uint, byteArray:ByteArray):void {
			_lib.map_write_bytearray(map, byteArray);
		}
		static public function map_compute_fov(map:uint, x:int, y:int, max_radius:int, light_walls:Boolean, algo:int):void {
			_lib.map_compute_fov(map, x, y, max_radius, light_walls, algo);
		}
		static public function map_get_tcod_map(map:uint):uint {
			return _lib.map_get_tcod_map(map);
		}
		static public function map_clear_visible(map:uint):void {
			_lib.map_clear_visible(map);
		}
		static public function map_clear_seen(map:uint):void {
			_lib.map_clear_seen(map);
		}
		static public function tcod_map_new(width:int, height:int):uint {
			return _lib.tcod_map_new(width, height);
		}
		static public function tcod_map_clear(map:uint):void {
			_lib.tcod_map_clear(map);
		}
		static public function tcod_map_copy(source:uint, dest:uint):void {
			_lib.tcod_map_copy(source, dest);
		}
		static public function tcod_map_set_properties(map:uint, x:int, y:int, is_transparent:Boolean, is_walkable:Boolean):void {
			_lib.tcod_map_set_properties(map, x, y, is_transparent, is_walkable);
		}
		static public function tcod_map_delete(map:uint):void {
			_lib.tcod_map_delete(map);
		}
		static public function tcod_map_compute_fov(map:uint, player_x:int, player_y:int, max_radius:int, light_walls:Boolean, algo:int):void {
			_lib.tcod_map_compute_fov(map, player_x, player_y, max_radius, light_walls, algo);
		}
		static public function tcod_map_is_in_fov(map:uint, x:int, y:int):int {
			return _lib.tcod_map_is_in_fov(map, x, y);
		}
		static public function tcod_map_is_transparent(map:uint, x:int, y:int):int {
			return _lib.tcod_map_is_transparent(map, x, y);
		}
		static public function tcod_map_is_walkable(map:uint, x:int, y:int):int {
			return _lib.tcod_map_is_walkable(map, x, y);
		}
		static public function tcod_map_get_width(map:uint):int {
			return _lib.tcod_map_get_width(map);
		}
		static public function tcod_map_get_height(map:uint):int {
			return _lib.tcod_map_get_height(map);
		}
		static public function tcod_map_get_nb_cells(map:uint):int {
			return _lib.tcod_map_get_nb_cells(map);
		}
		static public function tcod_path_new_using_map(map:uint, diagonalCost:Number):uint {
			return _lib.tcod_path_new_using_map(map, diagonalCost);
		}
		static public function tcod_path_new_using_function(map_width:int, map_height:int, func:Function, user_data:uint, diagonalCost:Number):uint {
			return _lib.tcod_path_new_using_function(map_width, map_height, func, user_data, diagonalCost);
		}
		static public function tcod_path_compute(path:uint, ox:int, oy:int, dx:int, dy:int):int {
			return _lib.tcod_path_compute(path, ox, oy, dx, dy);
		}
		static public function tcod_path_walk(path:uint, x:int, y:int, recalculate_when_needed:Boolean):int {
			return _lib.tcod_path_walk(path, x, y, recalculate_when_needed);
		}
		static public function tcod_path_is_empty(path:uint):int {
			return _lib.tcod_path_is_empty(path);
		}
		static public function tcod_path_size(path:uint):int {
			return _lib.tcod_path_size(path);
		}
		static public function path_get(path:uint, index:int, x:int, y:int):void {
			_lib.path_get(path, index, x, y);
		}
		static public function tcod_path_get_origin(path:uint, x:int, y:int):void {
			_lib.tcod_path_get_origin(path, x, y);
		}
		static public function tcod_path_get_destination(path:uint, x:int, y:int):void {
			_lib.tcod_path_get_destination(path, x, y);
		}
		static public function tcod_path_delete(path:uint):void {
			_lib.tcod_path_delete(path);
		}
	}
}

package libtcodAlchemy
{
		import __AS3__.vec.Vector;

		import flash.utils.ByteArray;
		import flash.utils.Endian;
		import flash.utils.getTimer;

		import libtcodAlchemy.libtcod_wrapper;

		/**
		* Stores blocking, transparency, and movement costs for each cell in a
		* map.  Each cell is stored as a Byte in a ByteArray with the following fields
		*
		*/
		public class Map
		{
				public static const FOV_BASIC:int = 0;
				public static const FOV_DIAMOND:int = 1;
				public static const FOV_SHADOW:int = 2;
				public static const FOV_PERMISSIVE_0:int = 3;
				public static const FOV_PERMISSIVE_1:int = 4;
				public static const FOV_PERMISSIVE_2:int = 5;
				public static const FOV_PERMISSIVE_3:int = 6;
				public static const FOV_PERMISSIVE_4:int = 7;
				public static const FOV_PERMISSIVE_5:int = 8;
				public static const FOV_PERMISSIVE_6:int = 9;
				public static const FOV_PERMISSIVE_7:int = 10;
				public static const FOV_PERMISSIVE_8:int = 11;

				private static const MASK_MAP_DATA:uint  		= 0xFF; // 1111 1111
				private static const MASK_IS_VISIBLE:uint		= 0x80; // 1000 0000
				private static const MASK_IS_SEEN:uint   		= 0x40; // 0100 0000
				private static const MASK_IS_TRANSPARENT:uint   = 0x20; // 0010 0000
				private static const MASK_MOVEMENT_COST:uint  	= 0x1F; // 0001 1111
				private static const MOVEMENT_BLOCKED:uint  	= 0x1F; // 0001 1111

				private var _data:ByteArray;
				private var _tcodMap:uint;
				private var _width:uint;
				private var _height:uint;
				private var _changed:Vector.<Boolean> = new Vector.<Boolean>();


				public function Map()
				{
				}

				public function dispose():void
				{
						if (_tcodMap != 0)
						{
								libtcod_wrapper.map_delete(_tcodMap);
								_tcodMap = 0;
						}
				}

				public function clone():Map
				{
						var ret:Map = new Map();
						ret.copy(this);
						return ret;
				}

				public function copy(map:Map):void
				{
						_width = map._width;
						_height = map._height;

						if (map._data)
						{
								if (_data)
								_data.clear();
								else
								_data = new ByteArray();

								_data.writeBytes(map._data);
								_tcodMap = libtcod_wrapper.map_new(width, height);
						}
						else
						{
								setSize(_width, _height, false);
						}
				}

				public function compare(map:Map):void
				{
						_changed = _compare(this._data, this._width, this._height, map ? map._data : null, map ? map._width : 0, map ? map._height : 0);
				}

				public function computeFov(x:int, y:int, maxRadius:int, lightWalls:Boolean, algorithm:int):void
				{
						libtcod_wrapper.map_read_bytearray(_tcodMap, _data);
						libtcod_wrapper.map_compute_fov(_tcodMap, x, y, maxRadius, lightWalls, algorithm);
						libtcod_wrapper.map_write_bytearray(_tcodMap, _data);
				}

				public function isChanged(x:int, y:int):Boolean
				{
						try
						{
								return _changed[index(width, x, y)] == true;
						}
						catch (e:Error)
						{
								return false;
						}
						return false;
				}

				public function get byteArray():ByteArray { return _data; }

				public function toString():String
				{
						var str:String = "";
						var i:int;
						var byte:uint;
						for (i = 0; i < _data.length; i++)
						{
								byte = _data[i];
								str = str + " " + byte;
						}
						return str;
				}

				public function get width():uint { return _width; }
				public function get height():uint { return _height; }

				public function setSize(newWidth:uint, newHeight:uint, copyOld:Boolean = true, defaultValue:uint = 0):void
				{
						var newData:ByteArray = new ByteArray();
						newData.length = newWidth * newHeight;

						if (_tcodMap != 0)
						libtcod_wrapper.map_delete(_tcodMap);

						var x:int;
						var y:int;

						for (y = 0; y < newHeight; y++)
						{
								for (x = 0; x < newWidth; x++)
								{
										if (copyOld && _data && x < _width && y < _height)
										{
												newData[index(newWidth, x, y)] = _data[index(_width, x, y)];
										}
										else
										{
												newData[index(newWidth, x, y)] = defaultValue;
										}
								}
						}

						_changed = _compare(newData, newWidth, newHeight, _data, _width, _height);

						_data = newData;
						_width = newWidth;
						_height = newHeight;
						_tcodMap = libtcod_wrapper.map_new(width, height);
				}

				public function clearVisible():void
				{
						// Faster to call the C code for large data-sets
						libtcod_wrapper.map_read_bytearray(_tcodMap, _data);
						libtcod_wrapper.map_clear_visible(_tcodMap);
						libtcod_wrapper.map_write_bytearray(_tcodMap, _data);
				}

				public function clearSeen():void
				{
						// Faster to call the C code for large data-sets
						libtcod_wrapper.map_read_bytearray(_tcodMap, _data);
						libtcod_wrapper.map_clear_seen(_tcodMap);
						libtcod_wrapper.map_write_bytearray(_tcodMap, _data);
				}

				public function getIsVisible(x:uint, y:uint):Boolean
				{
						return getMapCell(x, y).isVisible;
				}

				public function setIsVisible(x:uint, y:uint, value:Boolean):void
				{
						var cellValue:MapCell = getMapCell(x, y);
						cellValue.isVisible = value;
						setMapCell(x, y, cellValue);
				}

				public function getIsSeen(x:uint, y:uint):Boolean
				{
						return getMapCell(x, y).isSeen;
				}

				public function setIsSeen(x:uint, y:uint, value:Boolean):void
				{
						var cellValue:MapCell = getMapCell(x, y);
						cellValue.isSeen = value;
						setMapCell(x, y, cellValue);
				}

				public function getIsTransparent(x:uint, y:uint):Boolean
				{
						return getMapCell(x, y).isTransparent;
				}

				public function setIsTransparent(x:uint, y:uint, value:Boolean):void
				{
						var cellValue:MapCell = getMapCell(x, y);
						cellValue.isTransparent = value;
						setMapCell(x, y, cellValue);
				}

				public function getIsWalkable(x:uint, y:uint):Boolean
				{
						return getMapCell(x, y).isWalkable;
				}

				public function setIsWalkable(x:uint, y:uint, value:Boolean):void
				{
						var cellValue:MapCell = getMapCell(x, y);
						cellValue.isWalkable = value;
						setMapCell(x, y, cellValue);
				}

				public function getMovementCost(x:uint, y:uint):uint
				{
						return getMapCell(x, y).movementCost;
				}

				public function setMovementCost(x:uint, y:uint, value:uint):void
				{
						validateMovementCost(value);

						var cellValue:MapCell = getMapCell(x, y);
						cellValue.movementCost = value;
						setMapCell(x, y, cellValue);
				}

				private function validateLocation(x:uint, y:uint):void
				{
						if (_data == null || x >= width || y > height || index(width, x,y) > _data.length)
						throw new Error("(x,y) value invalid");
				}

				private function setMapCell(x:uint, y:uint, value:MapCell):void
				{
						validateLocation(x,y);
						_data[index(width, x, y)] = _createMapCell(value);
				}

				private static function index(width:int, x:uint, y:uint):uint
				{
						return y * width + x;
				}

				private static function validateMovementCost(cost:int):void
				{
						if (cost >= MOVEMENT_BLOCKED)
						throw new Error("Movement cost can be between 0-" + (MOVEMENT_BLOCKED - 1));
				}

				public static function createMapCell(isTransparent:Boolean, isWalkable:Boolean, movementCost:uint, visible:Boolean = false, seen:Boolean = false):uint
				{
						validateMovementCost(movementCost);

						var cellValue:MapCell = new MapCell();
						cellValue.isVisible = visible;
						cellValue.isSeen = seen;
						cellValue.isWalkable = isWalkable;
						cellValue.isTransparent = isTransparent;
						cellValue.movementCost = movementCost;
						return _createMapCell(cellValue);
				}

				private function getMapCell(x:uint, y:uint):MapCell
				{
						validateLocation(x,y);
						var byte:uint = _data[index(width, x, y)];

						var ret:MapCell = new MapCell();
						ret.isVisible = (byte & MASK_IS_VISIBLE) > 0;
						ret.isSeen = (byte & MASK_IS_SEEN) > 0;
						ret.isTransparent = (byte & MASK_IS_TRANSPARENT) > 0;
						ret.movementCost = byte & MASK_MOVEMENT_COST;
						ret.isWalkable = ret.movementCost != MOVEMENT_BLOCKED;

						return ret;
				}

				private static function _createMapCell(value:MapCell):uint
				{
						var byte:uint = 0;

						byte |= value.isVisible ? MASK_IS_VISIBLE : 0;
						byte |= value.isSeen ? MASK_IS_SEEN : 0;
						byte |= value.isTransparent ? MASK_IS_TRANSPARENT : 0;
						byte |= !value.isWalkable ? MOVEMENT_BLOCKED : value.movementCost & MASK_MOVEMENT_COST;

						return byte;
				}

				private static function _compare(data1:ByteArray, width1:uint, height1:uint, data2:ByteArray, width2:uint, height2:uint):Vector.<Boolean>
				{
						// TODO might be faster to do this in C
						var x:int;
						var y:int;

						var changed:Vector.<Boolean> = new Vector.<Boolean>(width1*height1, true);

						for (y = 0; y < height1; y++)
						{
								for (x = 0; x < width1; x++)
								{
										var idx:uint = index(width1, x, y);

										if (data2 && x < width2 && y < height2)
										{
												changed[idx] = (data1[index(width1, x, y)] - data2[index(width2, x, y)]) != 0;
										}
										else
										{
												changed[idx] = true;
										}
								}
						}

						return changed;
				}

				private function traceTime(start:int, msg:String):int
				{
						var end:int = flash.utils.getTimer();
						trace("Map: " + msg + " " + (end - start) + " ms for width " + width + " height " + height);
						return end;
				}
		}
}

class MapCell
{
		public var isVisible:Boolean;
		public var isSeen:Boolean;
		public var isTransparent:Boolean;
		public var isWalkable:Boolean;
		public var movementCost:uint;
}

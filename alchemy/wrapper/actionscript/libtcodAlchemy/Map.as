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

				private var _data:ByteArray;
				private var _tcodMap:uint;
				private var _width:uint;
				private var _height:uint;

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
								{
										_data.clear();
								}
								else
								{
										_data = new ByteArray();
								}
								
								_data.writeBytes(map._data);
								_tcodMap = libtcod_wrapper.map_new(width, height);
						}
						else
						{
								setSize(_width, _height, false);
						}
				}

				public function get width():uint { return _width; }
				public function get height():uint { return _height; }

				public function setSize(newWidth:uint, newHeight:uint, copyOld:Boolean = true, defaultValue:MapCell = null):void
				{
						if (defaultValue == null)
						{
								defaultValue = new MapCell();
						}
						var defaultByte:uint = defaultValue.byte;

						var newData:ByteArray = new ByteArray();
						newData.length = newWidth * newHeight;

						if (_tcodMap != 0)
						{
								libtcod_wrapper.map_delete(_tcodMap);
						}

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
												newData[index(newWidth, x, y)] = defaultByte;
										}
								}
						}

						_data = newData;
						_width = newWidth;
						_height = newHeight;
						_tcodMap = libtcod_wrapper.map_new(width, height);
				}

				public function computeFov(x:int, y:int, maxRadius:int, lightWalls:Boolean, algorithm:int):void
				{
						libtcod_wrapper.map_read_bytearray(_tcodMap, _data);
						libtcod_wrapper.map_compute_fov(_tcodMap, x, y, maxRadius, lightWalls, algorithm);
						libtcod_wrapper.map_write_bytearray(_tcodMap, _data);
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

				public function getMapCell(x:uint, y:uint):MapCell
				{
						validateLocation(x,y);
						var byte:uint = _data[index(width, x, y)];

						var ret:MapCell = new MapCell();
						ret.byte = byte;
						ret.x = x;
						ret.y = y;

						return ret;
				}

				public function setMapCell(value:MapCell):void
				{
						validateLocation(value.x, value.y);
						_data[index(width, value.x, value.y)] = value.byte;
				}

				public function getIsVisible(x:uint, y:uint):Boolean
				{
						return getMapCell(x, y).isVisible;
				}

				public function setIsVisible(x:uint, y:uint, value:Boolean):void
				{
						var cellValue:MapCell = getMapCell(x, y);
						cellValue.isVisible = value;
						setMapCell(cellValue);
				}

				public function getIsSeen(x:uint, y:uint):Boolean
				{
						return getMapCell(x, y).isSeen;
				}

				public function setIsSeen(x:uint, y:uint, value:Boolean):void
				{
						var cellValue:MapCell = getMapCell(x, y);
						cellValue.isSeen = value;
						setMapCell(cellValue);
				}

				public function getIsTransparent(x:uint, y:uint):Boolean
				{
						return getMapCell(x, y).isTransparent;
				}

				public function setIsTransparent(x:uint, y:uint, value:Boolean):void
				{
						var cellValue:MapCell = getMapCell(x, y);
						cellValue.isTransparent = value;
						setMapCell(cellValue);
				}

				public function getIsWalkable(x:uint, y:uint):Boolean
				{
						return getMapCell(x, y).isWalkable;
				}

				public function setIsWalkable(x:uint, y:uint, value:Boolean):void
				{
						var cellValue:MapCell = getMapCell(x, y);
						cellValue.isWalkable = value;
						setMapCell(cellValue);
				}

				public function getMovementCost(x:uint, y:uint):uint
				{
						return getMapCell(x, y).movementCost;
				}

				public function setMovementCost(x:uint, y:uint, value:uint):void
				{
						var cellValue:MapCell = getMapCell(x, y);
						cellValue.movementCost = value;
						setMapCell(cellValue);
				}

				public function changedCells(from:Map):Vector.<MapCell>
				{
						var vector:Vector.<MapCell> = new Vector.<MapCell>();

						var x:int;
						var y:int;

						var changed:Boolean;
						var current:MapCell;

						for (y = 0; y < height; y++)
						{
								for (x = 0; x < width; x++)
								{
										current = getMapCell(x, y);

										if (from && from._data && x < from.width && y < from.height)
												changed = (current.byte - from._data[index(from.width, x, y)]) != 0;
										else
												changed = true;

										if (changed)
										{
												vector.push(current);
										}
								}
						}

						return vector;
				}

				private function validateLocation(x:uint, y:uint):void
				{
						if (_data == null || x >= width || y > height || index(width, x,y) > _data.length)
						{
								throw new Error("(x,y) value invalid");
						}
				}

				private static function index(width:int, x:uint, y:uint):uint
				{
						return y * width + x;
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

				private function traceTime(start:int, msg:String):int
				{
						var end:int = flash.utils.getTimer();
						trace("Map: " + msg + " " + (end - start) + " ms for width " + width + " height " + height);
						return end;
				}
		}
}

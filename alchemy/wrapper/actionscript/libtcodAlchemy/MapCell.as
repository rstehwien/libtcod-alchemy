package libtcodAlchemy
{
	public class MapCell
	{
		public function MapCell(
			isTransparent:Boolean = true,
			isWalkable:Boolean = true, 
			movementCost:uint = 1,
			x:uint = 0,
			y:uint = 0,
			isVisible:Boolean = false,
			isSeen:Boolean = false)
		{
			this.isTransparent = isTransparent;
			this.isWalkable = isWalkable;
			this.movementCost = movementCost;
			this.isVisible = isVisible;
			this.isSeen = isSeen;
			this.x = x;
			this.y = y;
		}

		public var x:uint = 0;
		public var y:uint = 0;
		public var isVisible:Boolean = false;
		public var isSeen:Boolean = false;
		public var isTransparent:Boolean = true;
		public var isWalkable:Boolean = true;

		private var _movementCost:uint = 1;
		public function get movementCost():uint { return _movementCost; }
		public function set movementCost(value:uint):void
		{
			if (value >= MOVEMENT_BLOCKED)
			{
				throw new Error("Movement cost can be between 0-" + (MOVEMENT_BLOCKED - 1));
			}
			_movementCost = value;
		}

		internal function set byte(value:uint):void
		{
			isVisible = (value & MASK_IS_VISIBLE) > 0;
			isSeen = (value & MASK_IS_SEEN) > 0;
			isTransparent = (value & MASK_IS_TRANSPARENT) > 0;
			_movementCost = value & MASK_MOVEMENT_COST;
			isWalkable = _movementCost != MOVEMENT_BLOCKED;
			if (!isWalkable) _movementCost = MOVEMENT_BLOCKED - 1;
		}

		internal function get byte():uint
		{
			var byte:uint = 0;

			byte |= isVisible ? MASK_IS_VISIBLE : 0;
			byte |= isSeen ? MASK_IS_SEEN : 0;
			byte |= isTransparent ? MASK_IS_TRANSPARENT : 0;
			byte |= !isWalkable ? MOVEMENT_BLOCKED : movementCost & MASK_MOVEMENT_COST;

			return byte;
		}

		private static const MASK_MAP_DATA:uint       = 0xFF; // 1111 1111
		private static const MASK_IS_VISIBLE:uint     = 0x80; // 1000 0000
		private static const MASK_IS_SEEN:uint        = 0x40; // 0100 0000
		private static const MASK_IS_TRANSPARENT:uint = 0x20; // 0010 0000
		private static const MASK_MOVEMENT_COST:uint  = 0x1F; // 0001 1111
		private static const MOVEMENT_BLOCKED:uint    = 0x1F; // 0001 1111
	}
}
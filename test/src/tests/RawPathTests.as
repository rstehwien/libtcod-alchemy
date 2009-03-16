package tests
{
	import flexunit.framework.TestCase;
	
	import libtcodAlchemy.*;

	public class RawPathTests extends TestCase
	{
		private var _map:Map;
		private var _tcodPath:uint;
		
		public function RawPathTests(methodName:String=null)
		{
			super(methodName);
		}
		
		override public function setUp():void
		{
			_map = new Map(5, 5);
			var tcodMap:uint = libtcod_wrapper.map_get_tcod_map(_map.alchemyMap);
			_tcodPath = libtcod_wrapper.tcod_path_new_using_map(tcodMap, 1);
		}
		
		override public function tearDown():void
		{
			libtcod_wrapper.tcod_path_delete(_tcodPath);
			_map.dispose();
		}
		
		public function testNewDeletePath():void
		{
			var tcodMap:uint = libtcod_wrapper.map_get_tcod_map(_map.alchemyMap);
			var path:uint = libtcod_wrapper.tcod_path_new_using_map(tcodMap, 1);
			assertTrue(path != 0);
			libtcod_wrapper.tcod_path_delete(path);
		}

		public function test_tcod_path_compute():void
		{
			assertTrue(libtcod_wrapper.tcod_path_compute(_tcodPath, 0, 0, 4, 0));
		}

/*
		public function test_tcod_path_walk():void
		{
			assertTrue(libtcod_wrapper.tcod_path_compute(_tcodPath, 0, 0, 4, 0));
		}

		public function test_tcod_path_is_empty():void
		{
			//fail("TODO write test");
		}

		public function test_tcod_path_size():void
		{
			//fail("TODO write test");
		}

		public function test_path_get():void
		{
			//fail("TODO write test");
		}

		public function test_tcod_path_get_origin():void
		{
			//fail("TODO write test");
		}

		public function test_tcod_path_get_destination():void
		{
			//fail("TODO write test");
		}

		public function test_tcod_path_delete():void
		{
			//fail("TODO write test");
		}
*/
	}
}
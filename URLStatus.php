<?php
/**
 * Class for checking URL status(viewd or not). With using bitstore extension.
 */
class URLStatus {
	private static $memID = 0;
	
	/**
	 * Pass array of urls, and return array of unchecked urls
	 * @param array $urls 
	 */
	public static function unchecked(array $urls)
	{
		$urls = array_reduce($urls, function($r, $v) {
					$address = '0x'.substr(murmurhash3($v, 4), 0, 8);
					if(!bs_get(\URLStatus::getMemID(), $address)) {
						$r[] = $v;
					}
					return $r;
				});
		return $urls;
	}
	
	/**
	 * Test URL status
	 * @param type $url 
	 */
	public static function test($url)
	{
		$address = '0x'.substr(murmurhash3($url, 4), 0, 8);
		return bs_get(self::$memID, $address);
	}
	
	/**
	 * Set URL status as checked
	 * @param type $url 
	 */
	public static function check($url)
	{
		$address = '0x'.substr(murmurhash3($url, 4), 0, 8);
		
		/*
		$maxbit = bs_max(self::$memID);
		echo "Max bit: ".$maxbit."\n";
		echo "Address: ".$address."\n";
		echo "Memory ID: ".self::$memID."\n";
		*/

		$bit = bs_switch(self::$memID, $address);
		
		return $bit;
	}
	
	public static function setMemID($id)
	{
		self::$memID = $id;
	}
	
	public static function getMemID()
	{
		return self::$memID;
	}
}
?>

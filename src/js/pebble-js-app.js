// URL to your configuration page
var config_url = "http://wearewearable.com/pconfig/?v=1.0";

Pebble.addEventListener("ready", function(e) {
	
});
	
Pebble.addEventListener("showConfiguration", function(_event) {
	var url = config_url;
	
	for(var i = 0, x = window.localStorage.length; i < x; i++) {
		var key = window.localStorage.key(i);
		var val = window.localStorage.getItem(key);
		
		if(val != null) {
			url += "&" + encodeURIComponent(key) + "=" + encodeURIComponent(val);
		}
	}
	
	console.log(url);
	
	Pebble.openURL(url);
});

Pebble.addEventListener("webviewclosed", function(_event) {
	if(_event.response) {
		var values = JSON.parse(decodeURIComponent(_event.response));
		
		for(key in values) {
			window.localStorage.setItem(key, values[key]);
		}
		
		Pebble.sendAppMessage(values,
			function(_event) {
				console.log("Successfully sent options to Pebble");
			},
			function(_event) {
				console.log("Failed to send options to Pebble.\nError: " + _event.error.message);
			}
		);
	}
});
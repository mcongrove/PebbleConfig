## PConfig

PConfig is a tool for simplifying the creation of and communication with a configuration web page via PebbleKit JS.

This is done by automatically retrieving the values of every HTML form field on a web page, sending that data back to a PebbleKit JS script, handling the data and passing it along to the message inbox on the watch. PConfig also automatically saves all form field values to local storage in PebbleKit JS, sends the values to the configuration web page, and sets all fields to their corresponding values.

In case you missed that...

### Automagic

The following tasks are done automatically by PConfig:

 1. Appends all cached settings as a GET query to your configuration web page URL
 2. Sets the value for all form fields on the configuration web page with those values
 3. Handles the cancel and submit events on your form
 4. Retrieves the value of all form fields on the web page
 	* Also, optionally typecasts the values
 5. Sends settings to PebbleKit JS
 6. Saves all settings to local storage (for items #1 and #2)
 7. Passes data along from PebbleKit JS to your app message inbox

### Example

Sample code has been provided in this repository. Please review the files and refer to the integration steps below for further details.

### Integration Steps

###### Configuration Page

The configuration page must include HTML form fields (supports `input`, `select` and `textarea`) with IDs that match the `appKeys` in your `appinfo.json` file; there's no need for a `form` element. You can optionally add a `data-type` (`string` or `int`) to each field to typecast the returned values.

	<select id="theme" data-type="int">
		<option value="0">Dark</option>
		<option value="1">Light</option>
	</select>

You must also include the PConfig Javascript file and initialize it at the bottom of your page.

		...
		
		<script src="pconfig.js" type="text/javascript"></script>
		<script type="text/javascript">
			PConfig.init();
		</script>
	</body>
	</html>

Finally, assign event listeners to your submit and cancel buttons.

	document.getElementById("cancel").addEventListener("click", PConfig.onCancel);
	document.getElementById("save").addEventListener("click", PConfig.onSubmit);

###### PebbleKit JS

Copy the PebbleKit Javascript code (`pebble-js-app.js`) into your project exactly as provided; you only need to change the URL for the configuration web page at the top of the file.

###### App Info

In your `appinfo.json` file, ensure that your `appKeys` match the form field IDs. In the example earlier we had a `select` box with an ID of `theme`, so we'd do the following:

	"appKeys": {
		"theme": 0,
		"foo": 1,
		"bar": 2
	}

###### Watch Code

Retrieve the form fields in your C code as you would normally through the application message inbox. Following our `theme` example, we'd do this:

	enum {
		KEY_THEME = 0x0
	};
	
	static void in_received_handler(DictionaryIterator *iter, void *context) {
		Tuple *theme_tuple = dict_find(iter, KEY_THEME);
		theme_tuple ? persist_write_int(KEY_THEME, theme_tuple->value->uint8) : false;
	}

### Issues

This is a first-pass at this library, and was built specifically to suit the needs of the watch faces I've developed. It will improve in time, but there may be bugs, and there are certainly more features that can be added.

If you have bugs to report or features to request, please use the GitHub Issues on this page.

### License

This code is open sourced under the Apache 2.0 license.
Python plugins
--------------
Bourgeon currently offers two python modules to interact with the plugin system
and the Ragnarok client.  
These are respectively `bourgeon` and `ragnarok_client`.
To use them it's a simple as importing them in your script:
```python
import bourgeon
import ragnarok_client
```

### bourgeon
## Available functions
* **log(message : str):** Print *message* in Bourgeon's console, useful for debug or
logging purposes
* **register_callback(callback_name : str, function : func):** Register a
 *function* to be called when a given event (*callback_name*) happens
* **unregister_callback(callback_name : str, function : func):** Unregister a
function.

## Callbacks
At the moment the following callbacks are available:  
* `OnTick`: Executed every 100ms  
Arguments: *None*
* `OnKeyDown`: Executed every time a key is pressed inside the client's window.  
Arguments: 1: *virtual_key* : unsigned int, 2: *new_key_down* : int,
3: *accurate_key* : int
* `OnChatMessage`: Executed every time a chat message is received from the
server.  
Arguments: 1: *message* : str
* `OnTalkType`: Executed every time a talk type (*i.e.* an client command
starting with '/') is resolved on the client.  
Arguments: 1: *chat_buffer* : str

### ragnarok_client
* **print_in_chat(message : str, color : int, filter : int):** Print *message*
into the client's game chat.
* **get_hp():** Return the number of HP of the current character.
* **get_max_hp():** Return the number of Max HP of the current character.
* **get_sp():** Return the number of SP of the current character.
* **get_max_sp():** Return the number of Max SP of the current character.
* **use_item(item_id : int):** Use an item. Return **true** if the item was used
successfully, **false** otherwise.

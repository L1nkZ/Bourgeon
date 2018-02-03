# Plugin settings --------------------------------------------------------------
HP_MIN_PERCENT = 0.9
SP_MIN_PERCENT = 0.8
HP_ITEM = 545 # Red slim potion
SP_ITEM = 505 # Blue potion

# Plugin code ------------------------------------------------------------------
import bourgeon
import ragnarok_client as client

LOGIN_MODE = 0
GAME_MODE = 1

current_mode = LOGIN_MODE
ap_activated = False

def on_mode_switch(mode_type, map_name):
	global current_mode

	current_mode = mode_type


def on_tick():
	if current_mode == GAME_MODE and ap_activated:
		hp_percent = client.get_hp() / client.get_max_hp()
		sp_percent = client.get_sp() / client.get_max_sp()

		if hp_percent < HP_MIN_PERCENT:
			client.use_item(HP_ITEM)
		elif sp_percent < SP_MIN_PERCENT:
			client.use_item(SP_ITEM)


def show_ap_settings():
		client.print_in_chat("Autopot Settings", 0xFF00, 0)
		client.print_in_chat("Status: %s" % \
			("Activated" if ap_activated else "Deactivated"), 0xFFFFFF, 0)
		client.print_in_chat("HP: %d%%" % int(HP_MIN_PERCENT * 100), 0xFFFFFF, 0)
		client.print_in_chat("SP: %d%%" % int(SP_MIN_PERCENT * 100), 0xFFFFFF, 0)
		client.print_in_chat("HP item: %s" % HP_ITEM, 0xFF, 0)
		client.print_in_chat("SP item: %s" % SP_ITEM, 0xFF0000, 0)


def on_command(chat_buffer):
	global ap_activated

	if chat_buffer.find("/ap_settings") == 0:
		show_ap_settings()
	elif chat_buffer.find("/ap_toggle") == 0:
		ap_activated = not ap_activated
		if ap_activated:
			client.print_in_chat("Autpot activated", 0xFFFFFF, 0)
		else:
			client.print_in_chat("Autpot deactivated", 0xFFFFFF, 0)


bourgeon.log("-= Autopot plugin =-")
bourgeon.register_callback("OnTick", on_tick)
bourgeon.register_callback("OnModeSwitch", on_mode_switch)
bourgeon.register_callback("OnTalkType", on_command)

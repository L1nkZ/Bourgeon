# Plugin settings --------------------------------------------------------------
HP_MIN_PERCENT = 0.9
SP_MIN_PERCENT = 0.8
RED_SLIM_POTION = 545
BLUE_POTION = 505

# Plugin code ------------------------------------------------------------------
import bourgeon
import ragnarok_client as client

in_game = False

def on_tick():
	if in_game:
		hp_percent = client.get_hp() / client.get_max_hp()
		sp_percent = client.get_sp() / client.get_max_sp()

		if hp_percent < HP_MIN_PERCENT:
			if client.use_item(RED_SLIM_POTION):
				client.print_in_chat("Using a HP potion", 0xFFFFFF, 0)
		elif sp_percent < SP_MIN_PERCENT:
			if client.use_item(BLUE_POTION):
				client.print_in_chat("Using a SP potion", 0xFFFFFF, 0)


def on_chat_message(message):
	global in_game

	if not in_game:
		in_game = True
	bourgeon.log(message)


bourgeon.log("-= Autopot plugin =-")
bourgeon.register_callback("OnChatMessage", on_chat_message)
bourgeon.register_callback("Tick", on_tick)

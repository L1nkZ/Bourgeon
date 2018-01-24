import bourgeon
import ragnarok_client as client

in_game = False
s = 1

def on_tick():
	global s
	global in_game

	if s == 10:
		if in_game:
			bourgeon.log("HP: %d/%d" % (client.get_hp(), client.get_max_hp()))
			bourgeon.log("SP: %d/%d" % (client.get_sp(), client.get_max_sp()))
		s = 1
	s += 1


def on_chat_message(message):
	global in_game

	if not in_game:
		in_game = True
	bourgeon.log(message)


bourgeon.log("-= Test plugi n=-")
bourgeon.register_callback("OnChatMessage", on_chat_message)
bourgeon.register_callback("Tick", on_tick)

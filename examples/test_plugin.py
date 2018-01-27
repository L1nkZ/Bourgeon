# Plugin code ------------------------------------------------------------------
import bourgeon
import ragnarok_client as client

def on_tick():
	bourgeon.log("Tick")

def on_chat_message(message):
	bourgeon.log(message)


bourgeon.log("-= Test plugin =-")
bourgeon.register_callback("OnChatMessage", on_chat_message)
bourgeon.register_callback("Tick", on_tick)

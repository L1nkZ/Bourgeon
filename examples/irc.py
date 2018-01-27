# Plugin settings --------------------------------------------------------------
SERVER = "irc.rizon.net"
NICKNAME = "ro_client"
CHANNEL = "#Hercules"

# Plugin code ------------------------------------------------------------------
# This plugin requires the irc library
# To install it, it's as simple as: pip install irc
import bourgeon
import ragnarok_client as ro_client
import irc.client
import atexit

in_game = False

# Objects' initialization
bourgeon.log("-= IRC plugin =-")
irc_client = irc.client.Reactor()
server = irc_client.server()

# Called every 100ms
def on_tick():
    global irc_client
    irc_client.process_once()


# Called whenever we arrive on the map server
def on_enter_ingame():
    global server

    server.join(CHANNEL)
    ro_client.print_in_chat("IRC Plugin: Now listening in %s" % CHANNEL, 0xFFFFFF, 0)

# Called whenever a public irc message is received
def on_irc_message(server, event):
    username = event.source.nick
    message = event.arguments[0]
    channel = event.target
    ingame_notification = "(%s) %s: %s" % (channel, username, message)

    ro_client.print_in_chat(ingame_notification, 0xFFFFFF, 0)


# Called whenever an in-game chat message is received
def on_chat_message(message):
    global in_game

    if not in_game:
        in_game = True
        on_enter_ingame()


# Called when exiting
def on_exit():
    server.disconnect()


# Setup the connection, register the callbacks
server.connect(SERVER, 6667, NICKNAME)
server.add_global_handler("pubmsg", on_irc_message)
bourgeon.register_callback("OnChatMessage", on_chat_message)
bourgeon.register_callback("Tick", on_tick)
atexit.register(on_exit)

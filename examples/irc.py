# Plugin settings --------------------------------------------------------------
# Server to join
SERVER = "irc.rizon.net"
# Nickname on the server
NICKNAME = "ro_client"
# Channels to join
CHANNELS = ["#MyChannel"]

# Note: To talk on irc from the game chat, you must type:
# /irc #channel Your message here

# Plugin code ------------------------------------------------------------------
# This plugin requires the irc library
# To install it, it's as simple as: pip install irc
import bourgeon
import ragnarok_client as ro_client
import irc.client
import atexit

# Colors are of the form 0xBBGGRR
COLOR_INFO = 0x00FF00
COLOR_ERROR = 0xFF
COLOR_IRC_MSG = 0x893D89

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

    ro_client.print_in_chat("IRC Plugin:", COLOR_INFO, 0)
    for channel in CHANNELS:
        server.join(channel)
        ro_client.print_in_chat("Joined %s" % channel, COLOR_INFO, 0)

# Called whenever a public irc message is received
def on_irc_message(server, event):
    username = event.source.nick
    message = event.arguments[0]
    channel = event.target
    ingame_notification = "(%s) %s: %s" % (channel, username, message)

    ro_client.print_in_chat(ingame_notification, COLOR_IRC_MSG, 0)


# Called whenever an in-game chat message is received
def on_chat_message(message):
    global in_game

    if not in_game:
        in_game = True
        on_enter_ingame()


def on_talktype(chat_buffer):
    if chat_buffer.find("/irc") == 0:
        parts = chat_buffer.split(' ', 2)

        if len(parts) != 3:
            ro_client.print_in_chat("Invalid arguments.", COLOR_ERROR, 0)
            return
        elif parts[1] not in CHANNELS:
            ro_client.print_in_chat("Incorrect channel.", COLOR_ERROR, 0)
            return

        server.privmsg(parts[1], parts[2])
        ingame_notification = "(%s) %s: %s" % (parts[1].lower(), NICKNAME, parts[2])
        ro_client.print_in_chat(ingame_notification, COLOR_IRC_MSG, 0)


# Called when exiting
def on_exit():
    server.disconnect()


# Setup the connection, register the callbacks
server.connect(SERVER, 6667, NICKNAME)
server.add_global_handler("pubmsg", on_irc_message)
bourgeon.register_callback("OnChatMessage", on_chat_message)
bourgeon.register_callback("OnTalkType", on_talktype)
bourgeon.register_callback("OnTick", on_tick)
atexit.register(on_exit)

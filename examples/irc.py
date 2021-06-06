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
from ragnarok_client import Mode

# Colors are of the form 0xBBGGRR
COLOR_INFO = 0x00FF00
COLOR_ERROR = 0xFF
COLOR_IRC_MSG = 0x893D89

current_mode = Mode.Login
joined_channels = False

# Objects' initialization
bourgeon.log("-= IRC plugin =-")
irc_client = irc.client.Reactor()
server = irc_client.server()


# Called every 100ms
def on_tick():
    global irc_client
    irc_client.process_once()


# Called whenever we arrive on the map server ofr the first time
def join_channels():
    global joined_channels

    bourgeon.log("IRC Plugin:")
    for channel in CHANNELS:
        server.join(channel)
        bourgeon.log("  > Joined %s" % channel)
    joined_channels = True


def on_mode_switch(mode_type, map_name):
    global current_mode

    current_mode = mode_type
    if not joined_channels:
        join_channels()


# Called whenever a public irc message is received
def on_irc_message(server, event):
    if current_mode == Mode.Game:
        username = event.source.nick
        message = event.arguments[0]
        channel = event.target
        ingame_notification = "(%s) %s: %s" % (channel, username, message)

        ro_client.print_in_chat(ingame_notification, COLOR_IRC_MSG, 0)


def on_talktype(chat_buffer):
    if current_mode == Mode.Game:
        if chat_buffer.find("/irc ") == 0:
            parts = chat_buffer.split(' ', 2)

            if len(parts) != 3:
                ro_client.print_in_chat("Invalid arguments.", COLOR_ERROR, 0)
                return
            elif parts[1] not in CHANNELS:
                ro_client.print_in_chat("Incorrect channel.", COLOR_ERROR, 0)
                return

            server.privmsg(parts[1], parts[2])
            ingame_notification = "(%s) %s: %s" % (parts[1].lower(), NICKNAME,\
                                                   parts[2])
            ro_client.print_in_chat(ingame_notification, COLOR_IRC_MSG, 0)


# Called when exiting
def on_exit():
    server.disconnect()


# Setup the connection, register the callbacks
server.connect(SERVER, 6667, NICKNAME)
server.add_global_handler("pubmsg", on_irc_message)
atexit.register(on_exit)

bourgeon.register_callback("OnTalkType", on_talktype)
bourgeon.register_callback("OnTick", on_tick)
bourgeon.register_callback("OnModeSwitch", on_mode_switch)

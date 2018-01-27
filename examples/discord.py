# Plugin settings --------------------------------------------------------------
# Local storage token
token = "<Your token here>"
# Server to join
SERVER = "rAthena"
#  Channels to follow
CHANNELS = ["off-topic"]

# Plugin code ------------------------------------------------------------------
import bourgeon
import ragnarok_client as ro_client
import requests

class DiscordAuth(requests.auth.AuthBase):

    def __init__(self, access_token):
        self.access_token = access_token

    def __call__(self, r):
        r.headers['Authorization'] = self.access_token
        return r


API_ENDPOINT = "https://discordapp.com/api"
auth = DiscordAuth(token)
watched_channels = []
cursors = {} # Used to keep track of the last fetched messages
in_game = False

def find_guild(server):
    r = requests.get(API_ENDPOINT + "/users/@me/guilds", auth=auth)
    if r.status_code != 200:
        return None

    # List guilds
    guilds = r.json()
    selected_guild = None
    for guild in guilds:
        if guild["name"] == server:
            selected_guild = guild
    if not selected_guild:
        return None

    return selected_guild


def find_channels(guild, channel_list):
    selected_channels = []
    # List channels
    r = requests.get(API_ENDPOINT + "/guilds/%s/channels" % guild["id"], \
                     auth=auth)
    if r.status_code != 200:
        return None
    channels = r.json()
    for channel in channels:
        if channel["name"] not in channel_list:
            continue
        selected_channels += [channel]

    return selected_channels


def pull_messages(channels):
    global cursors

    if not in_game:
        return

    for channel in channels:
        # Initialize if not present
        if channel["id"] not in cursors:
            cursors[channel["id"]] = channel["last_message_id"]
            continue

        r = requests.get(API_ENDPOINT + "/channels/%s/messages?after=%s" % (channel["id"], \
                                                                            cursors[channel["id"]]), \
                                                                            auth=auth)
        if r.status_code != 200:
            return
        messages = r.json()

        for message in messages:
            author = message["author"]
            ingame_notification = "(#%s) %s: %s" % (channel["name"], \
                                                    author["username"], \
                                                    message["content"])
            ro_client.print_in_chat(ingame_notification, 0xFFFFFF, 0)
            cursors[channel["id"]] = message["id"]

s = 1
# Called every 100ms
def on_tick():
    global watched_channels
    global s

    # Fetch every 5s or so
    if s == 0:
        pull_messages(watched_channels)
    s = (s + 1) % 50


# Called whenever an in-game chat message is received
def on_chat_message(message):
    global in_game

    if not in_game:
        in_game = True


bourgeon.log("-= Discord plugin =-")
guild = find_guild(SERVER)
if guild:
    bourgeon.log("Found discord server: %s" % SERVER)
    watched_channels = find_channels(guild, CHANNELS)
    if watched_channels:
        bourgeon.log("Now listening to:")
        for channel in watched_channels:
            bourgeon.log("- %s" % channel["name"])
        bourgeon.register_callback("OnChatMessage", on_chat_message)
        bourgeon.register_callback("Tick", on_tick)
    else:
        bourgeon.log("No accessible channels found in the server")
else:
    bourgeon.log("Failed to find the discord server '%s'" % SERVER)

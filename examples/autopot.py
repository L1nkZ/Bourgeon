import json
from enum import IntEnum
from typing import List, Dict, Any

import bourgeon
import ragnarok_client as client
from bourgeon import ui
from ragnarok_client import Mode


class Configuration:
    def __init__(self, path: str):
        with open(path) as fp:
            data = json.load(fp)

        self.hp_min_percent = data["hp"]["percent"]
        self.sp_min_percent = data["sp"]["percent"]

        self.hp_items = data["hp"]["items"]
        self.sp_items = data["sp"]["items"]
        self.selected_hp_item = self.hp_items[0]["id"]
        self.selected_sp_item = self.sp_items[0]["id"]


config = Configuration("plugins/autopot.json")


class SettingsWindow:
    class UiMessage(IntEnum):
        AutopotWindowClosed = 1
        AutopotToggle = 2
        HpEdit = 3
        SpEdit = 4
        HpItemSelected = 5
        SpItemSelected = 6

    def __init__(self) -> None:
        self.activated = False
        max_chars = 2
        input_width = 25

        # HP threshold input
        self.hp_input = ui.TextInput(
            "%##hp_input",
            str(config.hp_min_percent),
            ui.AllowedChars.Decimal,
            max_chars,
            self.UiMessage.HpEdit,
        )
        self.hp_input.set_width(input_width)

        # SP threshold input
        self.sp_input = ui.TextInput(
            "%##sp_input",
            str(config.sp_min_percent),
            ui.AllowedChars.Decimal,
            max_chars,
            self.UiMessage.SpEdit,
        )
        self.sp_input.set_width(input_width)

        self.hp_combo = self._combo_from_item_list(
            "##hp_combo", self.UiMessage.HpItemSelected, config.hp_items)
        self.sp_combo = self._combo_from_item_list(
            "##sp_combo", self.UiMessage.SpItemSelected, config.sp_items)

        # Settings window
        self.window = ui.Window("Autopot Settings",
                                [[ui.Text("HP"), self.hp_combo, self.hp_input],
                                 [ui.Text("SP"), self.sp_combo, self.sp_input],
                                 [
                                     ui.CheckBox("Activated", self.activated,
                                                 self.UiMessage.AutopotToggle)
                                 ]], self.UiMessage.AutopotWindowClosed)
        self.window.set_size((240, 105))
        self.window.set_resizable(False)

    def open(self) -> None:
        ui.register_window(self.window)

    def close(self) -> None:
        ui.unregister_window(self.window)

    def handle_messages(self) -> None:
        message = self.window.read()
        while message is not None:
            msg_id, values = message
            if msg_id == self.UiMessage.AutopotWindowClosed:
                self.close()
            elif msg_id == self.UiMessage.AutopotToggle:
                self.activated = values[0]
                self.hp_input.set_read_only(self.activated)
                self.sp_input.set_read_only(self.activated)
                self.hp_combo.set_disabled(self.activated)
                self.sp_combo.set_disabled(self.activated)
                if self.activated:
                    bourgeon.log("Autopot activated")
                    bourgeon.log(
                        f" -> HP {config.hp_min_percent}%/{config.selected_hp_item}"
                    )
                    bourgeon.log(
                        f" -> SP {config.sp_min_percent}%/{config.selected_sp_item}"
                    )
                else:
                    bourgeon.log("Autopot deactivated")
            elif msg_id == self.UiMessage.HpEdit:
                try:
                    config.hp_min_percent = int(values[0])
                except ValueError:
                    config.hp_min_percent = 0
            elif msg_id == self.UiMessage.SpEdit:
                try:
                    config.sp_min_percent = int(values[0])
                except ValueError:
                    config.sp_min_percent = 0
            elif msg_id == self.UiMessage.HpItemSelected:
                item_index = values[0]
                config.selected_hp_item = config.hp_items[item_index]["id"]
            elif msg_id == self.UiMessage.SpItemSelected:
                item_index = values[0]
                config.selected_sp_item = config.sp_items[item_index]["id"]

            message = self.window.read()

    @staticmethod
    def _combo_from_item_list(label: str, msg_id: int,
                              item_list: List[Dict[str, Any]]) -> ui.Combo:
        name_list = [item["name"] for item in item_list]
        return ui.Combo(label, name_list, msg_id)


current_mode = Mode.Login
ap_window = SettingsWindow()


def on_mode_switch(mode_type: Mode, _map_name: str) -> None:
    """
    OnModeSwitch callback.
    """
    global current_mode
    current_mode = mode_type


def on_tick() -> None:
    """
    OnTick callback.
    """
    global ap_window

    ap_window.handle_messages()
    if current_mode == Mode.Game and ap_window.activated:
        hp_percent = (client.get_hp() / client.get_max_hp()) * 100.0
        sp_percent = (client.get_sp() / client.get_max_sp()) * 100.0

        if hp_percent < config.hp_min_percent:
            client.use_item(config.selected_hp_item)
        elif sp_percent < config.sp_min_percent:
            client.use_item(config.selected_sp_item)


def on_command(chat_buffer: str) -> None:
    """
    OnTalkType callback.
    """
    if chat_buffer.find("/autopot") == 0:
        ap_window.open()


bourgeon.register_callback("OnTick", on_tick)
bourgeon.register_callback("OnModeSwitch", on_mode_switch)
bourgeon.register_callback("OnTalkType", on_command)
bourgeon.log("Autopot plugin loaded!")

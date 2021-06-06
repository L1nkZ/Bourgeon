import bourgeon
import ragnarok_client as client
from bourgeon import ui
from ragnarok_client import Mode


class BasicInfoWindow:
    def __init__(self, name: str) -> None:
        self._hp_text = ui.Text("--")
        self._sp_text = ui.Text("--")
        self.window = ui.Window(name, [[
            ui.Text("HP"),
            self._hp_text,
            ui.Text("| SP"),
            self._sp_text,
        ]], 0)

    def open(self) -> None:
        ui.register_window(self.window)

    def close(self) -> None:
        ui.unregister_window(self.window)

    def update(self, hp: int, max_hp: int, sp: int, max_sp: int) -> None:
        self._hp_text.set_text(f"{hp} / {max_hp}")
        self._sp_text.set_text(f"{sp} / {max_sp}")


basic_info_window = None


def on_tick() -> None:
    """
	OnTick callback.
	"""
    global basic_info_window
    if basic_info_window:
        basic_info_window.update(client.get_hp(), client.get_max_hp(),
                                 client.get_sp(), client.get_max_sp())


def on_mode_switch(mode_type: Mode, _map_name: str) -> None:
    """
	OnModeSwitch callback.
	"""
    global basic_info_window
    if mode_type == Mode.Game:
        basic_info_window = BasicInfoWindow(client.get_char_name())
        basic_info_window.open()
    elif mode_type == Mode.Login:
        if basic_info_window:
            basic_info_window.close()


bourgeon.register_callback("OnTick", on_tick)
bourgeon.register_callback("OnModeSwitch", on_mode_switch)

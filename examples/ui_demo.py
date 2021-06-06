from enum import IntEnum

from bourgeon import ui, log, register_callback


class DemoWindow:
    class UiMessage(IntEnum):
        WindowClosed = 1
        ComboItemSelected = 2
        CheckBoxChecked = 3
        TextInputEdited = 4
        ListBoxItemSelected = 5

    def __init__(self):
        self.combo = ui.Combo("Combo", ["Line1", "Line2", "Line3"],
                              self.UiMessage.ComboItemSelected)
        self.check_box_state = False
        self.check_box = ui.CheckBox("CheckBox", self.check_box_state,
                                     self.UiMessage.CheckBoxChecked)
        self.text_input = ui.TextInput(
            "TextInput",
            "Hello",
            ui.AllowedChars.All,
            256,
            self.UiMessage.TextInputEdited,
        )
        self.list_box = ui.ListBox("ListBox", ["Line1", "Line2", "Line3"],
                                   self.UiMessage.ListBoxItemSelected)
        self.list_box.set_size((70, 60))
        self.window = ui.Window(
            "Demo",
            [[ui.Text("Text widget #1"),
              ui.Text("Text widget #2")], [ui.Separator()], [self.combo],
             [self.check_box], [self.text_input], [self.list_box]],
            self.UiMessage.WindowClosed)

    def open(self) -> None:
        ui.register_window(self.window)

    def close(self) -> None:
        ui.unregister_window(self.window)

    def handle_messages(self) -> None:
        message = self.window.read()
        while message is not None:
            msg_id, values = message
            if msg_id == self.UiMessage.WindowClosed:
                self.close()
            elif msg_id == self.UiMessage.ComboItemSelected:
                log(f"Combo item selected: {values[0]}")
            elif msg_id == self.UiMessage.CheckBoxChecked:
                self.check_box_state = values[0]
                log(f"CheckBox state: {self.check_box_state}")
            elif msg_id == self.UiMessage.TextInputEdited:
                log(f"TextInput has been edited: {values[0]}")
            elif msg_id == self.UiMessage.ListBoxItemSelected:
                log(f"ListBox item selected: {values[0]}")

            message = self.window.read()


demo_window = DemoWindow()
demo_window.open()
log("Demo UI loaded!")


def on_tick() -> None:
    """
    OnTick callback.
    """
    global demo_window
    demo_window.handle_messages()


register_callback("OnTick", on_tick)
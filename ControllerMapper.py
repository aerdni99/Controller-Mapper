"""
    ControllerMapper.py

    User Remote Script for Ableton Live 12.
    This script sends events from my DAW to my controller 
    and interprets specific midi messages from my virtual controller for custom transport functionality
"""

from _Framework.ControlSurface import ControlSurface
from _Framework.SubjectSlot import subject_slot

class MyController(ControlSurface):
    def __init__(self, c_instance):
        super(MyController, self).__init__(c_instance)
        self.log_message("MyController loaded")

        # Listen to transport state changes
        self.song().add_is_playing_listener(self.on_playback_changed)

    def on_playback_changed(self):
        if self.song().is_playing:
            # Replace this with your MIDI sending function
            self.log_message("Ableton is PLAYING")
            self.send_playback_midi(True)
        else:
            self.log_message("Ableton is STOPPED")
            self.send_playback_midi(False)

    def send_playback_midi(self, playing):
        # Example: send a MIDI CC 1 high if playing, low if stopped
        midi_value = 127 if playing else 0
        # Channel 0, CC 1
        self.send_midi((0xB0, 1, midi_value))

"""
Data models and structures for the application.

"""

from dataclasses import dataclass
from datetime import datetime


@dataclass
class GamePlay:
    timestamp: datetime
    level_reached: int
    response_times_seconds: list[int]

    def __init__(self, data: dict):
        if not ("level_reached" in data and "response_times" in data):
            raise ValueError("Invalid data for GamePlay")

        self.timestamp = datetime.now()
        self.level_reached = data["level_reached"]
        self.response_times_seconds = data["response_times"]


@dataclass
class User:
    id: int
    name: str
    device_id: str

    def __hash__(self):
        return hash(self.device_id)

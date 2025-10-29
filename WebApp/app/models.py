"""
Data models and structures for the application.

"""


from dataclasses import dataclass
from datetime import datetime

@dataclass
class GamePlay:
    time: datetime
    level_reached: int
    response_times_seconds: list[int]
    def __init__(self, data: dict):
        if not ("timestamp" in data and
                "level_reached" in data and
                "response_time" in data):
            raise ValueError("Invalid data for GamePlay")
        
        self.time = datetime.fromisoformat(data["timestamp"])
        self.level_reached = data["level_reached"]
        self.response_times_seconds = data["response_time"]
        

@dataclass
class User:
    id: int
    name: str
    device_id: str

USERS = [User(id=1, name="Alice", device_id="device_123"),
         User(id=2, name="Bob", device_id="device_456"),
         User(id=3, name="Charlie", device_id="device_789")]


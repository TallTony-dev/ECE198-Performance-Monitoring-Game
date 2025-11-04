from dataclasses import dataclass


@dataclass
class User:
    id: int
    name: str
    device_id: str
    icon_file: str

    def __hash__(self):
        return hash(self.device_id)

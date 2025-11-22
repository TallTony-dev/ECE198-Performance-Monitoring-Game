from datetime import datetime
from rich.table import Table
from rich import print
from src.models.user import User
from src.models.gameplay import GamePlay

USERS = [
    User(
        id=1,
        name="Yiran",
        device_id="78:1C:3C:2B:96:A4",
        icon_file="user_icon_1.png",
    ),
    User(
        id=2,
        name="Tony ",
        device_id="00:4B:12:EF:A5:38",
        icon_file="user_icon_2.png",
    ),
    User(
        id=3, name="Eric ", device_id="AA:BB:CC:11:22:33", icon_file="user_icon_3.png"
    ),
    User(
        id=4,
        name="Jimin",
        device_id="11:22:33:44:55:66",
        icon_file="user_icon_0.png",
    ),
    User(
        id=5,
        name="Tom",
        device_id="11:22:33:44:55:66",
        icon_file="user_icon_default.png",
    ),
    User(
        id=6,
        name="Bob",
        device_id="11:22:33:44:55:66",
        icon_file="user_icon_default.png",
    ),
    User(
        id=7,
        name="John",
        device_id="11:22:33:44:55:66",
        icon_file="user_icon_default.png",
    ),
    User(
        id=8,
        name="Chris",
        device_id="11:22:33:44:55:66",
        icon_file="user_icon_default.png",
    ),
]

game_plays: dict[User, list[GamePlay]] = {}

raw_datas: list[dict[str, object]] = list()

for user in USERS:
    game_plays[user] = []


def handle_device_webhook(data: dict):
    # Handle the actual format from ESP32
    device_id: str = data.get("device_id", "Unknown")
    timestamp: int = data.get("timestamp", 0)
    level_reached: int = data.get("level_reached", 0)
    response_times: list[float] = data.get("response_times", [])

    # Print to console
    table = Table(title="Device Data")
    table.add_column("Entity", style="cyan", no_wrap=True)
    table.add_column("Value", style="magenta")
    table.add_row("Device ID", str(device_id))
    table.add_row("Timestamp", str(timestamp))
    table.add_row("Level Reached", str(level_reached))
    table.add_row("Response Times", str(response_times))

    print(
        f"[bold green]Received device data at {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}[/bold green]"
    )
    print(table)

    game_play = GamePlay(data)
    # Find the user by device_id
    user = next((u for u in USERS if u.device_id == device_id), None)
    if user:
        game_plays[user].append(game_play)
        print(f"[bold blue]Recorded gameplay for user: {user.name}[/bold blue]")
    else:
        print(
            f"[bold red]Unknown device ID: {device_id}[/bold red], could not record gameplay."
        )

    # Record the raw data of each record
    for key in data:
        data[key] = str(data[key])
    data['user'] = user.name if user else "Unknown"
    data['time'] = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    raw_datas.append(data)
    print(data)

    return {"status": "success"}

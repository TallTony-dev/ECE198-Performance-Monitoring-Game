from datetime import datetime, time
from rich.table import Table
from rich import print
from app.models import User, GamePlay

USERS = [
    User(id=1, name="Yiran", device_id="78:1C:3C:2B:96:A4"),
    User(id=2, name="Tony", device_id="AA:BB:CC:DD:EE:FF"),
    User(id=3, name="Eric", device_id="AA:BB:CC:11:22:33"),
    User(id=4, name="Lee", device_id="11:22:33:44:55:66"),
]

game_plays: dict[User, list[GamePlay]] = {}

for user in USERS:
    game_plays[user] = []


def handle_device_webhook(data: dict):
    # Handle the actual format from ESP32
    device_id = data.get("device_id", "Unknown")
    timestamp = data.get("timestamp", 0)
    level_reached = data.get("level_reached", 0)
    response_times = data.get("response_times", [])

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

    return {"status": "success"}

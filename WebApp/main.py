from nicegui import app
from rich import print
from rich.table import Table
from time import time

@app.post('/sensor')
def sensor_webhook(data: dict):
    table = Table(title="Sensor Data")
    table.add_column("Entity", style="cyan", no_wrap=True)
    table.add_column("Value", style="magenta")
    table.add_row("Device ID", str(data["device_id"]))
    table.add_row("Timestamp", str(data["timestamp"]))
    table.add_row("Level Reached", str(data["level_reached"]))
    table.add_row("Response Time", str(data["response_time"]))
    
    print(f"[bold green]Received sensor data at {time()}[/bold green]")
    print(table)

    return {'status': 'success'}
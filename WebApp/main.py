from nicegui import app, ui
from rich import print
from rich.table import Table
from time import time

@app.post('/device/')
def sensor_webhook(data: dict):
    table = Table(title="Sensor Data")
    table.add_column("Entity", style="cyan", no_wrap=True)
    table.add_column("Value", style="magenta")
    table.add_row("Device ID", str(data["device_id"]))
    table.add_row("Timestamp", str(data["timestamp"]))
    table.add_row("Level Reached", str(data["level_reached"]))
    table.add_row("Response Time", str(data["response_time"]))
    
    print(f"[bold green]Received device data at {time()}[/bold green]")
    print(table)

    return {'status': 'success'}

# Optional: Add a simple page so the app has a UI
@ui.page('/')
def index():
    ui.label('Sensor Webhook Receiver')
    ui.label('POST to /sensor to submit data')

# Start the NiceGUI server
if __name__ in {"__main__", "__mp_main__"}:
    ui.run(
        host='0.0.0.0',
        port=8000,
        reload=True
    )

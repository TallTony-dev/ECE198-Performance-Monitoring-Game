from nicegui import app, ui
from rich import print
from rich.table import Table
from time import time
from datetime import datetime

# Store received data
received_data = []

@app.post('/device/')
def device_webhook(data: dict):
    # Store the data with timestamp
    entry = f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} - Device: {data['device_id']}, Timestamp: {data['timestamp']}, Level: {data['level_reached']}, Response: {data['response_time']}"
    received_data.append(entry)
    
    # Also print to console
    table = Table(title="Device Data")
    table.add_column("Entity", style="cyan", no_wrap=True)
    table.add_column("Value", style="magenta")
    table.add_row("Device ID", str(data["device_id"]))
    table.add_row("Timestamp", str(data["timestamp"]))
    table.add_row("Level Reached", str(data["level_reached"]))
    table.add_row("Response Time", str(data["response_time"]))
    
    print(f"[bold green]Received device data at {time()}[/bold green]")
    print(table)

    return {'status': 'success'}

@ui.page('/')
def index():
    ui.label('Device Webhook Receiver').classes('text-h4')
    ui.label('POST to /device/ to submit data').classes('text-subtitle1')
    
    ui.separator()
    
    ui.label(f'Total requests received: {len(received_data)}').classes('text-h6')
    
    # Display all received data
    if received_data:
        ui.label('Received Data:').classes('text-bold')
        for entry in received_data:
            ui.label(entry)
    else:
        ui.label('No data received yet')
    
    # Refresh button
    ui.button('Refresh', on_click=lambda: ui.navigate.reload())

if __name__ in {"__main__", "__mp_main__"}:
    ui.run(
        host='0.0.0.0',
        port=8000,
        reload=False  # Set to False for deployment
    )
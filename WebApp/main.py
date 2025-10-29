from nicegui import app, ui
from rich import print
from rich.table import Table
from time import time
from datetime import datetime

# Store received data
received_data = []

@app.post('/device/')
def device_webhook(data: dict):
    # Handle the actual format from ESP32
    device_id = data.get("device_id", "Unknown")
    timestamp = data.get("timestamp", 0)
    level_reached = data.get("level_reached", 0)
    response_times = data.get("response_times", [])  # This is an array!
    
    # Calculate average response time
    avg_response = sum(response_times) / len(response_times) if response_times else 0
    
    # Store the data with timestamp
    entry = (f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} - "
             f"Device: {device_id}, "
             f"Timestamp: {timestamp}, "
             f"Level: {level_reached}, "
             f"Avg Response: {avg_response:.3f}s, "
             f"Times: {response_times}")
    received_data.append(entry)
    
    # Print to console
    table = Table(title="Device Data")
    table.add_column("Entity", style="cyan", no_wrap=True)
    table.add_column("Value", style="magenta")
    table.add_row("Device ID", str(device_id))
    table.add_row("Timestamp", str(timestamp))
    table.add_row("Level Reached", str(level_reached))
    table.add_row("Response Times", str(response_times))
    table.add_row("Avg Response", f"{avg_response:.3f}s")
    
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
        reload=False
    )
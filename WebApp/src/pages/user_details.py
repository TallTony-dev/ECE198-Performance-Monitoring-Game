from nicegui import ui
from src.services.data_webhook import game_plays, USERS
from src.models.user import User
from src.models.gameplay import GamePlay
from datetime import datetime
from typing import Tuple
import statistics

# Table column definitions
TABLE_HEADERS = [
    {'name': 'index', 'label': '#', 'classes': 'flex-[0.10] min-w-[50px] flex-shrink-0'},
    {'name': 'timestamp', 'label': 'Timestamp', 'classes': 'flex-[0.20] min-w-[180px] flex-shrink-0'},
    {'name': 'level', 'label': 'Max Level', 'classes': 'flex-[0.10] min-w-[100px] flex-shrink-0'},
    {'name': 'response_times', 'label': 'Time spent per Level', 'classes': 'flex-[0.60] min-w-[655px] flex-shrink-0'},
    {'name': 'warning_level', 'label': 'Warning Level', 'classes': 'flex-[0.15] min-w-[150px] flex-shrink-0'},
]


def calculate_warning_level(response_times: list[float]) -> Tuple[str, str, str]:
    """Calculate warning level based on average response time"""
    if not response_times:
        return "N/A", "text-gray-400 bg-gray-100", "No Time"

    try:
        valid_times = [t for t in response_times if t > 0]
        if not valid_times:
            return "N/A", "text-gray-400 bg-gray-100", "No Time"

        avg_time = statistics.mean(valid_times)
        avg_time_str = f"{avg_time:.3f}s"

        GOOD_THRESHOLD = 0.50
        NORMAL_THRESHOLD = 1.50

        if avg_time <= GOOD_THRESHOLD:
            return "Good", "text-green-700 bg-green-100 font-semibold", avg_time_str
        elif avg_time <= NORMAL_THRESHOLD:
            return "Normal", "text-yellow-700 bg-yellow-100 font-semibold", avg_time_str
        else:
            return "Bad", "text-red-700 bg-red-100 font-semibold", avg_time_str
    except Exception:
        return "Error", "text-red-500 bg-red-100", "Error"


def render_user_card(user: User):
    """Render user information card"""
    with ui.card().classes("my-2 p-4 items-center"):
        with ui.row().classes("w-full flex-nowrap"):
            with ui.column().classes("w-30 justify-around"):
                ui.image("/static/" + user.icon_file).classes("w-30 h-30 mt-2 mb-2").style(
                    "border-radius: 50%;"
                )
            with ui.column().classes("w-50 justify-around"):
                ui.label(f"{user.name}").classes("text-h2")
                ui.label(f"User ID: {user.id}").classes("text-gray-300 text-sm pl-2")
                ui.label(f"Device ID: {user.device_id}").classes("text-gray-300 text-sm pl-2")


def render_table_header():
    """Render table header row"""
    with ui.row().classes("w-full bg-gray-100 font-bold text-xs border-b border-gray-300 items-center flex-nowrap px-2"):
        for header in TABLE_HEADERS:
            with ui.element('div').classes(f"p-2 text-left {header['classes']} flex items-center justify-start h-full"):
                ui.label(header['label'])


def render_response_times_cell(response_times: list[float]):
    """Render response times in chunked format"""
    if not response_times:
        ui.label("-").classes("text-gray-400")
        return

    CHUNK_SIZE = 10
    time_chunks = [response_times[i:i + CHUNK_SIZE] for i in range(0, len(response_times), CHUNK_SIZE)]

    with ui.column().classes("w-fit gap-4"):
        for chunk_index, chunk in enumerate(time_chunks):
            start_level = chunk_index * CHUNK_SIZE + 1
            with ui.column().classes("w-full gap-0.5"):
                # Level labels row
                with ui.row().classes("w-full justify-start flex-nowrap"):
                    for i in range(len(chunk)):
                        ui.label(f"Level {start_level + i}").classes(
                            "text-xs font-bold text-gray-700 w-[50px] text-left flex-shrink-0"
                        )
                # Time values row
                with ui.row().classes("w-full justify-start flex-nowrap"):
                    for time_s in chunk:
                        ui.label(f"{time_s:.3f}s").classes(
                            "text-xs font-mono text-gray-800 w-[50px] text-left flex-shrink-0"
                        )


def render_warning_level_cell(response_times: list[float]):
    """Render warning level status badge"""
    status, status_class, avg_time_str = calculate_warning_level(response_times)

    with ui.element('div').classes("flex items-center gap-2"):
        with ui.element('div').classes(f"text-sm py-1.5 px-3 rounded-full {status_class}").style("min-width: 80px; text-align: center;"):
            ui.label(status).classes("whitespace-nowrap")
        if avg_time_str != "No Time" and status != "Error":
            ui.label(f"({avg_time_str})").classes("text-xs text-gray-500 font-medium whitespace-nowrap")


def render_gameplay_row(index: int, play: GamePlay):
    """Render a single gameplay row"""
    row_classes = "w-full text-sm border-b border-gray-100 hover:bg-gray-50 py-[5px] transition-colors flex-nowrap px-2"

    with ui.row().classes(row_classes):
        for header in TABLE_HEADERS:
            field_name = header['name']
            with ui.element('div').classes(f"p-2 {header['classes']} flex flex-col items-start justify-start h-full"):

                if field_name == 'index':
                    ui.label(str(index)).classes("font-medium text-gray-800 whitespace-nowrap mt-[6px]")

                elif field_name == 'timestamp':
                    timestamp_str = play.timestamp.strftime("%Y-%m-%d %H:%M:%S")
                    ui.label(timestamp_str).classes("font-medium text-gray-800 whitespace-nowrap mt-[6px]")

                elif field_name == 'level':
                    ui.label(str(play.level_reached)).classes("font-medium text-gray-800 whitespace-nowrap mt-[6px]")

                elif field_name == 'response_times':
                    render_response_times_cell(play.response_times_seconds)

                elif field_name == 'warning_level':
                    render_warning_level_cell(play.response_times_seconds)


def render_gameplay_table(plays: list[GamePlay]):
    """Render the gameplay data table"""
    with ui.column().classes("w-fit bg-white shadow-sm rounded-xl border border-gray-200 overflow-x-auto gap-0").style("max-width: calc(100% * 1.045)"):
        render_table_header()

        for index, play in enumerate(plays, 1):
            render_gameplay_row(index, play)


def user_details(userid: int):
    """Display user details page with gameplay history"""
    # Find user by id
    user = next((u for u in USERS if u.id == userid), None)

    if not user:
        ui.label(f"User '{userid}' not found").classes("text-red-500 text-xl")
        return

    # Get user's gameplays
    plays = game_plays.get(user, [])

    # Layout
    with ui.row().classes("w-full flex-nowrap"):
        with ui.column().classes("w-[80%]"):
            render_user_card(user)
        with ui.column().classes("w-[20%]"):
            ui.log(100)

    ui.separator()

    # Gameplay history section
    ui.label("Gameplay History").classes("text-xl font-bold mt-4 mb-2")

    if plays:
        render_gameplay_table(plays)
    else:
        ui.label("No gameplay records yet").classes("text-gray-500 italic")

    # Refresh button
    ui.button("Refresh", on_click=lambda: ui.navigate.reload()).classes(
        "mt-4 text-white font-semibold py-2 px-5 rounded-lg "
        "!bg-[hsl(240_5.9%_10%)] !hover:bg-[hsl(240_5.9%_5%)] "
        "shadow-md transition-colors duration-200"
    )

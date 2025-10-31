from nicegui import ui
from app.data_webhook import game_plays


def home_page():
    ui.label("Device Webhook Receiver").classes("text-h4")
    ui.label("POST to /device/ to submit data").classes("text-subtitle1")
    ui.separator()

    # Display data grouped by users
    for user, plays in game_plays.items():
        render_user_card(user, plays)

    # Refresh button
    ui.button("Refresh", on_click=lambda: ui.navigate.reload())


def render_user_card(user, plays):
    """Render a card for a single user with their gameplay data"""
    with ui.card().classes("my-2 p-4"):
        ui.label(f"User: {user.name} (Device ID: {user.device_id})").classes("text-h6")

        if not plays:
            ui.label("No gameplay data recorded yet.")
            return

        for play in plays:
            render_gameplay_expansion(play)


def render_gameplay_expansion(play):
    """Render an expansion showing gameplay details"""
    title = f'Gameplay at {play.timestamp.strftime("%Y-%m-%d %H:%M:%S")} - Level Reached: {play.level_reached}'

    with ui.expansion(title):
        ui.label(f"Response Times (s): {play.response_times_seconds}")

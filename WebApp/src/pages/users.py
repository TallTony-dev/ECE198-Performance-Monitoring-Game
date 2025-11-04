from nicegui import ui
from services.data_webhook import game_plays
from models.user import User
from models.gameplay import GamePlay


def users_list():
    ui.label("Users").classes("text-h1")
    ui.separator()

    with ui.grid(columns=4):
        # Display data grouped by users
        for user, plays in game_plays.items():
            render_user_card(user, plays)

    # Refresh button
    ui.button("Refresh", on_click=lambda: ui.navigate.reload())


def render_user_card(user: User, plays: list[GamePlay]):
    """Render a card for a single user with their gameplay data"""
    with ui.card().classes("my-2 p-4 items-center"):
        ui.image("/static/user_icon.png").classes("w-16 h-16 mb-2").style(
            "border-radius: 16%;opacity: 0.8;"
        )
        ui.label(user.name).classes("text-h6")
        ui.label(f"Device ID: {user.device_id}").classes("text-gray-300 text-sm")

        if not plays:
            ui.label("No gameplay data recorded yet.")
            return

        for play in plays:
            render_gameplay_expansion(play)


def render_gameplay_expansion(play: GamePlay):
    """Render an expansion showing gameplay details"""
    title = f'Gameplay at {play.timestamp.strftime("%Y-%m-%d %H:%M:%S")} - Level Reached: {play.level_reached}'

    with ui.expansion(title):
        ui.label(f"Response Times (s): {play.response_times_seconds}")

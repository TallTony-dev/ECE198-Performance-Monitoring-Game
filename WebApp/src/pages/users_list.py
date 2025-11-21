from nicegui import ui
from src.services.data_webhook import game_plays
from src.models.user import User
from src.models.gameplay import GamePlay


def users_list():
    ui.label("Users").classes("text-h1")
    ui.separator()

    with ui.grid(columns=4).classes("gap-4 w-full"):
        # Display data grouped by users
        for user, plays in game_plays.items():
            render_user_card(user, plays)

    # Refresh button
    ui.button("Refresh", on_click=lambda: ui.navigate.reload()).classes(
        "mt-4 text-white font-semibold py-2 px-5 rounded-lg "
        "!bg-[hsl(240_5.9%_10%)] !hover:bg-[hsl(240_5.9%_5%)] "
        "shadow-md transition-colors duration-200"
    )


def render_user_card(user: User, plays: list[GamePlay]):
    """Render a card for a single user with their gameplay data"""
    with ui.card().classes("h-60 my-2 p-4 items-center cursor-pointer").style(
        "min-width: 250px; border-radius: 40px;"
    ).on("click", lambda: ui.navigate.to(f"/user/{user.id}")):
        ui.element("div").style(
            "position: absolute; top: 0; left: 0; right: 0; height: 80px; "
            "background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); "
            "border-radius: 40px 40px 0 0; z-index: 0;"
        )

        with ui.row().classes("w-full justify-around mb-1 mt-2 text-white").style(
            "z-index: 1; position: absolute;"
        ):
            with ui.column().classes("items-center"):
                ui.label("User").classes("text-h5")
            with ui.column().classes("items-center"):
                ui.label(f"Id: {user.id}").classes("text-sm py-1")

        # Number displays on top
        with ui.row().classes("w-full justify-around mb-1 mt-20").style(
            "z-index: 1; position: absolute;"
        ):
            with ui.column().classes("items-center"):
                ui.label("Plays").classes("text-xs my-0")
                ui.label(str(len(plays))).classes("text-h6 font-bold my-0")
            with ui.column():
                pass
            with ui.column().classes("items-center"):
                best = max([p.level_reached for p in plays]) if plays else 0
                ui.label("Best Level").classes("text-xs my-0")
                ui.label(str(best)).classes("text-h6 font-bold my-0")

        ui.image("/static/" + user.icon_file).classes("w-20 h-20 mt-6 mb-0").style(
            "border-radius: 50%;"
        )
        ui.label(user.name).classes("text-h6").style("margin-y: 0px;")
        ui.label(f"Device ID: {user.device_id}").classes("text-gray-300 text-sm")


def render_gameplay_expansion(play: GamePlay):
    """Render an expansion showing gameplay details"""
    title = f'Gameplay at {play.timestamp.strftime("%Y-%m-%d %H:%M:%S")} - Level Reached: {play.level_reached}'

    with ui.expansion(title):
        ui.label(f"Response Times (s): {play.response_times_seconds}")

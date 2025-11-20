from nicegui import ui
from src.services.data_webhook import game_plays
from src.models.user import User
from src.models.gameplay import GamePlay

TEST_USER = User(
        id=1,
        name="Yiran",
        device_id="78:1C:3C:2B:96:A4",
        icon_file="user_icon_1.png",
    )

def user_card(user:User):
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
            with ui.column().classes("w-50 justify-around"):
                ui.label("Lorem Ipsum")

def user_details(username: str):
    with ui.row().classes("w-full flex-nowrap"):
        with ui.column().classes("w-[80%]"):
            user_card(TEST_USER)
        with ui.column().classes("w-[20%]"):
            log = ui.log(100)


    ui.separator()

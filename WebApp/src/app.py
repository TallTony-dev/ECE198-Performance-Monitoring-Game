from nicegui import ui, app

from src.pages.users import users_list
from src.services.data_webhook import handle_device_webhook


def register_routes() -> None:
    @ui.page("/", title="Home")
    def home():
        users_list()

    @app.post("/device")
    def device_webhook(data: dict):
        return handle_device_webhook(data)


if __name__ in {"__main__", "__mp_main__"}:
    register_routes()
    app.add_static_files("/static", "../static")
    ui.run(host="0.0.0.0", port=8000, reload=True)

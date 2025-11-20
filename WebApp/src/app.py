from nicegui import ui, app

from src.pages.users_list import users_list
from src.pages.raw_data import raw_data_records_page
from src.pages.user_details import user_details
from src.services.data_webhook import handle_device_webhook


def register_routes() -> None:
    @ui.page("/", title="Home")
    def home():
        users_list()

    @ui.page("/raw", title="Raw Data Records")
    def raw_data_records():
        raw_data_records_page()

    @ui.page("/user/{username}")
    def user_page(username: str):
        user_details(username)

    @app.post("/device")
    def device_webhook(data: dict):
        return handle_device_webhook(data)


if __name__ in {"__main__", "__mp_main__"}:
    register_routes()
    app.add_static_files("/static", "./static")
    ui.run(host="0.0.0.0", port=8000, reload=True)

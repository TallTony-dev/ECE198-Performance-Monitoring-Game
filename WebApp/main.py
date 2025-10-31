from nicegui import ui, app
from app.pages import home_page  # Ensure pages are registered
from app.data_webhook import handle_device_webhook


def register_routes() -> None:
    @ui.page("/")
    def index():
        home_page()

    @app.post("/device")
    def device_webhook(data: dict):
        return handle_device_webhook(data)


if __name__ in {"__main__", "__mp_main__"}:
    register_routes()
    ui.run(host="0.0.0.0", port=8000, reload=True)

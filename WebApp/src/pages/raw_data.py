from nicegui import ui

from src.services.data_webhook import raw_datas

def raw_data_records_page():
    table = ui.table(
        columns=[
            {'name': 'user', 'label': 'User Name', 'field': 'user', 'align': 'left', 'sortable': True},
            {'name': 'device_id', 'label': 'Device ID', 'field': 'device_id', 'align': 'left', 'sortable': False},
            {'name': 'time', 'label': 'Time', 'field': 'time', 'align': 'left', 'sortable': True},
            {'name': 'level_reached', 'label': 'Level Reached', 'field': 'level_reached', 'align': 'left', 'sortable': True},
            {'name': 'response_times', 'label': 'Response Times (s)', 'field': 'response_times', 'align': 'left', 'sortable': False},
        ],
        rows= raw_datas,
        row_key='time',
        pagination=10
    ).style("width:100%")

    ui.input('Search by User Name').bind_value(table, 'filter')
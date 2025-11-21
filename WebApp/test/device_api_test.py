import requests
import time
from rich.console import Console
from rich.panel import Panel
from rich.table import Table
from rich.prompt import Prompt, IntPrompt
from rich import box
from rich.progress import Progress, SpinnerColumn, TextColumn

console = Console()

# Predefined list of users with their device IDs
USERS: list[dict[str, str]] = [
    {"id": "1", "name": "Yiran", "device_id": "78:1C:3C:2B:96:A4"},
    {"id": "2", "name": "Tony", "device_id": "AA:BB:CC:DD:EE:FF"},
    {"id": "3", "name": "Eric", "device_id": "AA:BB:CC:11:22:33"},
    {"id": "4", "name": "Jimin", "device_id": "11:22:33:44:55:66"},
]

# Server URL
SERVER_URL = "http://127.0.0.1:8000/device/"


def display_users() -> None:
    """Display available users in a table"""
    table = Table(title="Available Users", box=box.ROUNDED)
    table.add_column("Number", style="cyan", justify="center")
    table.add_column("Name", style="green")
    table.add_column("Device ID", style="yellow")

    for idx, user in enumerate(USERS, 1):
        table.add_row(str(idx), user["name"], user["device_id"])

    console.print(table)


def select_user() -> dict[str, str]:
    """Let user select from the predefined list"""
    display_users()

    choice = IntPrompt.ask("\n[bold cyan]Select user number[/bold cyan]", default=1)
    if not (1 <= choice <= len(USERS)):
        console.print(f"[red]✗[/red] Please enter a number between 1 and {len(USERS)}")
        return select_user()

    selected_user = USERS[choice - 1]
    console.print(
        f"\n[green]✓[/green] Selected: [bold]{selected_user['name']}[/bold] ({selected_user['device_id']})"
    )
    return selected_user


def collect_response_times() -> list[float]:
    """Collect response times from user with validation"""
    response_times: list[float] = []

    console.print("\n[bold cyan]Enter Response Times[/bold cyan]")
    console.print("[dim]Enter each response time (in seconds) and press Enter.[/dim]")
    console.print("[dim]Press Enter without input to finish.[/dim]\n")

    round_num = 1
    while True:
        user_input = Prompt.ask(f"[cyan]Round {round_num}[/cyan]", default="")

        # Empty input means we're done
        if user_input == "":
            break

        response_time: float = 0.0
        try:
            response_time = float(user_input)
        except ValueError:
            console.print(
                "[red]✗[/red] Invalid number. Please enter a valid decimal number."
            )
            continue

        # Validate that it's a positive number
        if response_time <= 0:
            console.print("[red]✗[/red] Response time must be positive. Try again.")
            continue

        response_times.append(response_time)
        console.print(f"[green]✓[/green] Added: {response_time}s")
        round_num += 1

    return response_times


def send_data(device_id: str, level_reached: int, response_times: list[float]) -> None:
    """Send data to server with progress indicator"""
    data: dict[str, object] = {
        "device_id": device_id,
        "timestamp": int(time.time() * 1000),
        "level_reached": level_reached,
        "response_times": response_times,
    }

    # Display the data being sent
    console.print("\n")
    console.print(
        Panel.fit(
            f"[bold]Device ID:[/bold] {device_id}\n"
            f"[bold]Level Reached:[/bold] {level_reached}\n"
            f"[bold]Response Times:[/bold] {response_times}\n"
            f"[bold]Timestamp:[/bold] {data['timestamp']}",
            title="[bold cyan]Data to Send[/bold cyan]",
            border_style="cyan",
        )
    )

    try:
        with Progress(
            SpinnerColumn(),
            TextColumn("[progress.description]{task.description}"),
            console=console,
        ) as progress:
            task = progress.add_task("[cyan]Sending data to server...", total=None)

            response = requests.post(SERVER_URL, json=data, timeout=10)
            progress.update(task, completed=True)

        # Display response
        if response.status_code == 200:
            console.print(
                Panel.fit(
                    f"[green bold]✓ Success![/green bold]\n\n"
                    f"[bold]Status Code:[/bold] {response.status_code}\n"
                    f"[bold]Response:[/bold] {response.json()}\n\n"
                    f"[cyan]View your data at:[/cyan]\n"
                    f"[link={SERVER_URL}]{SERVER_URL}[/link]",
                    title="[bold green]Server Response[/bold green]",
                    border_style="green",
                )
            )
        else:
            console.print(
                Panel.fit(
                    f"[yellow]Unexpected status code: {response.status_code}[/yellow]\n"
                    f"[bold]Response:[/bold] {response.text}",
                    title="[bold yellow]Warning[/bold yellow]",
                    border_style="yellow",
                )
            )

    except requests.exceptions.Timeout:
        console.print("[red]✗[/red] Request timed out. The server may be busy.")
    except requests.exceptions.ConnectionError:
        console.print("[red]✗[/red] Connection error. Check if the server is running.")
    except Exception as e:
        console.print(f"[red]✗[/red] Error: {e}")


def main():
    """Main function"""
    console.clear()
    console.print(
        Panel.fit(
            "[bold cyan]ECE198 Performance Monitoring Game[/bold cyan]\n"
            "[dim]Data Submission Tool[/dim]",
            border_style="cyan",
        )
    )

    # Step 1: Select user
    user = select_user()

    # Step 2: Collect response times
    response_times = collect_response_times()

    # Step 3: Calculate level reached
    level_reached = len(response_times)

    if level_reached == 0:
        console.print("\n[yellow]No response times entered. Exiting.[/yellow]")
        return

    console.print(
        f"\n[green]✓[/green] Collected [bold]{level_reached}[/bold] response time(s)"
    )

    # Step 4: Send data
    send_data(user["device_id"], level_reached, response_times)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        console.print("\n\n[yellow]Program interrupted by user[/yellow]")

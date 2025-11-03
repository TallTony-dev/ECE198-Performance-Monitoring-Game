import requests
import time
import ast

# Your Server URL
url = "https://ece198-performance-monitoring-game.onrender.com/device/"

try:
    # Input example
    raw = input("Format: AA:BB:CC:DD:EE:FF 5 [0.125, 0.234, 0.189, 0.156, 0.201]\nEnter data: ").strip()
    parts = raw.split(" ", 2)

    # Format verification
    if len(parts) != 3:
        raise ValueError("Invalid format. There are no 3 elements.")

    device_id = parts[0]

    # level_reached
    try:
        level_reached = int(parts[1])
        if level_reached <= 0:
            raise ValueError
    except ValueError:
        raise ValueError("Level must be a positive integer.")

    # response_times parsing
    try:
        response_times = ast.literal_eval(parts[2])
        if not isinstance(response_times, list):
            raise ValueError
    except Exception:
        raise ValueError("Invalid list format for response times. Use [0,1,2].")
    
    # Adjusting array length
    if len(response_times) < level_reached:
        response_times += [0.0] * (level_reached - len(response_times))
    elif len(response_times) > level_reached:
        response_times = response_times[:level_reached]

    # ESP32 data format
    data = {
        "device_id": device_id,
        "timestamp": int(time.time() * 1000),
        "level_reached": level_reached,
        "response_times": response_times
    }

    print("Sending data to server...")
    print(f"URL: {url}")
    print(f"Data: {data}")
    print()

    try:
        # Send POST request
        response = requests.post(url, json=data, timeout=10)
        
        # Print results
        print(f"Status Code: {response.status_code}")
        print(f"Response: {response.json()}")
        
        if response.status_code == 200:
            print("\nSuccess! Check the webpage to see your data.")
            print("Visit: https://ece198-performance-monitoring-game.onrender.com/")
        else:
            print(f"\n Unexpected status code: {response.status_code}")
            print(f"Response text: {response.text}")
            
    except requests.exceptions.Timeout:
        print("Request timed out")
    except requests.exceptions.ConnectionError:
        print("Connection error - check if server is running")
    except Exception as e:
        print(f"Error: {e}")
            
except Exception as e:
    print(e)
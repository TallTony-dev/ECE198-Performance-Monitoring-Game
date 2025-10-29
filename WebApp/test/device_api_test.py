import requests
import time

# Your Render URL
url = "https://ece198-performance-monitoring-game.onrender.com/device/"

# Sample data matching ESP32 format
data = {
    "device_id": "AA:BB:CC:DD:EE:FF",
    "timestamp": int(time.time() * 1000),  # milliseconds
    "level_reached": 5,
    "response_times": [0.125, 0.234, 0.189, 0.156, 0.201]
}

print("Sending data to server...")
print(f"URL: {url}")
print(f"Data: {data}")
print()

try:
    # Send POST request
    response = requests.post(url, json=data, timeout=10)
    
    # Print results
    print(f"✅ Status Code: {response.status_code}")
    print(f"✅ Response: {response.json()}")
    
    if response.status_code == 200:
        print("\n🎉 Success! Check the webpage to see your data.")
        print("Visit: https://ece198-performance-monitoring-game.onrender.com/")
    else:
        print(f"\n⚠️ Unexpected status code: {response.status_code}")
        print(f"Response text: {response.text}")
        
except requests.exceptions.Timeout:
    print("❌ Request timed out")
except requests.exceptions.ConnectionError:
    print("❌ Connection error - check if server is running")
except Exception as e:
    print(f"❌ Error: {e}")
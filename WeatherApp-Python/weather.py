import os
import sys
import requests

"""
Weather App (Python)
Fetches live weather data using the OpenWeather API.

Setup:
  1) Create a free account on https://openweathermap.org/
  2) Get your API key and set it in your environment:
     - macOS/Linux: export OPENWEATHER_API_KEY="your_key_here"
     - Windows PowerShell: setx OPENWEATHER_API_KEY "your_key_here"
  3) Install dependency: pip install requests
"""

API_KEY = os.getenv("OPENWEATHER_API_KEY")

def get_weather(city: str, units: str = "metric"):
    if not API_KEY:
        raise RuntimeError("Missing OPENWEATHER_API_KEY environment variable.")
    url = "https://api.openweathermap.org/data/2.5/weather"
    params = {"q": city, "appid": API_KEY, "units": units}
    resp = requests.get(url, params=params, timeout=10)
    resp.raise_for_status()
    data = resp.json()
    return {
        "city": data.get("name", city),
        "temp": data["main"]["temp"],
        "feels_like": data["main"]["feels_like"],
        "humidity": data["main"]["humidity"],
        "condition": data["weather"][0]["description"],
        "wind": data["wind"]["speed"],
        "units": "C" if units == "metric" else "F",
    }

def main():
    if len(sys.argv) >= 2:
        city = " ".join(sys.argv[1:])
    else:
        city = input("Enter city (e.g., Fort Wayne): ").strip() or "Fort Wayne"

    try:
        w = get_weather(city, units="metric")
        print(f"Weather in {w['city']}: {w['temp']}°{w['units']} (feels like {w['feels_like']}°{w['units']}), {w['condition']}")
        print(f"Humidity: {w['humidity']}% | Wind: {w['wind']} m/s")
    except Exception as e:
        print("Error:", e)
        print("Make sure your OPENWEATHER_API_KEY is set and city name is valid.")

if __name__ == "__main__":
    main()

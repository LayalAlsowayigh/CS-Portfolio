# Weather App (Python)

## Description
A simple Python program that fetches live weather data using the OpenWeather API.
It displays the current temperature, feels-like, humidity, wind, and conditions for a given city.

## Technologies
- Python
- Requests library
- OpenWeather API
- JSON parsing

## Features
- Fetches real-time weather data
- Displays temperature, feels-like, humidity, wind, and conditions
- Accepts the city name via command line or input prompt

## Setup & How to Run
```bash
# 1) Install dependency
pip install requests

# 2) Set your API key (replace your_key_here)
# macOS/Linux:
export OPENWEATHER_API_KEY="your_key_here"
# Windows PowerShell:
setx OPENWEATHER_API_KEY "your_key_here"

# 3) Run the app
python weather.py Fort Wayne
# or simply:
python weather.py
# and then type a city when prompted
```

## Example Output
```
Weather in Fort Wayne: 17°C (feels like 15°C), clear sky
Humidity: 62% | Wind: 3.5 m/s
```

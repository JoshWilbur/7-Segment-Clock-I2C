import requests


class Weather:
    # This class will hold methods for obtaining weather stats
    def __init__(self):
        pass

    @staticmethod
    def current_temperature_f(location):
        url = f"http://wttr.in/{location}?format=%t"  # Use wttr.in for easy access to stats, no API key required
        https_response = requests.get(url, timeout=10)  # Attempt to connect to website, 10 second connection timeout

        # Check if the request was successful, remove degree symbol
        if https_response.status_code == 200:
            temp_str = https_response.text.strip().replace("°F", "")
            return int(temp_str)
        return None


if __name__ == "__main__":
    # Obtain current temperature for Bangor, Maine (~15 miles from me)
    weather = Weather()
    place = "Bangor"
    temp = weather.current_temperature_f(place)
    if temp is not None:
        print(temp)
    else:
        print("Error when getting temp")

    if response and response.text:
        print("Response:", response.status_code)
        print("Content:", response.text)
import requests
from sys import argv

url = "http://127.0.0.1:9000/test"

payload = {"name":"wzp", "id":6}

if __name__ == '__main__':
    r = requests.get(url, params=payload)
    print r.text
    r = requests.post(url, data=payload)
    print r.text

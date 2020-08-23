import requests
from clint.textui import progress

url = 'https://dblp.org/xml/release/dblp-2020-05-01.xml.gz'
res = requests.get(url, stream=True)
total_length = int(res.headers.get('content-length'))
chunk_size = 10 * 1024 * 1024
with open("dblp-2020-05-01.xml.gz", "wb") as pypkg:
    for chunk in progress.bar(res.iter_content(chunk_size=chunk_size), expected_size=(total_length/chunk_size) + 1, width=100):
        if chunk:
            pypkg.write(chunk)
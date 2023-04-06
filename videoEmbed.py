import requests
import re

from flask import Flask
from flask import render_template

app = Flask(__name__, static_folder='assets', template_folder='templates')

@app.route('/index.html')
def home():
    return render_template('index.html')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/videos.html')
def videos():
    return render_template('videos.html')

@app.route('/requests.html')
def request():
    return render_template('requests.html')

@app.route('/about.html')
def about():
    return render_template('about.html')

@app.route('/calendar.html')
def calendar():
    return render_template('calendar.html')

"""
def get_embed_links(urls):
    video_ids = [re.findall(r'(?:v=|\/)([a-zA-Z0-9_-]{11})(?:\?|&|$)', url)[0] for url in urls]
    embed_links = [f'https://www.youtube.com/embed/{id}' for id in video_ids]
    return embed_links

@app.route('/get_videos_html')
def get_videos_html():
    video_urls = requests.get('').json()
    embed_links = get_embed_links(video_urls)
    videos_html = ''
    for link in embed_links:
        videos_html += f'''
        <div class="col-xl-3 col-lg-4 col-md-6">
            <div class="gallery-item h-100">
                <iframe width="560" height="315" src="{link}" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>
                <div class="gallery-links d-flex align-items-center justify-content-center">
                    <a href="{link}" title="Video" class="glightbox preview-link"><i class="bi bi-arrows-angle-expand"></i></a>
                </div>
            </div>
        </div>
        '''
    return f'<div class="container-fluid"><div class="row gy-4 justify-content-center">{videos_html}</div></div>'
"""

if __name__ == '__main__':
    app.run("192.168.0.161", 8081, True)

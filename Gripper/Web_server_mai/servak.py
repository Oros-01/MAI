from flask import Flask, send_file

app = Flask(__name__)

button_pressed = False

@app.route('/')
@app.route('/')
def index():
    with open('index.html', encoding='utf-8') as f:
        return f.read(), 200, {'Content-Type': 'text/html; charset=utf-8'}

@app.route('/press', methods=['POST'])
def press():
    global button_pressed
    button_pressed = True
    return {'status': 'ok'}

@app.route('/reset', methods=['POST'])
def reset():
    global button_pressed
    button_pressed = False
    return {'status': 'ok'}

@app.route('/state')
def state():
    return {'pressed': button_pressed}

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
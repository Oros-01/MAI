from flask import Flask
import os

app = Flask(__name__)

state = {"pressed": False}

HTML = """
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Gripper</title>
</head>
<body style="display:flex; flex-direction:column; align-items:center; padding-top:100px; font-family:sans-serif">

  <button
    style="padding:20px 40px; font-size:20px; cursor:pointer"
    onmousedown="send('/press')" <!-- Логика мышки/пальца -->
    onmouseup="send('/reset')"
    ontouchstart="send('/press')"
    ontouchend="send('/reset')"
  >
    Сжать
  </button>

  <div id="status" style="margin-top:30px; font-size:24px">Не сжато</div>

  <script>
    async function send(url) {
      await fetch(url, {method: 'POST'});
    }

    setInterval(async () => {
      const r = await fetch('/state');
      const d = await r.json();
      document.getElementById('status').textContent = d.pressed ? 'Сжато!' : 'Не сжато';
    }, 300);
  </script>

</body>
</html>
"""

@app.route('/')
def index():
    return HTML

@app.route('/press', methods=['POST'])
def press():
    global state
    state['pressed'] = True
    return {'status': 'ok'}

@app.route('/reset', methods=['POST'])
def reset():
    global state
    state['pressed'] = False
    return {'status': 'ok'}

@app.route('/state')
def get_state():
    return state




if __name__ == '__main__':
    port = int(os.environ.get('PORT', 5000))
    app.run(host='0.0.0.0', port=port, debug=False)
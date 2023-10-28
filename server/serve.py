from flask import Flask, request, render_template, jsonify

app = Flask(__name__, template_folder='template')
gpio_state = {'gpioState': []}

@app.route('/gpio_state', methods=['POST'])
def receive_data():
    try:
        content = request.get_json()
        received_gpio_state = content['gpioState']
        gpio_state['gpioState'] = received_gpio_state
        
        print(f"Received data: gpio_state={received_gpio_state}")
        return jsonify({'success': True})
    except Exception as e:
        print(f"Error receiving gpio_state: {str(e)}")
        return jsonify({'success': False, 'error': str(e)})

@app.route('/get_gpio_state', methods=['GET'])
def get_gpio_state():
    return jsonify(gpio_state)

@app.route('/', methods=['GET'])
def home():
    return render_template("index.html", gpio_state=gpio_state)

if __name__ == '__main__':
    app.run(host="0.0.0.0")
    
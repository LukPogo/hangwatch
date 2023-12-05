import time
from flask import Flask, request,  jsonify

app = Flask(__name__, template_folder='template')

state={}

@app.route('/hooks', methods=['POST'])
def receive_data():
    
    lastUpdate = time.time()
    global state
    try:

        content = request.get_json() 
        received_state = content['state']
        board_id = content['board_id']
        if board_id not in state:
            state[board_id] = {}

         
        state[board_id]['state'] = received_state
        state[board_id]['place'] = content['place']
        state[board_id]['time'] = lastUpdate
        
        print(f"Board ID: {board_id}, Place: {state[board_id]['place']}, State: {state[board_id]['state']}")
        
        return jsonify({'success': True})
    except Exception as e:
        print(f"Error receiving state: {str(e)}")
        return jsonify({'success': False, 'error': str(e)})


@app.route('/hooks', methods=['GET'])
def check_state():
    global state
    for board_id, board_state in state.items():
        if (time.time() - board_state['time']) > 7200:
            board_state['state'] = 'offline'
        elif (time.time() - board_state['time']) > 10:
            board_state['state'] = 'empty'    
    
    
    return jsonify(state)


if __name__ == '__main__':
    app.run(host="0.0.0.0", debug=True)
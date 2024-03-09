#!/usr/bin/env python3

import argparse
import time
from flask import Flask, request,  jsonify, make_response

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
            state[board_id] = {"state_change_time": lastUpdate}

        if state[board_id]['state'] != received_state:
            state[board_id]['state'] = received_state
            state[board_id]['state_change_time'] = lastUpdate
        state[board_id]['place'] = content['place']
        state[board_id]['last_activity_time'] = lastUpdate
        
        print(f"Board ID: {board_id}, Place: {state[board_id]['place']}, State: {state[board_id]['state']}")
        
        return jsonify({'success': True})
    except Exception as e:
        print(f"Error receiving state: {str(e)}")
        return jsonify({'success': False, 'error': str(e)})


@app.route('/hooks', methods=['GET'])
def check_state():
    global state
    for board_id, board_state in state.items():
        if (time.time() - board_state['last_activity_time']) > 7200:
            board_state['state'] = 'offline'
            board_state['state_change_time'] = time.time()

    response = make_response(jsonify(state))
    response.headers['Access-Control-Allow-Origin'] = '*'
    return response


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Server for receiving state from boards')
    parser.add_argument('--port', type=int, default=5000, help='Port to run the server on')
    args = parser.parse_args()
    app.run(host="0.0.0.0", port=args.port, debug=True)

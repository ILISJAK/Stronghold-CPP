import requests
import json

base_url = 'http://localhost:8080'

def start_game(num_players):
    start_game_url = base_url + '/start-game'
    data = {'numPlayers': num_players}
    try:
        response = requests.post(start_game_url, json=data)
        response.raise_for_status()
        print_response(response)
    except requests.exceptions.RequestException as e:
        print('An error occurred during the request:', e)
    print()

def make_move(player_identifier, action, amount):
    make_move_url = base_url + '/make-move'
    data = {'playerIdentifier': player_identifier, 'action': action, 'amount': amount}
    try:
        response = requests.post(make_move_url, json=data)
        response.raise_for_status()
        print_response(response)
    except requests.exceptions.RequestException as e:
        print('An error occurred during the request:', e)
    print()

def print_response(response):
    print('Response:', response.status_code)
    print('Content:', response.text)
    print()

# Interactive terminal testing
while True:
    print('Select an action:')
    print('1. Start Game')
    print('2. Make Move')
    print('3. Quit')
    choice = input('Enter your choice (1-3): ')

    if choice == '1':
        num_players = int(input('Enter the number of players: '))
        start_game(num_players)
    elif choice == '2':
        player_identifier = input('Enter the player identifier: ')
        action = input('Enter the action: ')
        amount = int(input('Enter the amount: '))
        make_move(player_identifier, action, amount)
    elif choice == '3':
        print('Exiting the program.')
        break
    else:
        print('Invalid choice. Please try again.')

import requests
import json

base_url = "http://localhost:8080"


def start_game(num_players):
    start_game_url = base_url + "/start-game"
    data = {"numPlayers": num_players}

    print("\nRequest data:", data)  # debug logging
    try:
        response = requests.post(start_game_url, json=data)
        response.raise_for_status()
        print_response(response)
    except requests.exceptions.RequestException as e:
        print("An error occurred during the request:", e)
    print()


def make_move(
    player_identifier,
    action,
    amount=None,
    entity_type=None,
    target_player_identifier=None,
):
    make_move_url = base_url + "/make-move"
    data = {"playerIdentifier": player_identifier, "action": action}

    if amount is not None:
        data["amount"] = amount

    if entity_type is not None:
        if action == "train-troop":
            data["troop"] = entity_type
        elif action == "build-structure":
            data["structure"] = entity_type
        elif action == "buy-from-market" or action == "sell-to-market":
            data["resource"] = entity_type

    if target_player_identifier is not None:
        data["targetPlayerIdentifier"] = target_player_identifier

    print("\nRequest data:", data)  # debug logging

    try:
        response = requests.post(make_move_url, json=data)
        response.raise_for_status()
        print_response(response)
    except requests.exceptions.RequestException as e:
        print("An error occurred during the request:", e)

    print()


def print_response(response):
    print("Response:", response.status_code)
    print("Content:", response.text)
    print()


# Interactive terminal testing
while True:
    print("Select an action:")
    print("1. Start Game")
    print("2. Make Move")
    print("3. Quit")
    choice = input("Enter your choice (1-3): ")

    if choice == "1":
        num_players = int(input("Enter the number of players: "))
        start_game(num_players)
    elif choice == "2":
        player_identifier = input("Enter the player identifier: ")
        action = input("Enter the action: ")
        if action == "train-troop":
            troop_type = input("Enter the troop type: ")
            amount = int(input("Enter the amount: "))
            make_move(player_identifier, action, amount, troop_type)
        elif action == "build-structure":
            structure_type = input("Enter the structure type: ")
            amount = int(input("Enter the amount: "))
            make_move(player_identifier, action, amount, structure_type)
        elif action == "buy-from-market" or action == "sell-to-market":
            resource = input("Enter the resource: ")
            amount = int(input("Enter the amount: "))
            make_move(player_identifier, action, amount, resource)
        elif action == "train-villager" or action == "build-housing":
            amount = int(input("Enter the amount: "))
            make_move(player_identifier, action, amount)
        elif action == "attack-lord":
            target_player_identifier = input("Enter the target player identifier: ")
            make_move(
                player_identifier,
                action,
                target_player_identifier=target_player_identifier,
            )
        elif action == "raid":
            target_player_identifier = input("Enter the target player identifier: ")
            make_move(
                player_identifier,
                action,
                target_player_identifier=target_player_identifier,
            )
        else:
            print("Invalid action. Please try again.")
    elif choice == "3":
        print("Exiting the program.")
        break
    else:
        print("Invalid choice. Please try again.")

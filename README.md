# StrongholdCPP API

This is a C++ API designed to work primarily with the StrongholdCPP game source code, managing game instances, handling move actions, fetching game properties, and loading a game. It leverages the capabilities of the cpprestsdk (Casablanca) to provide a robust and performant API interface.

## Table of Contents

- [Features](#features)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Usage](#usage)

## Features

1. **Starting a Game Instance:** Initialize a new game with preset or custom parameters.

2. **Handling Move Actions:** Manage various move actions such as training villagers and troops, raiding, etc.

3. **Getting Properties:** Fetch necessary game properties to monitor the status of the game.

4. **Loading a Game:** Load a previously saved game instance.

## Dependencies

The StrongholdCPP API depends on cpprestsdk (Casablanca), a Microsoft project for cloud-based client-server communication in native code using a modern asynchronous C++ API design. The installation guide below includes instructions to install cpprestsdk.

## Installation

### Prerequisites

- [CMake](https://cmake.org/) (version 3.9 or higher)
- [cpprestsdk (Casablanca)](https://github.com/microsoft/cpprestsdk)
- [vcpkg](https://github.com/microsoft/vcpkg)

### Installation Steps

1. Install cpprestsdk (Casablanca) and its dependencies using vcpkg:

   ```shell
   vcpkg install cpprestsdk:x64-windows
   ```

2. Clone the repository:

   ```shell
   git clone https://github.com/ILISJAK/strongholdcpp-api.git
   ```

3. Navigate to the project directory:

   ```shell
   cd strongholdcpp-api
   ```

4. Create a build directory and navigate into it:

   ```shell
   mkdir build
   cd build
   ```

5. Configure the project using CMake:

   ```shell
   cmake ..
   ```

6. Build the project:

   ```shell
   cmake --build .
   ```

   This will generate the `StrongholdCPP` executable in the `build/Release` directory.
   
## Usage

To run the `StrongholdCPP` executable:

```shell
./Release/StrongholdCPP
```

This will start the StrongholdCPP server, ready to receive API requests. The API communicates using HTTP requests. Depending on the operation you want to perform, you'll need to use the appropriate API endpoint.

The following PHP example demonstrates how to interact with the API (starting a game):

```php
function start_game($num_players)
{
    global $base_url;
    $start_game_url = $base_url . "/start-game";
    $data = array("numPlayers" => (int) $num_players);
    echo $num_players;
    echo "\nRequest data: ";
    print_r($data); // debug logging

    $curl = curl_init($start_game_url);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($curl, CURLOPT_POST, true);
    curl_setopt($curl, CURLOPT_POSTFIELDS, json_encode($data));
    curl_setopt($curl, CURLOPT_HTTPHEADER, array('Content-Type: application/json'));

    $response = curl_exec($curl);
    if (!$response) {
        die('Error: "' . curl_error($curl) . '" - Code: ' . curl_errno($curl));
    }

    $http_status = curl_getinfo($curl, CURLINFO_HTTP_CODE);
    print_response($http_status, $response);

    curl_close($curl);
}


// Usage example:
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $num_players = $_POST['numPlayers'] ?? null;
    if ($num_players !== null) {
        $_SESSION['numPlayers'] = $_POST['numPlayers'] ?? null;
        start_game($num_players);
    }
}
```
Remember to replace PHP cURL URLs with appropriate StrongholdCPP API endpoints in your application.

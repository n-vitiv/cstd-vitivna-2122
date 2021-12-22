# cstd-vitivna-2122

Subject: CSTD
Group: KI-47
Developer: Nazarii Vitiv

Simple tenis game, implemented at C language.
HW interface - UART
Data driven format - xml

Play modes:
* Man vs AI
* Man vs Man
* AI vs AI

AI supports next complexity levels:
* Random move
* Win strategy

Actions:
* New
* Save
* Load

The proposal for data driven approach: configuration (saved in configuration file or DB):
* board size
* tool size

There was created .config file with fields:
* board_width
* board_height
* tool_size

For full compilation use bash script:
./ci.sh
This script will install all neede libs and will run compilation.
To run game print ./game
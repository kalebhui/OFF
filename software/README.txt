GENERAL

- Server-side (software/server/)
    - To host server (on GCP VM)
        - cd into server directory
        - run the command 'python3 game_server.py'

- Client-side (software/client/)
    - To open client connection (*must use Linux*):
        - cd into client directory
        - compile c code with 'gcc -o client client.c -std=c99 -lpthread'
        - run the command './client'

- Game (software/game/)
    - Used for testing game directly on Windows without server/client/DE1-Soc
        - cd into game directory
        - run the command 'python game.py'

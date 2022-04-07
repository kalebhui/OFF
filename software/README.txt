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

KEYBOARD INPUTS

General:
- r:        reset level
- 0:        level select

player select:
- 1:        select player 1
- 2:        select player 2

level select: 
- 1:        select level 1
- 2:        select level 2
- 3:        select level 1
- 4:        select level 2
- 5:        select level 1
- 6:        select infinite level

player 1 keys:
- w:        jump
- s:        jump downwards 
- a:        move left
- d:        move right

player 2 keys:
- i:        move up
- k:        move down
- j:        move left
- l:        move right
- 1:        select normal block
- 2:        select ice block
- 3:        select trampoline block
- 4:        select gravity block
- space:    place block

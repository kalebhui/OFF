# Used to serve as a python client to view game state from server
# Also allows user to make inputs to server and play as one of the players
# this client in single threaded

import socket            
import pygame
import threading
 
s = socket.socket()
s1 = socket.socket()        
port = 3389              
 
# connect to the server
s.connect(('35.212.170.255', port))
s1.connect(('35.212.170.255', port))
 
pygame.init()
clock = pygame.time.Clock()
screen_width = 320
screen_height = 240
tile_size = 10
status_height = 3
screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption("OFF: Outwit or Fall Flat")

yellow_square = pygame.image.load('images/yellow.png')
red_square = pygame.image.load('images/red.png')
ice_square = pygame.image.load('images/ice.png')
trampoline_square = pygame.image.load('images/trampoline.png')
finish_square = pygame.image.load('images/finish.png')
player_one = pygame.image.load('images/player-one.png')
player_two = pygame.image.load('images/player-two.png')
status_border = pygame.image.load('images/status-border.png')

yellow_square = pygame.transform.scale(yellow_square, (tile_size, tile_size))
red_square = pygame.transform.scale(red_square, (tile_size, tile_size))
ice_square = pygame.transform.scale(ice_square, (tile_size, tile_size))
trampoline_square = pygame.transform.scale(trampoline_square, (tile_size, tile_size))
finish_square = pygame.transform.scale(finish_square, (tile_size, tile_size))
player_one = pygame.transform.scale(player_one, (tile_size, tile_size))
player_two = pygame.transform.scale(player_two, (tile_size, tile_size))
status_border = pygame.transform.scale(status_border, (screen_width, status_height))

def run_game():
    open = True
    # signal that this client should receive data from server
    s.send('0'.encode())
    # signal that this client should receive data from server
    s1.send('1'.encode())
    while open:
        changed = False
        clock.tick(20) # number of frames per sec
        screen.fill((0, 0, 0))
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                open = False

        message = s.recv(10000).decode()
        s.send('0'.encode()) #signal message received
        if message[0] == '.':
            changed = True
            message = message
        message = message.split(',')

        # take input from keyboard and send to server
        key = pygame.key.get_pressed()
        if key[pygame.K_w]:
            s1.send('w'.encode())
        elif key[pygame.K_a]:
            s1.send('a'.encode())
        elif key[pygame.K_s]:
            s1.send('s'.encode())
        elif key[pygame.K_d]:
            s1.send('d'.encode())
        elif key[pygame.K_r]:
            s1.send('r'.encode())
        elif key[pygame.K_1]:
            s1.send('1'.encode())
        elif key[pygame.K_2]:
            s1.send('2'.encode())
        elif key[pygame.K_3]:
            s1.send('3'.encode())
        elif key[pygame.K_SPACE]:
            s1.send('p'.encode())

        # output game state to display
        for i in range(0, len(message) - 1, 3):
            tile = int(message[i + 2])
            if tile == -1:
                img = player_one
            elif tile == -2:
                img = player_two
            elif tile == 0:
                img = status_border
            elif tile == 1:
                img = yellow_square
            elif tile == 2:
                img = red_square
            elif tile == 3:
                img = ice_square
            elif tile == 4:
                img = trampoline_square
            elif tile == 5:
                img = finish_square
            else:
                print("ERRRRRRRRRRRRRRROR")
            try:
                img_rect = img.get_rect()
                img_rect.x = int(message[i]) 
                img_rect.y = int(message[i + 1])
                screen.blit(img, img_rect)
            except:
                print(tile)

        pygame.display.update()

run_game()
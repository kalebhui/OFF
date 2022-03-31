# Used to serve as a python client to view game state from server

import socket            
import pygame
 
s = socket.socket()        
port = 3389              
s.connect(('35.212.170.255', port))
 
pygame.init()
clock = pygame.time.Clock()
screen_width = 320
screen_height = 240
tile_size = 10
screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption("OFF: Outwit or Fall Flat")

yellow_square = pygame.image.load('images/yellow.png')
red_square = pygame.image.load('images/red.png')
ice_square = pygame.image.load('images/ice.png')
trampoline_square = pygame.image.load('images/trampoline.png')
finish_square = pygame.image.load('images/finish.png')
player_one = pygame.image.load('images/player-one.png')
player_two = pygame.image.load('images/player-two.png')

yellow_square = pygame.transform.scale(yellow_square, (tile_size, tile_size))
red_square = pygame.transform.scale(red_square, (tile_size, tile_size))
ice_square = pygame.transform.scale(ice_square, (tile_size, tile_size))
trampoline_square = pygame.transform.scale(trampoline_square, (tile_size, tile_size))
finish_square = pygame.transform.scale(finish_square, (tile_size, tile_size))
player_one = pygame.transform.scale(player_one, (tile_size, tile_size))
player_two = pygame.transform.scale(player_two, (tile_size, tile_size))

#signal that this client should receive data from server
s.send('0'.encode())

open = True
while open:
    clock.tick(20)
    screen.fill((0, 0, 0))
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            open = False

    message = s.recv(1024).decode()
    s.send('0'.encode()) # signal message received
    message = message.split(',')

    # output game state to display
    for i in range(0, len(message) - 1, 3):
        tile = int(message[i + 2])
        if tile == -1:
            img = player_one
        elif tile == -2:
            img = player_two
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
        img_rect = img.get_rect()
        img_rect.x = int(message[i]) 
        img_rect.y = int(message[i + 1])
        screen.blit(img, img_rect)

    pygame.display.update()

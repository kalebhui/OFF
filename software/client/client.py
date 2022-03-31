# Import socket module
import socket            
import pygame
 
# Create a socket object
s = socket.socket()        
 
# Define the port on which you want to connect
port = 3389              
 
# connect to the server on local computer
s.connect(('35.212.170.255', port))
 
pygame.init()
clock = pygame.time.Clock()
screen_width = 320
screen_height = 240
tile_size = 10
screen = pygame.display.set_mode((screen_width, screen_height)) #replace later with code to output to framebuffer #!!!!
pygame.display.set_caption("OFF: Outwit or Fall Flat")


# world = World(bitmap)
# playerOne = Player('images/player-one.png', spawn_coords_p1, 1)
# playerTwo = Player('images/player-two.png', spawn_coords_p2, 2)
# print("line 26")
yellow_square = pygame.image.load('images/yellow.png')
red_square = pygame.image.load('images/red.png')
ice_square = pygame.image.load('images/ice.png')
trampoline_square = pygame.image.load('images/trampoline.png')
finish_square = pygame.image.load('images/finish.png')
player_one = pygame.image.load('images/player-one.png')
player_two = pygame.image.load('images/player-two.png')
# print("line 34")
yellow_square = pygame.transform.scale(yellow_square, (tile_size, tile_size))
red_square = pygame.transform.scale(red_square, (tile_size, tile_size))
ice_square = pygame.transform.scale(ice_square, (tile_size, tile_size))
trampoline_square = pygame.transform.scale(trampoline_square, (tile_size, tile_size))
finish_square = pygame.transform.scale(finish_square, (tile_size, tile_size))
player_one = pygame.transform.scale(player_one, (tile_size, tile_size))
player_two = pygame.transform.scale(player_two, (tile_size, tile_size))
# print("line 42")
open = True
s.send('0'.encode())
while open:
    clock.tick(20) # number of frames per sec
    screen.fill((0, 0, 0)) #background colour #!!!!\
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            open = False
    # print("before receive")
    message = s.recv(1024).decode()
    s.send('0'.encode())
    # print(message)
    message = message.split(',')
    # print(message)
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

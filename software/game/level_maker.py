from math import ceil
from turtle import color
import pygame

pygame.init()
clock = pygame.time.Clock()
screen_width = 800
screen_height = 480
tile_size = 20
spawn_coords_p2 = (20 * tile_size, 5 * tile_size)
screen = pygame.display.set_mode((screen_width, screen_height)) #replace later with code to output to framebuffer
pygame.display.set_caption("Outwit or Fall Flat: LEVEL EDITOR")

#empty map setting
world_data = []
for row in range(24):
	r = [0] * 40
	world_data.append(r)




class World():
    def __init__(self, data):
        #load images
        self.yellow_square = pygame.image.load('images/yellow.png')
        self.red_square = pygame.image.load('images/red.png')
        self.ice_square = pygame.image.load('images/ice.png')
        self.trampoline_square = pygame.image.load('images/trampoline.png')
        self.finish_square = pygame.image.load('images/finish.png')


    def update(self):
        key = pygame.key.get_pressed()
        if key[pygame.K_SPACE]: # place block
            world_data[playerTwo.rect.y // tile_size][playerTwo.rect.x // tile_size] = playerTwo.block_type # add new block to world data
        if key[pygame.K_r]: # reset to default
            for row in range(24):
                for col in range(40):
                    world_data[row][col] = 0
        for row in range(24):
            for col in range(40):
                if(world_data[row][col] == 1): #normal square
                    img = pygame.transform.scale(self.yellow_square, (tile_size, tile_size))
                    screen.blit(img, (col * tile_size, row * tile_size)) # draw each tile
                if(world_data[row][col] == 2): #starting square
                    img = pygame.transform.scale(self.red_square, (tile_size, tile_size))
                    screen.blit(img, (col * tile_size, row * tile_size)) # draw each tile
                elif(world_data[row][col] == 3): #ice square
                    img = pygame.transform.scale(self.ice_square, (tile_size, tile_size))
                    screen.blit(img, (col * tile_size, row * tile_size)) # draw each tile
                elif(world_data[row][col] == 4): #trampoline square
                    img = pygame.transform.scale(self.trampoline_square, (tile_size, tile_size))
                    screen.blit(img, (col * tile_size, row * tile_size)) # draw each tile
                elif(world_data[row][col] == 5): #finish square
                    img = pygame.transform.scale(self.finish_square, (tile_size, tile_size))
                    screen.blit(img, (col * tile_size, row * tile_size)) # draw each tile

class Player():
    def __init__(self, image_path, coordinate):
        self.playerImg =  pygame.transform.scale(pygame.image.load(image_path), (tile_size, tile_size))
        self.rect = self.playerImg.get_rect()
        self.rect.x = coordinate[0] # initial player x location
        self.rect.y = coordinate[1] # initial player y location
        self.width = self.playerImg.get_width()
        self.height = self.playerImg.get_height()
        self.vel_y = 0
        self.vel_x = 0
        self.enter_pressed = False
        self.spawn_coords = coordinate # remember starting coordinates for respawn
        self.block_type = 1 #default block type

    def update(self):
        change_x = 0
        change_y = 0
        # if keystroke is pressed check whether its right or left
        key = pygame.key.get_pressed()
        if key[pygame.K_i]:
            change_y -= tile_size
        if key[pygame.K_k]:
           change_y += tile_size
        if key[pygame.K_j]:
            change_x -= tile_size
        if key[pygame.K_l]:
            change_x += tile_size
        if key[pygame.K_1]:
            self.block_type = 1
        if key[pygame.K_2]:
            self.block_type = 3    
        if key[pygame.K_3]:
            self.block_type = 2         
        if key[pygame.K_4]:
            self.block_type = 4 
        if key[pygame.K_5]:
            self.block_type = 5
        if key[pygame.K_RETURN]:
            print(world_data)


        self.rect.x += change_x
        if self.rect.left <= 0:
            self.rect.left = 0
        elif self.rect.right >= screen_width:
            self.rect.right = screen_width
        self.rect.y += change_y
        if self.rect.top <= 0:
            self.rect.top = 0
        elif self.rect.bottom >= screen_height:
            self.rect.bottom = screen_height


        if(self.block_type == 1): #normal square
            self.playerImg = pygame.transform.scale(world.yellow_square, (tile_size, tile_size))
        if(self.block_type == 2): #starting square
            self.playerImg = pygame.transform.scale(world.red_square, (tile_size, tile_size))
        elif(self.block_type == 3): #ice square
            self.playerImg = pygame.transform.scale(world.ice_square, (tile_size, tile_size))
        elif(self.block_type == 4): #trampoline square
            self.playerImg = pygame.transform.scale(world.trampoline_square, (tile_size, tile_size))
        elif(self.block_type == 5): #finish square
            self.playerImg = pygame.transform.scale(world.finish_square, (tile_size, tile_size))

        # draw player at current location
        screen.blit(self.playerImg, self.rect)

world = World(world_data)
playerTwo = Player('images/player-two.png', spawn_coords_p2)

# game loop, will continue until quit
open = True
while open:
    clock.tick(20) # number of frames per sec
    screen.fill((0, 0, 0)) #background colour
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            open = False

    playerTwo.update()
    world.update()

    pygame.display.update()

pygame.quit()
quit()
from math import ceil
from turtle import color
import pygame

pygame.init()
clock = pygame.time.Clock()
screen_width = 800
screen_height = 480
tile_size = 20
spawn_coords_p1 = (tile_size, screen_height - 140)
spawn_coords_p2 = (20 * tile_size, 5 * tile_size)
screen = pygame.display.set_mode((screen_width, screen_height)) #replace later with code to output to framebuffer
pygame.display.set_caption("OFF: Outwit or Fall Flat")

#default map setting
bitmap = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

text_font = pygame.font.SysFont('Bauhaus 93', 30)
white = (255, 255, 255)

def draw_text(text, font, text_col, x, y):
    img = font.render(text, True, text_col)
    screen.blit(img, (x, y))



class World():
    def __init__(self, data):
        self.default_tile_list = []

        #load images
        self.start_time = pygame.time.get_ticks()
        self.finish_time = -1
        self.level_completed = False
        self.blocks_placed = 0
        self.yellow_square = pygame.image.load('images/yellow.png')
        self.red_square = pygame.image.load('images/red.png')
        self.ice_square = pygame.image.load('images/ice.png')
        self.trampoline_square = pygame.image.load('images/trampoline.png')
        self.finish_square = pygame.image.load('images/finish.png')

        row_count = 0
        for row in data:
            col_count = 0
            for tile in row:
                if tile != 0:
                    if tile == 1:
                        img = pygame.transform.scale(self.yellow_square, (tile_size, tile_size))
                    elif tile == 2:
                        img = pygame.transform.scale(self.red_square, (tile_size, tile_size))
                    elif tile == 3:
                        img = pygame.transform.scale(self.ice_square, (tile_size, tile_size))
                    elif tile == 4:
                        img = pygame.transform.scale(self.trampoline_square, (tile_size, tile_size))
                    elif tile == 5:
                        img = pygame.transform.scale(self.finish_square, (tile_size, tile_size))
                    img_rect = img.get_rect()
                    img_rect.x = col_count * tile_size
                    img_rect.y = row_count * tile_size
                    tile = (img, img_rect, tile)
                    self.default_tile_list.append(tile)
                col_count += 1
            row_count += 1
        self.tile_list = self.default_tile_list.copy()

    def update(self):
        draw_text(str((pygame.time.get_ticks() - self.start_time) / 1000), text_font, white, tile_size - 10, 10) #timer for level

        #check if p1 finished the level
        for tile in world.tile_list:
            if tile[1].colliderect(playerOne.rect.x, playerOne.rect.y + 1, playerOne.width, playerOne.height):
                if tile[2] == 5:
                    self.finish_time = pygame.time.get_ticks() - self.start_time
                    self.level_completed = True

        key = pygame.key.get_pressed()
        if key[pygame.K_SPACE]: # place block
            if not playerTwo.in_block(): #makes sure you can't place blocks over each other. maybe not necessary?
                if(playerTwo.block_type == 1): #normal square
                    img = pygame.transform.scale(self.yellow_square, (tile_size, tile_size))
                elif(playerTwo.block_type == 3): #ice square
                    img = pygame.transform.scale(self.ice_square, (tile_size, tile_size))
                elif(playerTwo.block_type == 4): #trampoline square
                    img = pygame.transform.scale(self.trampoline_square, (tile_size, tile_size))
                img_rect = img.get_rect()
                img_rect.x = playerTwo.rect.x
                img_rect.y = playerTwo.rect.y
                self.tile_list.append((img, img_rect, playerTwo.block_type)) # add new block to tile list
                self.blocks_placed += 1
        if key[pygame.K_r]: # reset to default
            self.tile_list = self.default_tile_list.copy()
        for tile in self.tile_list:
            screen.blit(tile[0], tile[1]) # draw each tile

class Player():
    def __init__(self, image_path, coordinate, playerNumber):
        self.playerImg =  pygame.transform.scale(pygame.image.load(image_path), (tile_size, tile_size))
        self.rect = self.playerImg.get_rect()
        self.rect.x = coordinate[0] # initial player x location
        self.rect.y = coordinate[1] # initial player y location
        self.number = playerNumber # player number, determines if player can place blocks or not
        self.width = self.playerImg.get_width()
        self.height = self.playerImg.get_height()
        self.vel_y = 0
        self.vel_x = 0
        self.jumped = False
        self.spawn_coords = coordinate # remember starting coordinates for respawn
        self.block_type = 1 #default block type

    #checks if the player is in a block
    def in_block(self):
        for tile in world.tile_list:
            if tile[1].colliderect(self.rect):
                return True
        return False

    def update(self):
        if (self.number == 1): #update p1
            change_x = 0
            change_y = 0

            onBlock = False
            onIce = False
            onTrampoline = False

            for tile in world.tile_list:
                if tile[1].colliderect(self.rect.x, self.rect.y + 1, self.width, self.height):
                    onBlock = True #p1 is standing on a block
                    if tile[2] == 3:
                        onIce = True #p1 is standing on ice
                    elif tile[2] == 4:
                        onTrampoline = True

            # if keystroke is pressed check whether its right or left
            key = pygame.key.get_pressed()

            if key[pygame.K_w]:
                if onTrampoline:
                    self.vel_y = -17
                elif onBlock:
                    self.vel_y = -12

            if key[pygame.K_a]:
                if onIce:
                   self.vel_x = max(self.vel_x - 1, -15) #max ice speed
                   change_x -= 7
                else:
                    change_x -= 10 

            if key[pygame.K_d]:
                if onIce:
                   self.vel_x = min(self.vel_x + 1, 15) #max ice speed
                   change_x += 7
                else:
                    change_x += 10 

            if onBlock and not onIce:
                self.vel_x = 0 #shouldn't continue sliding if we are not on ice

            if self.vel_x > 0:
                self.vel_x = max(self.vel_x - 0.1, 0) #friction of ice

            if self.vel_x < 0:
                self.vel_x = min(self.vel_x + 0.1, 0) #friction of ice
            
            change_x += self.vel_x

            if change_x > 15:
                change_x = 15
            elif change_x < -15:
                change_x = -15

            self.vel_y += 1
            if self.vel_y > 12:
                self.vel_y = 12
            change_y += self.vel_y

            #collision detection with the other blocks
            for tile in world.tile_list:
                if tile[1].colliderect(self.rect.x + change_x, self.rect.y, self.width, self.height):
                    change_x = 0
                if tile[1].colliderect(self.rect.x, self.rect.y + change_y, self.width, self.height):
                    if self.vel_y < 0:
                        change_y = tile[1].bottom - self.rect.top
                        self.vel_y = 0
                    elif self.vel_y > 0:
                        change_y = tile[1].top - self.rect.bottom
                        self.vel_y = 0
            
            self.rect.x += change_x
            if self.rect.left <= 0:
                self.rect.left = 0
            elif self.rect.right >= screen_width:
                self.rect.right = screen_width

            self.rect.y += change_y
            if self.rect.top <= 0:
                self.rect.top = 0
                self.vel_y = 0
            elif self.rect.bottom >= screen_height: #respawn when touch bottom of screen
                self.rect.x = self.spawn_coords[0]
                self.rect.y = self.spawn_coords[1]
        
        elif (self.number == 2): #update p2
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
                self.block_type = 4         
            
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

        # draw player at current location
        screen.blit(self.playerImg, self.rect)

world = World(bitmap)
playerOne = Player('images/player-one.png', spawn_coords_p1, 1)
playerTwo = Player('images/player-two.png', spawn_coords_p2, 2)

# game loop, will continue until quit
open = True
while open:
    clock.tick(20) # number of frames per sec
    screen.fill((0, 0, 0)) #background colour
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            open = False

    if world.level_completed:
        draw_text(f"Congratulations! You have completed the level in {(world.finish_time - world.start_time) / 1000}s, with {world.blocks_placed} blocks placed!", 
        text_font, white, screen_width // 2 - 395, screen_height // 2)
    #update blits of all objects
    else:
        world.update()
        playerOne.update()
        playerTwo.update()

    pygame.display.update()

pygame.quit()
quit()
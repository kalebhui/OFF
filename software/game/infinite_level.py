from ast import In
from math import ceil
from turtle import color
import time
import pygame
import bitmaps

pygame.init()
clock = pygame.time.Clock()

# Dimension setting
tile_size = 10
status_tile_size = 10
bar_height = tile_size // 3
screen_width = 320
screen_height = 240
game_height = screen_height - 3 * tile_size - bar_height

# Spawn settings
spawn_coords_p1 = (tile_size, screen_height - 140)
spawn_coords_p2 = (20 * tile_size, 5 * tile_size)

# Movement settings
reg_change_x = 7
reg_change_y = 6
tramp_change_y = 9
ice_change_x = 5 
ice_max_change_x = 3
gravity = 0.5

# Level settings
reg_blk_count = 100
ice_blk_count = 100
tramp_blk_count = 100

avail_blocks = {} #store counts in hashmap
if (reg_blk_count):
    avail_blocks[1] = reg_blk_count
if (ice_blk_count):
    avail_blocks[3] = ice_blk_count
if (tramp_blk_count):
    avail_blocks[4] = tramp_blk_count

#infinite level settings
screen_shift_default = 1
infinite_count_default = 0
infinite_divisor_default = 10

#default map setting
screen = pygame.display.set_mode((screen_width, screen_height)) #replace later with code to output to framebuffer
pygame.display.set_caption("OFF: Outwit or Fall Flat")

bitmap = [
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    [2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
]

text_font = pygame.font.SysFont('Bauhaus 93', 24)
white = (255, 255, 255)

def draw_text(text, font, text_col, x, y):
    img = font.render(text, True, text_col)
    screen.blit(img, (x, y))

PLAYER_SELECT = 0
LEVEL_SELECT = 1
GAMEPLAY = 2
INFINITE = 11

class Menu():
    def __init__(self, player_select, level_select):
        self.menu_tile_size = tile_size / 2 #menu screens were created with this tile size
        self.yellow_square = pygame.image.load('images/yellow.png')
        self.red_square = pygame.image.load('images/red.png')
        self.ice_square = pygame.image.load('images/ice.png')
        self.trampoline_square = pygame.image.load('images/trampoline.png')
        self.finish_square = pygame.image.load('images/finish.png')
        self.scaled_finish_square = pygame.transform.scale(self.finish_square, (self.menu_tile_size, self.menu_tile_size)) #useful for converting tiles to green
        self.player_select_tile_list = create_tile_list(self, player_select, self.menu_tile_size, self.menu_tile_size)
        self.level_select_tile_list = create_tile_list(self, level_select, self.menu_tile_size, self.menu_tile_size)
        self.current_menu_screen = PLAYER_SELECT
        self.player_one_connect = False
        self.player_two_connect = False

    def update(self):
        key = pygame.key.get_pressed()
        if self.current_menu_screen == PLAYER_SELECT:
            if self.player_one_connect and self.player_two_connect:
                time.sleep(0.5) #to allow both P1 and P2 to turn green before going to the next screen
                self.current_menu_screen = LEVEL_SELECT
            elif key[pygame.K_1]:
                self.convert_red_to_green_tiles(0, screen_height, screen_width // 2) #turns the red tiles to green so there is an indication that p1 has connected
                self.player_one_connect = True
            elif key[pygame.K_2]:
                self.convert_red_to_green_tiles(screen_width // 2, screen_height, screen_width // 2) #turns the red tiles to green so there is an indication that p2 has connected
                self.player_two_connect = True
            for tile in self.player_select_tile_list:
                screen.blit(tile[0], tile[1])
        elif self.current_menu_screen == LEVEL_SELECT:
            if key[pygame.K_1]:
                self.current_menu_screen = GAMEPLAY
                world = World(bitmap, 1) #start at level 1
            elif key[pygame.K_i]:
                self.current_menu_screen = GAMEPLAY
                world = World(bitmaps.infinite_bitmap, INFINITE) #start at level 1
            for tile in self.level_select_tile_list:
                screen.blit(tile[0], tile[1])

    def convert_red_to_green_tiles(self, x, y, size):
        if self.current_menu_screen == PLAYER_SELECT:
            for i in range(len(self.player_select_tile_list)):
                tile = self.player_select_tile_list[i]
                if tile[2] == 2: #if it is a red tile
                    if tile[1].x > x and tile[1].x <= x + size and tile[1].y < y and tile[1].y > y - size: #if it is range
                        self.player_select_tile_list[i] = (self.scaled_finish_square, tile[1], 5) #convert it to a green square
        elif self.current_menu_screen == LEVEL_SELECT:
            for i in range(len(self.level_select_tile_list)):
                tile = self.level_select_tile_list[i]
                if tile[2] == 2: #if it is a red tile
                    if tile[1].x > x and tile.x <= x + size and tile[1].y > y and tile[1].y <= y + size: #if it is range
                        self.level_select_tile_list[i] = (self.scaled_finish_square, tile[1], 5) #convert it to a green square

def create_tile_list(self, data, size_x, size_y):
        tile_list = []
        for row in range(len(data)):
            for col in range(len(data[0])):
                tile = data[row][col]
                if tile != 0:
                    if tile == 1:
                        img = pygame.transform.scale(self.yellow_square, (size_x, size_y))
                    elif tile == 2:
                        img = pygame.transform.scale(self.red_square, (size_x, size_y))
                    elif tile == 3:
                        img = pygame.transform.scale(self.ice_square, (size_x, size_y))
                    elif tile == 4:
                        img = pygame.transform.scale(self.trampoline_square, (size_x, size_y))
                    elif tile == 5:
                        img = pygame.transform.scale(self.finish_square, (size_x, size_y))
                    img_rect = img.get_rect()
                    img_rect.x = col * size_x
                    img_rect.y = row * size_y
                    tile = (img, img_rect, tile)
                    tile_list.append(tile)
        return tile_list

def create_status_bar(self, data, size_x, size_y):
    offset = 0
    tile_list = []
    for key in data:
        if key == 1:
            img = pygame.transform.scale(self.yellow_square, (size_x, size_y))
        elif key == 2:
            img = pygame.transform.scale(self.red_square, (size_x, size_y))
        elif key == 3:
            img = pygame.transform.scale(self.ice_square, (size_x, size_y))
        elif key == 4:
            img = pygame.transform.scale(self.trampoline_square, (size_x, size_y))
        elif key == 5:
            img = pygame.transform.scale(self.finish_square, (size_x, size_y))
        img_rect = img.get_rect()
        # space out blocks horizontally
        img_rect.x = size_x * (offset * 3 + 2)
        # center the blocks vertically
        img_rect.centery = game_height + (screen_height - game_height) // 2 + bar_height 
        block = (img, img_rect, key)
        tile_list.append(block)
        offset += 1

    # draw the horizontal line accross the screen
    img = pygame.transform.scale(self.status_border, (screen_width, bar_height))
    img_rect = img.get_rect()
    img_rect.x = 0
    img_rect.y = game_height
    tile_list.append((img, img_rect, -1))

    return tile_list

class World():
    def __init__(self, data, level):
        self.default_tile_list = []
        self.default_status_tile_list =[]
        self.start_time = pygame.time.get_ticks()
        self.finish_time = -1
        self.level_completed = False
        self.blocks_placed = 0
        self.level = level
        self.camera = InfiniteCamera()

        #load images
        self.yellow_square = pygame.image.load('images/yellow.png')
        self.red_square = pygame.image.load('images/red.png')
        self.ice_square = pygame.image.load('images/ice.png')
        self.trampoline_square = pygame.image.load('images/trampoline.png')
        self.finish_square = pygame.image.load('images/finish.png')
        self.status_border = pygame.image.load('images/status-border.png')
        self.default_tile_list = create_tile_list(self, data, tile_size, tile_size) + create_status_bar(self, avail_blocks, status_tile_size, status_tile_size)
        self.default_avail_blocks = avail_blocks.copy()
        self.avail_blocks = avail_blocks.copy()
        self.tile_list = self.default_tile_list.copy()

    def update(self):
        draw_text(str((pygame.time.get_ticks() - self.start_time) / 1000), text_font, white, tile_size - 10, 10) #timer for level
        self.camera.update()
        # draw block counts
        offset = 0
        for key in self.avail_blocks:
            xcoord = status_tile_size * (offset * 3 + 2)
            ycoord = game_height + (screen_height - game_height) // 2 + bar_height
            draw_text(str(self.avail_blocks[key]), text_font, white, xcoord, ycoord)
            offset += 1
        
        #check if p1 finished the level
        for tile in world.tile_list:
            if tile[1].colliderect(playerOne.rect.x, playerOne.rect.y + 1, playerOne.width, playerOne.height):
                if tile[2] == 5:
                    self.finish_time = pygame.time.get_ticks() - self.start_time
                    self.level_completed = True

        key = pygame.key.get_pressed()
        empty = 0
        if key[pygame.K_SPACE]: # place block
            if not playerTwo.in_block(): #makes sure you can't place blocks over each other. maybe not necessary?
                if(playerTwo.block_type == 1): #normal square
                    img = pygame.transform.scale(self.yellow_square, (tile_size, tile_size))
                    if (self.avail_blocks[playerTwo.block_type] == 0):
                        empty = 1
                    else:
                        self.avail_blocks[playerTwo.block_type] -= 1
                elif(playerTwo.block_type == 3): #ice square
                    img = pygame.transform.scale(self.ice_square, (tile_size, tile_size))
                    if (self.avail_blocks[playerTwo.block_type] == 0):
                        empty = 1
                    else:
                        self.avail_blocks[playerTwo.block_type] -= 1
                elif(playerTwo.block_type == 4): #trampoline square
                    img = pygame.transform.scale(self.trampoline_square, (tile_size, tile_size))
                    if (self.avail_blocks[playerTwo.block_type] == 0):
                        empty = 1
                    else:
                        self.avail_blocks[playerTwo.block_type] -= 1
                
                #only place blocks if there are any left
                if not empty:
                    img_rect = img.get_rect()
                    img_rect.x = playerTwo.rect.x
                    img_rect.y = playerTwo.rect.y
                    self.tile_list.append((img, img_rect, playerTwo.block_type)) # add new block to tile list
                    self.blocks_placed += 1

        if key[pygame.K_r]: # reset to default
            self.tile_list = self.default_tile_list.copy()
            self.avail_blocks = self.default_avail_blocks.copy()

        # #infinite level logic
        # if self.infinite_count % self.infinite_divisor == 0:
        #     for tile in self.tile_list:
        #         if (tile[2] != -1): #check if bar
        #             tile[1].y += self.screen_shift
        #             # print(tile)
        # self.infinite_count += 1
        # if self.infinite_count % 100 == 0:
        #     if self.infinite_divisor > 2:
        #         self.infinite_divisor -= 1
        #     elif self.screen_shift < 5:
        #         self.screen_shift += 1
        
        index = 0
        for tile in self.tile_list:
            if tile[2] == -1: #to keep the status bar in view
                screen.blit(tile[0], tile[1])
            elif tile[1].y > game_height: #if the block is part of the status bar
                screen.blit(tile[0], tile[1])
            elif self.camera.onScreen(tile[1]): #if the tile is within camera view
                    camera_adjusted_rect = tile[1].copy()
                    camera_adjusted_rect.y = tile[1].y - self.camera.bottom_y #readjust its x coordinate so its within screen boundaries
                    screen.blit(tile[0], camera_adjusted_rect) # draw each tile
            else:
                self.tile_list.pop(index)
            # print(self.tile_list)
            index += 1
        

class InfiniteCamera():
    def __init__(self):
        self.bottom_y = 0
        self.infinite_count = infinite_count_default
        self.infinite_divisor = infinite_divisor_default
        self.screen_shift = screen_shift_default
    
    #takes in a tile which has a x and y coord and returns whether they are on screen
    def onScreen(self, tile):
        if tile.bottom >= self.bottom_y: # !!!!
            return True
        return False

    def update(self):
        # if self.infinite_count % self.infinite_divisor == 0:
        self.bottom_y -= 0.5
        # self.infinite_count += 1
        # if self.infinite_count % 100 == 0:
        #     if self.infinite_divisor > 2:
        #         self.infinite_divisor -= 1
        #     elif self.screen_shift < 5:
        #         self.screen_shift += 1
            
        # self.leftmost_x = playerOne.rect.x - screen_width // 2 #camera is relative to player one position
        # if self.leftmost_x < 0:
        #     self.leftmost_x = 0
        # elif self.leftmost_x > self.max_x - screen_width:
        #     self.leftmost_x = self.max_x - screen_width

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
                    self.vel_y = -tramp_change_y
                elif onBlock:
                    self.vel_y = -reg_change_y
            if key[pygame.K_a]:
                if onIce:
                    self.vel_x = max(self.vel_x - 1, -reg_change_x) #max ice speed
                    change_x -= ice_max_change_x
                else:
                    change_x -= ice_change_x 

            if key[pygame.K_d]:
                if onIce:
                    self.vel_x = min(self.vel_x + 1, reg_change_x) #max ice speed
                    change_x += ice_max_change_x
                else:
                    change_x += ice_change_x 

            if onBlock and not onIce:
                self.vel_x = 0 #shouldn't continue sliding if we are not on ice

            if self.vel_x > 0:
                self.vel_x = max(self.vel_x - 0.1, 0) #friction of ice

            if self.vel_x < 0:
                self.vel_x = min(self.vel_x + 0.1, 0) #friction of ice
            
            change_x += self.vel_x

            if change_x > reg_change_x:
                change_x = reg_change_x
            elif change_x < -reg_change_x:
                change_x = -reg_change_x

            self.vel_y += gravity
            if self.vel_y > reg_change_y:
                self.vel_y = reg_change_y
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
            if self.rect.top <= world.camera.bottom_y:
                self.rect.top = world.camera.bottom_y
                self.vel_y = gravity
            elif self.rect.bottom >= game_height + world.camera.bottom_y: #respawn when touch bottom of screen
                self.rect.x = self.spawn_coords[0]
                self.rect.y = self.spawn_coords[1] + world.camera.bottom_y
        
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
            if self.rect.top <= 0 + world.camera.bottom_y:
                self.rect.top = 0 + world.camera.bottom_y
            elif self.rect.bottom >= game_height + world.camera.bottom_y:
                self.rect.bottom = game_height + world.camera.bottom_y

        if False:
            screen.blit(self.playerImg, self.rect)
        else:
            camera_adjusted_rect = self.rect.copy()
            camera_adjusted_rect.y = -(world.camera.bottom_y - self.rect.y)
            # draw player at current location
            screen.blit(self.playerImg, camera_adjusted_rect)
            # draw player at current location
            #screen.blit(self.playerImg, self.rect)

menu = Menu(bitmaps.player_select, bitmaps.level_select)
world = World(bitmap, 1)
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

    if menu.current_menu_screen != GAMEPLAY:
        menu.update()

    elif world.level_completed:
        #rewrite to fit screen
        draw_text(f"Congratulations! You have completed the level in {(world.finish_time - world.start_time) / 1000}s, with {world.blocks_placed} blocks placed!", 
        text_font, white, screen_width // 2 - 300, screen_height // 2)
        
    #update blits of all objects
    else:
        world.update()
        playerOne.update()
        playerTwo.update()

    pygame.display.update()

pygame.quit()
quit()
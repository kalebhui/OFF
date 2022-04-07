import socket
import threading
import time
import sys
import pygame
import random
from collections import deque
import bitmaps
import os

os.environ["SDL_VIDEODRIVER"] = "dummy" #!!!! delete might be making the server slower

SERVER = ''
PORT = 3389
FORMAT = "utf-8"
DISCONNECT_MESSAGE = "quit"
clients_arr = []
max_threads = 4 # Final product clients will require 2 threads each
changed = False
player_change = False
player_one_inputs = deque() #stores user inputs
player_two_inputs = deque() #stores user inputs

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((SERVER, PORT))

pygame.init()
clock = pygame.time.Clock()

# constant definitions
PLAYER_SELECT = 0
LEVEL_SELECT = 1
GAMEPLAY = 2
FINISHED = 3
COMPLETE_MENU = 4
MENUOFFSET = 100
INFINITE = 6

PLAYER1 = -1
PLAYER2 = -2
STATUSBAR = 0
TILE_REG  = 1
TILE_RED  = 2
TILE_ICE  = 3
TILE_TRAMP  = 4
TILE_FINISH  = 5
TILE_GRAV = 6
TILE_BOMB = 7

FONT_HEIGHT = 24
FONT_WIDTH = 16

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

levels = bitmaps.levels #stores bitmaps for levels

screen = pygame.display.set_mode((screen_width, screen_height))

avail_blocks = bitmaps.avail_blocks
spawn_rates = bitmaps.spawn_rates

#infinite level settings
screen_shift_default = 1
infinite_count_default = 0
infinite_divisor_default = 5

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
        self.post_mode = 0

    def update(self, key_p1, key_p2):
        global world #allows us to access the world variable in this function
        global changed
        if self.current_menu_screen == PLAYER_SELECT:
            if self.player_one_connect and self.player_two_connect:
                pygame.time.delay(500) #to allow both P1 and P2 to turn green before going to the next screen. ALSO can potentially crash pygame??
                player_one_inputs.clear()
                player_two_inputs.clear()
                self.current_menu_screen = LEVEL_SELECT
                changed = True
            elif key_p1 == '1':
                self.convert_red_to_green_tiles(0, screen_height, screen_width // 2) #turns the red tiles to green so there is an indication that p1 has connected
                self.player_one_connect = True
            elif key_p2 == '2':
                self.convert_red_to_green_tiles(screen_width // 2, screen_height, screen_width // 2) #turns the red tiles to green so there is an indication that p2 has connected
                self.player_two_connect = True
        elif self.current_menu_screen == LEVEL_SELECT:
            playerOne.reset()
            playerTwo.reset()
            if key_p1 == '1' or key_p2 == '1':
                world = World(levels[1], 1) #intializes world with proper level, ndex 0 is garbage value
                self.current_menu_screen = GAMEPLAY
                changed = True
            elif key_p1 == '2' or key_p2 == '2':
                world = World(levels[2], 2)
                self.current_menu_screen = GAMEPLAY
                changed = True
            elif key_p1 == '3' or key_p2 == '3':
                world = World(levels[3], 3)
                self.current_menu_screen = GAMEPLAY
                changed = True
            elif key_p1 == '4' or key_p2 == '4':
                world = World(levels[4], 4)
                self.current_menu_screen = GAMEPLAY
                changed = True
            elif key_p1 == '5' or key_p2 == '5':
                world = World(levels[5], 5)
                self.current_menu_screen = GAMEPLAY
                changed = True
            elif key_p1 == '6' or key_p2 == '6':
                world = World(levels[6], INFINITE)
                self.current_menu_screen = GAMEPLAY
                changed = True
        if self.current_menu_screen == COMPLETE_MENU:
            if key_p1 == '1' or key_p2 == '1':
                self.post_mode = 1
                pygame.time.delay(500)
                world = World(levels[world.level], world.level) #recreate world with same bitmap
                playerOne.reset() #respawn player one
                playerTwo.reset() #respawn player two
                self.current_menu_screen = GAMEPLAY
                changed = True
            elif key_p1 == '2' or key_p2 == '2':
                self.post_mode = 2
                pygame.time.delay(500)
                world = World(levels[world.level + 1], world.level + 1) #recreate world with next level bitmap
                playerOne.reset() #respawn player one
                playerTwo.reset() #respawn player two
                self.current_menu_screen = GAMEPLAY
                changed = True
            elif key_p1 == '3' or key_p2 == '3':
                self.post_mode = 3
                pygame.time.delay(500)
                self.current_menu_screen = LEVEL_SELECT
                changed = True
            self.post_mode = 0

    def convert_red_to_green_tiles(self, x, y, size):
        if self.current_menu_screen == PLAYER_SELECT:
            for i in range(len(self.player_select_tile_list)):
                tile = self.player_select_tile_list[i]
                if tile[2] == TILE_RED: #if it is a red tile
                    if tile[1].x > x and tile[1].x <= x + size and tile[1].y < y and tile[1].y > y - size: #if it is range
                        self.player_select_tile_list[i] = (self.scaled_finish_square, tile[1], 5) #convert it to a green square

        elif self.current_menu_screen == LEVEL_SELECT:
            for i in range(len(self.level_select_tile_list)):
                tile = self.level_select_tile_list[i]
                if tile[2] == TILE_RED: #if it is a red tile
                    if tile[1].x > x and tile.x <= x + size and tile[1].y > y and tile[1].y <= y + size: #if it is range
                        self.level_select_tile_list[i] = (self.scaled_finish_square, tile[1], 5) #convert it to a green square


class Camera():
    def __init__(self, level, level_width = None):
        self.leftmost_x = 0
        self.bottom_y = 0
        self.min_x = 0
        if level_width == None:
            level_bitmap = levels[level]
            self.max_x = len(level_bitmap[0]) * tile_size
        else:
            self.max_x = level_width
    
    #takes in a tile which has a x and y coord and returns whether they are on screen
    def onScreen(self, tile):
        if tile.left >= self.leftmost_x and tile.right <= self.leftmost_x + screen_width:
            return True
        return False

    def update(self, playerOne):
        global changed
        old_camera = self.leftmost_x
        self.leftmost_x = max(self.min_x, playerOne.rect.x - screen_width // 2) #camera is relative to player one position
        
        if self.leftmost_x > self.max_x - screen_width:
            self.leftmost_x = self.max_x - screen_width

        if old_camera != self.leftmost_x:
            changed = True

class InfiniteCamera():
    def __init__(self):
        self.bottom_y = 0
        self.leftmost_x = 0
        self.infinite_count = infinite_count_default
        self.infinite_divisor = infinite_divisor_default
        self.screen_shift = screen_shift_default
    
    #takes in a tile which has a x and y coord and returns whether they are on screen
    def onScreen(self, tile):
        return tile.bottom <= (self.bottom_y + game_height)

    def update(self):
        global changed
        old_camera = self.bottom_y

        # this code will gradually speed up the falling process
        if self.infinite_count % self.infinite_divisor == 0:
            self.bottom_y -= self.screen_shift
            if self.infinite_count % 50 == 0:
                if self.infinite_divisor > 2:
                    self.infinite_divisor -= 1
                elif self.screen_shift < 2:
                    self.screen_shift += 0.5
        self.infinite_count += 1

        if old_camera != self.bottom_y:
            changed = True

class World():
    def __init__(self, data, level):
        self.default_tile_list = []
        self.default_status_tile_list =[]
        self.start_time = pygame.time.get_ticks()
        self.finish_time = -1
        self.level_completed = False
        self.blocks_placed = 0
        self.level = level
        self.enemy_spawn_rate = spawn_rates[level]

        if (level == INFINITE):
            self.camera = InfiniteCamera()
        else:
            self.camera = Camera(level)

        #load images
        self.yellow_square = pygame.image.load('images/yellow.png')
        self.red_square = pygame.image.load('images/red.png')
        self.ice_square = pygame.image.load('images/ice.png')
        self.trampoline_square = pygame.image.load('images/trampoline.png')
        self.finish_square = pygame.image.load('images/finish.png')
        self.status_border = pygame.image.load('images/status-border.png')
        self.gravity_square = pygame.image.load('images/gravity_block.png')
        self.default_tile_list = create_tile_list(self, data, tile_size, tile_size) + create_status_bar(self, avail_blocks[level], status_tile_size, status_tile_size,level)
        self.default_avail_blocks = avail_blocks[level].copy()
        self.avail_blocks = avail_blocks[level].copy()
        self.tile_list = self.default_tile_list.copy()

        pygame.sprite.Group.empty(enemy_group)

    def update(self, key_p1, key_p2):
        global changed
        if self.level == INFINITE:
            self.camera.update()
        else:
            self.camera.update(playerOne) #update camera relative to p1
        if random.random() < self.enemy_spawn_rate:
            random_x = random.randint(self.camera.leftmost_x + 3 * tile_size, self.camera.leftmost_x + screen_width - 3 * tile_size)
            enemy_group.add(Enemy(random_x, 0))
        
        #check if p1 finished the level
        for tile in world.tile_list:
            if tile[1].colliderect(playerOne.rect.x, playerOne.rect.y + 1, playerOne.width, playerOne.height) or tile[1].colliderect(playerOne.rect.x, playerOne.rect.y - 1, playerOne.width, playerOne.height):
                if tile[2] == 5:
                    self.finish_time = pygame.time.get_ticks() - self.start_time
                    self.level_completed = True

        empty = 0
        if key_p2 == 'p': # place block
            if not playerTwo.in_block(): #makes sure you can't place blocks over each other. maybe not necessary?
                if playerTwo.block_type in self.avail_blocks:
                    if(playerTwo.block_type == TILE_REG):
                        img = pygame.transform.scale(self.yellow_square, (tile_size, tile_size))
                        if (self.avail_blocks[playerTwo.block_type] == 0):
                            empty = 1
                        else:
                            if self.level != INFINITE:
                                self.avail_blocks[playerTwo.block_type] -= 1
                    elif(playerTwo.block_type == TILE_ICE):
                        img = pygame.transform.scale(self.ice_square, (tile_size, tile_size))
                        if (self.avail_blocks[playerTwo.block_type] == 0):
                            empty = 1
                        else:
                            if self.level != INFINITE:
                                self.avail_blocks[playerTwo.block_type] -= 1
                    elif(playerTwo.block_type == TILE_TRAMP):
                        img = pygame.transform.scale(self.trampoline_square, (tile_size, tile_size))
                        if (self.avail_blocks[playerTwo.block_type] == 0):
                            empty = 1
                        else:
                            if self.level != INFINITE:
                                self.avail_blocks[playerTwo.block_type] -= 1
                    elif(playerTwo.block_type == TILE_GRAV): #grav block
                        img = pygame.transform.scale(self.gravity_square, (tile_size, tile_size))
                        if(self.avail_blocks[playerTwo.block_type] == 0):
                            empty = 1
                        else:
                            if self.level != INFINITE:
                                self.avail_blocks[playerTwo.block_type] -= 1
                else:
                    empty = 1
                #only place blocks if there are any left
                if not empty:
                    img_rect = img.get_rect()
                    img_rect.x = playerTwo.rect.x
                    img_rect.y = playerTwo.rect.y
                    self.tile_list.append((img, img_rect, playerTwo.block_type)) # add new block to tile list
                    self.blocks_placed += 1
                    changed = True

        if key_p1 == 'r' or key_p2 == 'r': # reset to default
            self.tile_list = self.default_tile_list.copy()
            self.avail_blocks = self.default_avail_blocks.copy()
            changed = True
            pygame.sprite.Group.empty(enemy_group)


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
        self.spawn_coords = coordinate # remember starting coordinates for respawn
        self.block_type = 1 #default block type
        self.adjusted_rect = self.rect.copy() 
        self.gravity = gravity

    #checks if the player is in a block
    def in_block(self):
        for tile in world.tile_list:
            if tile[1].colliderect(self.rect):
                return True
        return False
    
    def reset(self):
        self.rect.x = self.spawn_coords[0]
        self.rect.y = self.spawn_coords[1]
        self.vel_y = 0
        self.vel_x = 0
        self.block_type = 1
        self.gravity = gravity

    def update(self, key):
        global changed
        if (self.number == 1): #update p1
            if pygame.sprite.spritecollide(self, enemy_group, True): #if you hit an enemy you die
                if world.level == INFINITE:
                    world.level_completed
                else:
                    self.reset()

            change_x = 0
            change_y = 0

            onBlock = False
            onIce = False
            onTrampoline = False
            onGrav = False

            for tile in world.tile_list[:]:
                if tile[1].colliderect(self.rect.x, self.rect.y + 1, self.width, self.height):
                    onBlock = True #p1 is standing on a block
                    if tile[2] == TILE_ICE:
                        onIce = True #p1 is standing on ice
                    elif tile[2] == TILE_TRAMP:
                        onTrampoline = True
                    elif tile[2] == TILE_GRAV:
                        onGrav = True
                        self.gravity *= -1 #switch the gravity
                        self.vel_y = 0
                        change_y = 0
                elif tile[1].colliderect(self.rect.x, self.rect.y - 1, self.width, self.height):
                    onBlock = True #so when gravity is reversed we can't spam s
                    if tile[2] == TILE_ICE:
                        onIce = True
                    elif tile[2] == TILE_TRAMP:
                        onTrampoline = True
                    elif tile[2] == TILE_GRAV:
                        onGrav = True
                        self.gravity = gravity #switch the gravity
                        self.vel_y = 0
                        change_y = 0.5

            if key == 'w':
                if onTrampoline and not onGrav:
                    self.vel_y = -tramp_change_y
                elif onBlock and not onGrav:
                    self.vel_y = -reg_change_y

            if key == 's':
                if onTrampoline and not onGrav:
                    self.vel_y = tramp_change_y
                elif onBlock and not onGrav:
                    self.vel_y = reg_change_y

            elif key == 'a':
                if onIce:
                    self.vel_x = max(self.vel_x - 1, -reg_change_x) #max ice speed
                    change_x -= ice_max_change_x
                else:
                    if not onBlock:
                        self.vel_x = 0
                    change_x -= ice_change_x 

            elif key == 'd':
                if onIce:
                    self.vel_x = min(self.vel_x + 1, reg_change_x) #max ice speed
                    change_x += ice_max_change_x
                else:
                    if not onBlock:
                        self.vel_x = 0
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

            # terminal velocity is reg_change_y, should not get higher before and after gravity flip
            self.vel_y += self.gravity
            if self.gravity >= 0:
                if self.vel_y > reg_change_y:
                    self.vel_y = reg_change_y
            else:
                if self.vel_y < -reg_change_y:
                    self.vel_y = -reg_change_y
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
            elif self.rect.right >= screen_width + world.camera.leftmost_x:
                self.rect.right = screen_width + world.camera.leftmost_x

            self.rect.y += change_y
            if world.level == INFINITE:
                if self.rect.bottom >= game_height + world.camera.bottom_y:
                    world.level_completed = True
                    world.finish_time = pygame.time.get_ticks() - world.start_time
                elif self.rect.top <= world.camera.bottom_y:
                    self.rect.top = world.camera.bottom_y
                    self.vel_y = 0
                    self.change_y = 0
            elif self.rect.bottom >= game_height or self.rect.top <= 0: #respawn when touch bottom of screen
                self.reset()

        elif (self.number == 2): #update p2
            change_x = 0
            change_y = 0

            if key == 'w':
                change_y -= tile_size
            elif key == 's':
               change_y += tile_size
            elif key == 'a':
                change_x -= tile_size
            elif key == 'd':
                change_x += tile_size
            elif key == '1':
                self.block_type = TILE_REG
            elif key == '2':
                self.block_type = TILE_ICE   
            elif key == '3':
                self.block_type = TILE_TRAMP 
            elif key == '4':
                self.block_type = TILE_GRAV       
            
            self.rect.x += change_x
            if self.rect.left <= world.camera.leftmost_x:
                self.rect.left = world.camera.leftmost_x
            elif self.rect.right >= screen_width + world.camera.leftmost_x:
                self.rect.right = screen_width + world.camera.leftmost_x

            self.rect.y += change_y
            if self.rect.top <= world.camera.bottom_y:
                self.rect.top = world.camera.bottom_y
            elif self.rect.bottom >= game_height + world.camera.bottom_y:
                self.rect.bottom = game_height + world.camera.bottom_y

        if world.level == INFINITE:
            camera_adjusted_rect = self.rect.copy()
            camera_adjusted_rect.y = -(world.camera.bottom_y - self.rect.y)
            self.adjusted_rect = camera_adjusted_rect.copy()
        else:
            camera_adjusted_rect = self.rect.copy()
            camera_adjusted_rect.x -= world.camera.leftmost_x
            self.adjusted_rect = camera_adjusted_rect.copy()

class Enemy(pygame.sprite.Sprite):
    def __init__(self, x, y):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.transform.scale(pygame.image.load('images/enemy_block.png'), (tile_size, tile_size))
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.real_x = x
        self.rect.y = y

    def update(self, count):
        global changed
        # self.rect.y += 1 #move downwards
        if count == 20:
            self.rect.y += tile_size
            changed = True
        self.rect.x = self.real_x - world.camera.leftmost_x
        collide_rect = pygame.Rect(self.real_x, self.rect.y, tile_size, tile_size) #since camera adjustments change x
        if self.rect.bottom >= game_height - tile_size:
            self.kill() #remove enemy from game if out of bounds
        for tile in world.tile_list[:]:
            if tile[1].colliderect(collide_rect) and not (tile[2] == TILE_FINISH or tile[2] == TILE_RED or tile[2] == TILE_GRAV):
                world.avail_blocks[tile[2]] += 1
                world.tile_list.remove(tile)
                self.kill()

def create_tile_list(self, data, size_x, size_y):
        tile_list = []
        for row in range(len(data)):
            for col in range(len(data[0])):
                tile = data[row][col]
                if tile != 0:
                    if tile == TILE_REG:
                        img = pygame.transform.scale(self.yellow_square, (size_x, size_y))
                    elif tile == TILE_RED:
                        img = pygame.transform.scale(self.red_square, (size_x, size_y))
                    elif tile == TILE_ICE:
                        img = pygame.transform.scale(self.ice_square, (size_x, size_y))
                    elif tile == TILE_TRAMP:
                        img = pygame.transform.scale(self.trampoline_square, (size_x, size_y))
                    elif tile == TILE_FINISH:
                        img = pygame.transform.scale(self.finish_square, (size_x, size_y))
                    elif tile == TILE_GRAV:
                        img = pygame.transform.scale(self.gravity_square, (size_x, size_y))
                    img_rect = img.get_rect()
                    img_rect.x = col * size_x
                    img_rect.y = row * size_y
                    tile = (img, img_rect, tile)
                    tile_list.append(tile)
        return tile_list

def create_status_bar(self, data, size_x, size_y, level):
    block_offset = screen_width // len(data)
    offset = 0
    tile_list = []
    for key in data:
        if key == TILE_REG:
            img = pygame.transform.scale(self.yellow_square, (size_x, size_y))
        elif key == TILE_RED:
            img = pygame.transform.scale(self.red_square, (size_x, size_y))
        elif key == TILE_ICE:
            img = pygame.transform.scale(self.ice_square, (size_x, size_y))
        elif key == TILE_TRAMP:
            img = pygame.transform.scale(self.trampoline_square, (size_x, size_y))
        elif key == TILE_FINISH:
            img = pygame.transform.scale(self.finish_square, (size_x, size_y))
        elif key == TILE_GRAV:
            img = pygame.transform.scale(self.gravity_square, (size_x, size_y))
        img_rect = img.get_rect()
        # space out blocks horizontally
        if level == INFINITE:
            img_rect.centerx = block_offset * offset + block_offset // 2
        else: # don't need any characters for infinite level
            img_rect.centerx = block_offset * offset + block_offset // 2 - FONT_WIDTH
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
    tile_list.append((img, img_rect, STATUSBAR))

    return tile_list

menu = Menu(bitmaps.player_select, bitmaps.level_select)
#world = World(bitmap, 1) #start at level 1
world = None #will get intialized in the level select screen after player choooses level
playerOne = Player('images/player-one.png', spawn_coords_p1, 1)
playerTwo = Player('images/player-two.png', spawn_coords_p2, 2)
enemy_group = pygame.sprite.Group()

# game loop, will continue until quit
def run_game():
    global world
    global changed
    open = True
    count = 0
    while open:
        clock.tick(20) # number of frames per sec
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                open = False

        input_p1 = ''
        input_p2 = ''
        if player_one_inputs:
            input_p1 = player_one_inputs[0]
            player_one_inputs.popleft() #delete the latest action
        if player_two_inputs:
            input_p2 = player_two_inputs[0]
            player_two_inputs.popleft() #delete the latest action

        if input_p1 == '0' or input_p2 == '0': # return to level select (quit level)
            changed = True
            menu.current_menu_screen = LEVEL_SELECT

        if menu.current_menu_screen != GAMEPLAY:
            menu.update(input_p1, input_p2)
        elif world.level_completed:
            if world.level < 6:
                menu.current_menu_screen = FINISHED
                pygame.time.delay(3000)
                menu.current_menu_screen = COMPLETE_MENU
            else: # no levels left, return to level select
                menu.current_menu_screen = FINISHED
                pygame.time.delay(3000)
                menu.current_menu_screen = LEVEL_SELECT
                changed = True
        else:
            world.update(input_p1, input_p2)
            playerOne.update(input_p1)
            playerTwo.update(input_p2)
            count += 1
            enemy_group.update(count)
            if (count == 20):
                count = 0

def receiver (conn, addr):
    playerNum = -1
    connected = True
    while playerNum == -1 and connected:
        try:
            message = conn.recv(1).decode(FORMAT)
            message = int(message)
            print("[" + str(addr) + "] " + message)
        except:
            if not message:
                connected = False
                print("[" + str(addr) + "] " + DISCONNECT_MESSAGE)
            else:
                print("[" + str(addr) + "] Invalid Input: Requires 1 or 2")
        if message == 1 and not menu.player_one_connect:
            print("[" + str(addr) + "] SET TO P1")
            player_one_inputs.append('1')
            playerNum = 1
        elif message == 2 and not menu.player_two_connect:
            print("[" + str(addr) + "] SET TO P2")
            player_two_inputs.append('2')
            playerNum = 2

    while connected:
        # block thread until we get information from Client
        message = conn.recv(1).decode(FORMAT)
        if message == DISCONNECT_MESSAGE or not message:
            connected = False
            print("[" + str(addr) + "] " + DISCONNECT_MESSAGE)
        else:
            print("[" + str(addr) + "] " + message)
        if playerNum == 1:
            player_one_inputs.append(message)
        elif playerNum == 2:
            player_two_inputs.append(message)
    conn.close()

def sender ():
    global world
    global playerOne
    global playerTwo
    global clients_arr
    global player_change
    global changed
    connected = True

    while connected:
        # block thread until we get information from Client
        try:
            str_tilelist = ''
            if changed: #signal to clear the display
                str_tilelist += '.,' 
                changed = False
                if menu.current_menu_screen == GAMEPLAY: #determine what the background is
                    if world.level % 2 == 0: #even number levels are day themed
                        str_tilelist += '2,'
                    else: #odd number levels are night themed
                        str_tilelist += '3,'
                else:
                    str_tilelist += '1,'
            if menu.current_menu_screen == FINISHED: # signal game is done
                str_tilelist += f"?,{world.finish_time}, {world.blocks_placed},"
            elif menu.current_menu_screen == COMPLETE_MENU: # signal post complete menu
                str_tilelist += f"/,{menu.post_mode},"
            elif menu.current_menu_screen != GAMEPLAY: 
                str_tilelist += '!,!,' # remove status bar text if it exists 
                if menu.current_menu_screen == PLAYER_SELECT:
                    tile_list = menu.player_select_tile_list
                elif menu.current_menu_screen == LEVEL_SELECT:
                    tile_list = menu.level_select_tile_list
                for tile in tile_list:
                    str_tilelist += f"{tile[1].x},{tile[1].y},{tile[2] + MENUOFFSET},"
                str_tilelist += "&"
            else: # game in progress
                if world.level != INFINITE: # infinite level has unlimited blocks
                    str_tilelist += '!,' 
                    for key in world.avail_blocks:
                        str_tilelist += f"{world.avail_blocks[key]},"
                    str_tilelist += '!,' 

                str_tilelist += f"{playerOne.adjusted_rect.x},{playerOne.adjusted_rect.y},{PLAYER1},"
                str_tilelist += f"{playerTwo.adjusted_rect.x},{playerTwo.adjusted_rect.y},{PLAYER2},"
                
                index = 0
                for tile in world.tile_list:
                    if tile[1].y > (game_height - bar_height): #if the block is part of the status bar
                        str_tilelist += f"{tile[1].x},{tile[1].y},{tile[2]},"
                    elif world.camera.onScreen(tile[1]): #if the tile is within camera view
                        if world.level == INFINITE:
                            camera_adjusted_rect = tile[1].copy()
                            camera_adjusted_rect.y = tile[1].y - world.camera.bottom_y #readjust its x coordinate so its within screen boundaries
                            str_tilelist += f"{camera_adjusted_rect.x},{camera_adjusted_rect.y},{tile[2]},"
                        else:
                            camera_adjusted_rect = tile[1].copy()
                            camera_adjusted_rect.x -= world.camera.leftmost_x #readjust its x coordinate so its within screen boundaries
                            str_tilelist += f"{camera_adjusted_rect.x},{camera_adjusted_rect.y},{tile[2]},"
                    elif world.level == INFINITE:
                        world.tile_list.pop(index) #remove tile if no longer on screen in infinite level
                    index += 1
                
                for enemy in pygame.sprite.Group.sprites(enemy_group):
                    str_tilelist += f"{enemy.rect.x},{enemy.rect.y},{TILE_BOMB},"
                str_tilelist += "&"

            for client in clients_arr:
                client[0].send(str_tilelist.encode())
                client[0].recv(1).decode(FORMAT)
        except socket.error:
            connected = False
            for client in clients_arr:
                print("[" + str(client[1]) + "] " + DISCONNECT_MESSAGE) #!!!!
    for client in clients_arr:
        client[0].close()

def handle_client(conn, addr):
    print("[NEW CONNECTION] " + str(addr) + " connected")
    
    message = conn.recv(1).decode(FORMAT)
    message = int(message)
    if message == 1:
        thread = threading.Thread(target=receiver, args=(conn,addr))
        thread.start()
    elif message == 0:
        clients_arr.append((conn, addr))
    else:
        print("ERROR")

def run_server():
    server.listen(5)
    print("[LISTENING]")
    
    count = 0
    while count < max_threads:
        # Establish connection with client.
        conn, addr = server.accept()
        handle_client(conn, addr)
        # thread = threading.Thread(target=handle_client, args=(conn,addr))
        # thread.start()
        print("[ACTIVE CONNECTIONS] " + str(threading.activeCount() - 1)) # Doesn't include main thread
        count += 1

    print("[MAX THREADS CONNECTED]")
    thread = threading.Thread(target=sender, args=())
    thread.start()
    run_game()

try:
    print("Starting server")
    run_server()
    pygame.quit()
    quit()
except (KeyboardInterrupt, SystemExit): # close server with Ctrl + C
    print("\nClosing Server")
    pygame.quit()
    sys.exit()
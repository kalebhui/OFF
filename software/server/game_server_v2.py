import socket
import threading
import time
import sys
import pygame
from collections import deque
import bitmaps
import os

os.environ["SDL_VIDEODRIVER"] = "dummy" #!!!! delete might be making the server slower

SERVER = ''
PORT = 3389
FORMAT = "utf-8"
DISCONNECT_MESSAGE = "quit"
clients_arr = []
max_threads = 2 # Final product clients will require 2 threads each
changed = False
player_change = False
player_one_inputs = deque() #stores user inputs
player_two_inputs = deque() #stores user inputs

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((SERVER, PORT))

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
reg_blk_count = 10
ice_blk_count = 9
tramp_blk_count = 8

avail_blocks = {} #store counts in hashmap
if (reg_blk_count):
    avail_blocks[1] = reg_blk_count
if (ice_blk_count):
    avail_blocks[3] = ice_blk_count
if (tramp_blk_count):
    avail_blocks[4] = tramp_blk_count

levels = bitmaps.levels #stores bitmaps for levels

screen = pygame.display.set_mode((screen_width, screen_height))
# text_font = pygame.font.SysFont('Bauhaus 93', 24)
# white = (255, 255, 255)

PLAYER_SELECT = 0
LEVEL_SELECT = 1
GAMEPLAY = 2


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

    def update(self, key_p1, key_p2):
        global world #allows us to access the world variable in this function
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
            if key_p1 == '1' or key_p2 == '1':
                world = World(levels[0], 1) #intializes world with proper level
                self.current_menu_screen = GAMEPLAY
            elif key_p1 == '2' or key_p2 == '2':
                world = World(levels[1], 2)
                self.current_menu_screen = GAMEPLAY
            elif key_p1 == '3' or key_p2 == '3':
                world = World(levels[2], 3)
                self.current_menu_screen = GAMEPLAY
            elif key_p1 == '4' or key_p2 == '4':
                world = World(levels[3], 4)
                self.current_menu_screen = GAMEPLAY
            elif key_p1 == '5' or key_p2 == '5':
                world = World(levels[4], 5)
                self.current_menu_screen = GAMEPLAY

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


class Camera():
    def __init__(self, level, level_width = None):
        self.leftmost_x = 0
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
        old_camera = self.leftmost_x
        self.leftmost_x = playerOne.rect.x - screen_width // 2 #camera is relative to player one position
        if old_camera != self.leftmost_x:
            changed = True #if the camera is moving we are changing the tile list potentially???
        if self.leftmost_x < 0:
            self.leftmost_x = 0
        elif self.leftmost_x > self.max_x - screen_width:
            self.leftmost_x = self.max_x - screen_width


class World():
    def __init__(self, data, level):
        self.default_tile_list = []
        self.default_status_tile_list =[]
        self.start_time = pygame.time.get_ticks()
        self.finish_time = -1
        self.level_completed = False
        self.blocks_placed = 0
        self.level = level
        self.camera = Camera(level, screen_width + 200)

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

    def update(self, key_p1, key_p2):
        # draw_text(str((pygame.time.get_ticks() - self.start_time) / 1000), text_font, white, tile_size - 10, 10) #!!!!
        self.camera.update(playerOne)
        # draw block counts
        offset = 0
        for key in self.avail_blocks: #move this to sender function I think!!!!
            xcoord = status_tile_size * (offset * 3 + 2)
            ycoord = game_height + (screen_height - game_height) // 2 + bar_height
            # draw_text(str(self.avail_blocks[key]), text_font, white, xcoord, ycoord) #!!!!
            offset += 1
        
        #check if p1 finished the level
        for tile in world.tile_list:
            if tile[1].colliderect(playerOne.rect.x, playerOne.rect.y + 1, playerOne.width, playerOne.height):
                if tile[2] == 5:
                    self.finish_time = pygame.time.get_ticks() - self.start_time
                    self.level_completed = True

        empty = 0
        if key_p2 == 'p': # place block
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
                    changed = True #???

        if key_p1 == 'r' or key_p2 == 'r': # reset to default
            self.tile_list = self.default_tile_list.copy()
            self.avail_blocks = self.default_avail_blocks.copy()
            changed = True #???


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

    def update(self, key):
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

            if key == 'w':
                if onTrampoline:
                    self.vel_y = -tramp_change_y
                elif onBlock:
                    self.vel_y = -reg_change_y
            elif key == 'a':
                if onIce:
                    self.vel_x = max(self.vel_x - 1, -reg_change_x) #max ice speed
                    change_x -= ice_max_change_x
                else:
                    change_x -= ice_change_x 

            elif key == 'd':
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
            elif self.rect.right >= screen_width + world.camera.leftmost_x:
                self.rect.right = screen_width + world.camera.leftmost_x

            self.rect.y += change_y
            if self.rect.top <= 0:
                self.rect.top = 0
                self.vel_y = gravity
            elif self.rect.bottom >= game_height: #respawn when touch bottom of screen
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
                self.block_type = 1
            elif key == '2':
                self.block_type = 3    
            elif key == '3':
                self.block_type = 4         
            
            self.rect.x += change_x
            if self.rect.left <= world.camera.leftmost_x:
                self.rect.left = world.camera.leftmost_x
            elif self.rect.right >= screen_width + world.camera.leftmost_x:
                self.rect.right = screen_width + world.camera.leftmost_x

            self.rect.y += change_y
            if self.rect.top <= 0:
                self.rect.top = 0
            elif self.rect.bottom >= game_height:
                self.rect.bottom = game_height

        camera_adjusted_rect = self.rect.copy()
        camera_adjusted_rect.x -= world.camera.leftmost_x
        self.adjusted_rect = camera_adjusted_rect.copy()
        # draw player at current location
        # screen.blit(self.playerImg, camera_adjusted_rect)

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
    tile_list.append((img, img_rect, 0))

    return tile_list

menu = Menu(bitmaps.player_select, bitmaps.level_select)
#world = World(bitmap, 1) #start at level 1
world = None #will get intialized in the level select screen after player choooses level
playerOne = Player('images/player-one.png', spawn_coords_p1, 1)
playerTwo = Player('images/player-two.png', spawn_coords_p2, 2)

# game loop, will continue until quit
def run_game():
    global world
    global player_change
    open = True
    while open:
        clock.tick(20) # number of frames per sec
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                open = False

        input_p1 = ''
        input_p2 = ''
        if player_one_inputs:
            input_p1 = player_one_inputs[0]
            player_change = True
            player_one_inputs.popleft() #delete the latest action
        if player_two_inputs:
            input_p2 = player_two_inputs[0]
            player_change = True
            player_two_inputs.popleft() #delete the latest action

        if menu.current_menu_screen != GAMEPLAY:
            menu.update(input_p1, input_p2)
        elif world.level_completed:
            if world.level < 5:
                #draw_text(f"Congratulations! You have completed the level in {(world.finish_time - world.start_time) / 1000}s, with {world.blocks_placed} blocks placed!", 
                # text_font, white, screen_width // 2 - 395, screen_height // 2)
                # print("done")
                pygame.time.delay(3000) # wait for 3 seconds
                world = World(levels[world.level + 1], world.level + 1) #recreate world with next level bitmap
                playerOne.reset() #respawn player one
                playerTwo.reset() #respawn player two
            else:
                pass #!!!!
        else:
            world.update(input_p1, input_p2)
            playerOne.update(input_p1)
            playerTwo.update(input_p2)

def receiver (conn, addr):
    playerNum = -1
    connected = True
    while playerNum == -1 and connected: #!!!! right now two players can connect to 1 or 2
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

# def sender (conn, addr):
#     global world
#     global playerOne
#     global playerTwo
    
#     connected = True
#     while connected:
#         # block thread until we get information from Client
#         try:
#             if menu.current_menu_screen != GAMEPLAY:
#                 str_tilelist = ''
#                 if menu.current_menu_screen == PLAYER_SELECT:
#                     tile_list = menu.player_select_tile_list
#                 elif menu.current_menu_screen == LEVEL_SELECT:
#                     tile_list = menu.level_select_tile_list
#                 for tile in tile_list:
#                     str_tilelist += f"{tile[1].x},{tile[1].y},{tile[2]},"
#                 str_tilelist += "&"
#                 conn.send(str_tilelist.encode())
#                 conn.recv(1).decode(FORMAT)
#             else:
#                 str_tilelist = ''
#                 str_tilelist += f"{playerOne.adjusted_rect.x},{playerOne.adjusted_rect.y},-1,"
#                 str_tilelist += f"{playerTwo.adjusted_rect.x},{playerTwo.adjusted_rect.y},-2,"
#                 for tile in world.tile_list:
#                     if tile[1].y > (game_height - bar_height): #if the block is part of the status bar
#                         str_tilelist += f"{tile[1].x},{tile[1].y},{tile[2]},"
#                     elif world.camera.onScreen(tile[1]): #if the tile is within camera view
#                         camera_adjusted_rect = tile[1].copy()
#                         camera_adjusted_rect.x -= world.camera.leftmost_x #readjust its x coordinate so its within screen boundaries
#                         str_tilelist += f"{camera_adjusted_rect.x},{camera_adjusted_rect.y},{tile[2]},"
#                 str_tilelist += "&"
#                 conn.send(str_tilelist.encode())
#                 conn.recv(1).decode(FORMAT)
#         except socket.error:
#             connected = False
#             print("[" + str(addr) + "] " + DISCONNECT_MESSAGE) #!!!!
#     conn.close()

def sender ():
    global world
    global playerOne
    global playerTwo
    global clients_arr
    
    connected = True
    while connected:
        # block thread until we get information from Client
        try:
            str_tilelist = ''
            if player_change:
                str_tilelist += '.,'
                player_change = False
            if menu.current_menu_screen != GAMEPLAY:
                if menu.current_menu_screen == PLAYER_SELECT:
                    tile_list = menu.player_select_tile_list
                elif menu.current_menu_screen == LEVEL_SELECT:
                    tile_list = menu.level_select_tile_list
                for tile in tile_list:
                    str_tilelist += f"{tile[1].x},{tile[1].y},{tile[2]},"
                str_tilelist += "&"
                for client in clients_arr:
                    client[0].send(str_tilelist.encode())
                    client[0].recv(1).decode(FORMAT)
            else:
                str_tilelist += f"{playerOne.adjusted_rect.x},{playerOne.adjusted_rect.y},-1,"
                str_tilelist += f"{playerTwo.adjusted_rect.x},{playerTwo.adjusted_rect.y},-2,"
                for tile in world.tile_list:
                    if tile[1].y > (game_height - bar_height): #if the block is part of the status bar
                        str_tilelist += f"{tile[1].x},{tile[1].y},{tile[2]},"
                    elif world.camera.onScreen(tile[1]): #if the tile is within camera view
                        camera_adjusted_rect = tile[1].copy()
                        camera_adjusted_rect.x -= world.camera.leftmost_x #readjust its x coordinate so its within screen boundaries
                        str_tilelist += f"{camera_adjusted_rect.x},{camera_adjusted_rect.y},{tile[2]},"
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
        # sender(conn, addr)
    else:
        print("ERROR")

def run_server():
    server.listen(5)
    print("[LISTENING]")
    
    while (threading.activeCount() - 1) < max_threads:
        # Establish connection with client.
        conn, addr = server.accept()
        handle_client(conn, addr)
        # thread = threading.Thread(target=handle_client, args=(conn,addr))
        # thread.start()
        print("[ACTIVE CONNECTIONS] " + str(threading.activeCount() - 1)) # Doesn't include main thread

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
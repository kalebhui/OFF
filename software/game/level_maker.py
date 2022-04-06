from math import ceil
from turtle import color
import pygame
import bitmaps

pygame.init()
clock = pygame.time.Clock()
screen_width = 650
screen_height = 240
tile_size = 10
spawn_coords_p2 = (20 * tile_size, 5 * tile_size)
screen = pygame.display.set_mode((screen_width, screen_height)) #replace later with code to output to framebuffer
pygame.display.set_caption("Outwit or Fall Flat: LEVEL EDITOR")

#empty map setting

#potential function for converting an image into a bitmap

# pixel_map = []
# image = pygame.image.load('images/load_btn.png')
 
# new_image = pygame.transform.scale(image, (image.get_width()//2, image.get_height()//2)) #maybe if we need to scale it down

# for i in range(image.get_height()):
#     for j in range(image.get_width()):
#         pixel_col = image.get_at((j, i))
#         real_colour = 0
#         if pixel_col[0] + pixel_col[1] + pixel_col[2] > 400:
#             real_colour = (0, 0, 0, 255)
#         else:
#             real_colour = (255, 255, 255, 255)
#         newSurface = pygame.Surface((tile_size, tile_size))
#         newSurface.fill(real_colour)
#         pixel_map.append((newSurface, (j*4,i*4)))


#for quickly finding coordinates based on placing green squares

def findPosition():
    for i in range(screen_height//tile_size):
        for j in range(screen_width//tile_size):
            if world_data[i][j] == 5:
                print(i, j)



# for row in range(len(world_data)):
#     for col in range (31):
#         if world_data[row][col] == 2:
#             world_data[row][col+34] = 2

# for row in range(3, len(world_data)):
#     for col in range (len(world_data[0])):
#         if world_data[row][col] == 1:
#             world_data[row][col] = 0
#             world_data[row-3][col] = 1
#         elif world_data[row][col] == 2:
#             world_data[row][col] = 0
#             world_data[row-3][col] = 2

world_data = []


# to intialize a blank world

# for row in range(len(world_data)-15):
#     for col in range (len(world_data[0])):
#         if world_data[row][col] == 2:
#             world_data[row+12][col] = 2
for row in range(screen_height // tile_size):
	r = [0] * (screen_width // tile_size)
	world_data.append(r)


# used to print the world_data to be used for bitmap 
# deletes any blank space at the end of the level
def handle_world_data(width):
    if width == 0:
        return
    empty = True
    for row in world_data:
        if row[width] != 0:
            empty = False
            break
    if empty == False:
        print(world_data)
        print(width + 1)
    else:
        for row in world_data:
            del row[width]
        handle_world_data(width - 1)

class World():
    def __init__(self, data):
        #load images
        self.yellow_square = pygame.image.load('images/yellow.png')
        self.red_square = pygame.image.load('images/red.png')
        self.ice_square = pygame.image.load('images/ice.png')
        self.trampoline_square = pygame.image.load('images/trampoline.png')
        self.finish_square = pygame.image.load('images/finish.png')
        self.gravity_square = pygame.image.load('images/gravity_block.png')
        self.pressed = False


    def update(self, event_list):
        key = pygame.key.get_pressed()
        for event in event_list:
            if event.type == pygame.MOUSEBUTTONDOWN:
                self.pressed = True
            elif event.type == pygame.MOUSEBUTTONUP:
                self.pressed = False
                findPosition() #!!!!!!!!
        if self.pressed or key[pygame.K_SPACE]:
            world_data[blockPlacer.rect.y // tile_size][blockPlacer.rect.x // tile_size] = blockPlacer.block_type # add new block to world data
        if key[pygame.K_r]: # reset to default
            for row in range(screen_height // tile_size):
                for col in range(screen_width // tile_size):
                    world_data[row][col] = 0
        for row in range(screen_height // tile_size):
            for col in range(screen_width // tile_size):
                img = None
                if(world_data[row][col] == 1): #normal square
                    img = pygame.transform.scale(self.yellow_square, (tile_size, tile_size))
                if(world_data[row][col] == 2): #starting square
                    img = pygame.transform.scale(self.red_square, (tile_size, tile_size))
                elif(world_data[row][col] == 3): #ice square
                    img = pygame.transform.scale(self.ice_square, (tile_size, tile_size))
                elif(world_data[row][col] == 4): #trampoline square
                    img = pygame.transform.scale(self.trampoline_square, (tile_size, tile_size))
                elif(world_data[row][col] == 5): #finish square
                    img = pygame.transform.scale(self.finish_square, (tile_size, tile_size))
                elif(world_data[row][col] == 6): #finish square
                    img = pygame.transform.scale(self.gravity_square, (tile_size, tile_size))
                
                if img != None:
                    img_rect = img.get_rect()
                    img_rect.x = col * tile_size
                    img_rect.y = row * tile_size
                    screen.blit(img, img_rect)


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
        self.mode = "mouse"

    def update(self, event_list):
        change_x = 0
        change_y = 0
        # if keystroke is pressed check whether its right or left
        key = pygame.key.get_pressed()
        if key[pygame.K_UP]:
            change_y -= tile_size
        if key[pygame.K_DOWN]:
           change_y += tile_size
        if key[pygame.K_LEFT]:
            change_x -= tile_size
        if key[pygame.K_RIGHT]:
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
        if key[pygame.K_6]:
            self.block_type = 6
        if key[pygame.K_0]:
            self.block_type = 0
        if key[pygame.K_BACKSPACE]:
            if self.mode == "mouse":
                self.mode = "keyboard"
            else:
                self.mode = "mouse"
        if key[pygame.K_RETURN]:
            handle_world_data((screen_width // tile_size) - 1)
            return True

        if self.mode == "keyboard":
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
        
        else:
            self.rect.x = pygame.mouse.get_pos()[0] #get x coord of mouse
            self.rect.y = pygame.mouse.get_pos()[1] #get y coord of mouse

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
        elif(self.block_type == 6): #finish square
            self.playerImg = pygame.transform.scale(world.gravity_square, (tile_size, tile_size))

        # draw player at current location
        screen.blit(self.playerImg, self.rect)

world = World(world_data)
blockPlacer = Player('images/player-two.png', spawn_coords_p2)

# game loop, will continue until quit
open = True
while open:
    clock.tick(60) # number of frames per sec. NOTE: updated 120 so mouse movement feels smoother
    screen.fill((116, 148, 222)) #background colour
    event_list = pygame.event.get() #have to call from main loop and pass it to objects
    for event in event_list:
        if event.type == pygame.QUIT:
            open = False

    world.update(event_list)
    if blockPlacer.update(event_list):
        open = False
    # for pixel in pixel_map:
    #     screen.blit(pixel[0], pixel[1])
    #screen.blit(new_image, (0, 0))
    pygame.display.update()

pygame.quit()
quit()
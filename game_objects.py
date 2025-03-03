import os
import pygame
from maze import *



# Load images for each character
def load_images():
    images = {}
    characters = ['#', '.', '@', 'g', 'r', 'i', '*', '|', '$', 'h', 'k', 's', '+', '-']
    for char in characters:
        filename = f'{char}.png'
        if not os.path.exists(filename):
            create_placeholder_image(char, filename)
        images[char] = pygame.image.load(filename)
    return images

# Create placeholder images for characters
def create_placeholder_image(char, filename):
    font = pygame.font.SysFont('Roboto', TILE_SIZE)
    image = pygame.Surface((TILE_SIZE, TILE_SIZE))
    image.fill((0, 0, 0))
    text_surface = font.render(char, True, (0, 255, 0))
    image.blit(text_surface, (0, 0))
    pygame.image.save(image, filename)

# Structure for a cell coordinate in the grid
class Cell:
    def __init__(self, x, y):
        self.x = x
        self.y = y

# Structure for an enemy
class Enemy:
    def __init__(self, tile, room, x, y, health, strength, speed, armour):
        self.tile = tile
        self.room = room
        self.x = x
        self.y = y
        self.old_x = x
        self.old_y = y
        self.health = health
        self.strength = strength
        self.speed = speed
        self.armour = armour
        

# Initialize the game
def init():
    global player_x, player_y, keys, health, score, room, potion, magic, enemy_count, sword, weapon, idols, draw_whole_screen
    keys = 0
    health = 100
    score = 0
    room = 2
    potion = 0
    magic = 0
    enemy_count = 0
    sword = False
    weapon = 1
    idols = 0
    draw_whole_screen = False

# Load the room
def load_room():
    global player_x, player_y, keys, health, score, room, potion, magic, enemy_count, sword, weapon, idols, draw_whole_screen
    carveMaze()
    placePlayer()
    enemies = []
    enemy_count = 0

    for i in range(room - 1):
        placeObject('g')
    for i in range(room):
        placeObject('i')
    for i in range(room + 1):
        placeObject('r')
    placeObject('*')
    placeObject('|')
    placeObject('$')
    placeObject('h')
    placeObject('k')
    placeObject('s')
    placeHDoor()
    placeVDoor()

    for y in range(PLAYABLE_HEIGHT):
        for x in range(MAP_WIDTH):
            c = map[y][x]
            if c == '@':
                print(f"Player position: ({player_x}, {player_y})")
                player_y = y
                player_x = x
                print(f"Player position: ({player_x}, {player_y})")
            elif c == 'g':
                enemies.append(Enemy(c, room, x, y, 30, 5, 1, 10))
                enemy_count += 1
            elif c == 'r':
                enemies.append(Enemy(c, room, x, y, 15, 5, 2, 0))

# Draw the map
def draw_map(screen, images):
    for y in range(MAP_HEIGHT):
        for x in range(MAP_WIDTH):
            tile = map[y][x]
            if tile in images:
                screen.blit(images[tile], (x * TILE_SIZE, y * TILE_SIZE))


import pygame
import random
import time
import os
from maze import carveMaze, placePlayer, placeObject, placeHDoor, placeVDoor, map

# Constants
MAP_WIDTH = 40
MAP_HEIGHT = 24
HUD_TOP = 1
HUD_BOTTOM = 2
PLAYABLE_HEIGHT = MAP_HEIGHT - HUD_TOP - HUD_BOTTOM
TILE_SIZE = 20

# Global variables
run = True
in_play = False
obstruction = False

player_x = 19
player_y = 8
old_x, old_y = 0, 0
direction_x, direction_y = 0, 0
fx, fy = 0, 0
room = 2
draw_whole_screen = False

keys, idols, potion = 0, 0, 0
health = 100
magic = 0
score = 0
sword = False
weapon = 1

enemy_count = 0
enemies = []

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
    font = pygame.font.SysFont('Arial', TILE_SIZE)
    image = pygame.Surface((TILE_SIZE, TILE_SIZE))
    image.fill((255, 255, 255))
    text_surface = font.render(char, True, (0, 0, 0))
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
    global keys, health, score, room, potion, magic, enemy_count, sword, weapon, idols, draw_whole_screen
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
    global player_x, player_y, enemy_count, enemies, map
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
                player_y = y
                player_x = x
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

# Main game loop
def game_loop(screen, images):
    global player_x, player_y, old_x, old_y, direction_x, direction_y, health, score, magic, sword, weapon, keys, idols, room, in_play, run

    while run:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_w:
                    player_y -= 1
                elif event.key == pygame.K_a:
                    player_x -= 1
                elif event.key == pygame.K_s:
                    player_y += 1
                elif event.key == pygame.K_d:
                    player_x += 1
                elif event.key == pygame.K_q:
                    in_play = False
                    run = False

        screen.fill((0, 0, 0))
        draw_map(screen, images)
        pygame.display.flip()
        time.sleep(0.1)

# Main function
def main():
    pygame.init()
    screen = pygame.display.set_mode((MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE))
    pygame.display.set_caption("Dungeon Game")

    images = load_images()
    init()
    load_room()

    game_loop(screen, images)
    pygame.quit()

if __name__ == "__main__":
    main()
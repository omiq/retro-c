import pygame
import random
import time
import os
from maze import *
from game_objects import *

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


# Check if the position is walkable
def is_walkable(x, y):
    return map[y][x] in ['.', '@']

# Move the player
def move_player(dx, dy):
    global player_x, player_y, health, score, keys, idols, potion, sword, weapon, magic
    new_x = player_x + dx
    new_y = player_y + dy
    if is_walkable(new_x, new_y):
        # Clear the player's old position
        map[player_y][player_x] = '.'
        player_x = new_x
        player_y = new_y
        # Update the player's new position
        map[player_y][player_x] = '@'
        print(f"Player moved to ({player_x}, {player_y})")  # Debug print
        # Handle interactions with objects
        tile = map[player_y][player_x]
        if tile == 'i':
            idols += 1
            map[player_y][player_x] = '@'
        elif tile == '$':
            score += 10
            map[player_y][player_x] = '@'
        elif tile == 'h':
            health += 10
            map[player_y][player_x] = '@'
        elif tile == 'k':
            keys += 1
            map[player_y][player_x] = '@'
        elif tile == 's':
            sword = True
            map[player_y][player_x] = '@'
        elif tile == '*':
            potion += 1
            map[player_y][player_x] = '@'

# Move enemies
def move_enemies():
    global health
    for enemy in enemies:
        if enemy.health <= 0:
            continue
        dx = player_x - enemy.x
        dy = player_y - enemy.y
        if abs(dx) > abs(dy):
            step_x = 1 if dx > 0 else -1
            step_y = 0
        else:
            step_x = 0
            step_y = 1 if dy > 0 else -1
        new_x = enemy.x + step_x
        new_y = enemy.y + step_y
        if is_walkable(new_x, new_y):
            map[enemy.y][enemy.x] = '.'  # Clear the enemy's old position
            enemy.old_x = enemy.x
            enemy.old_y = enemy.y
            enemy.x = new_x
            enemy.y = new_y
            map[enemy.y][enemy.x] = enemy.tile  # Update the enemy's new position
        # Check for combat with player
        if enemy.x == player_x and enemy.y == player_y:
            health -= enemy.strength
            print(f"Player attacked by {enemy.tile} at ({enemy.x}, {enemy.y}). Health: {health}")  # Debug print

# Main game loop
def game_loop(screen, images):
    global player_x, player_y, old_x, old_y, direction_x, direction_y, health, score, magic, sword, weapon, keys, idols, room, in_play, run

    while run:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_w:
                    move_player(0, -1)
                elif event.key == pygame.K_a:
                    move_player(-1, 0)
                elif event.key == pygame.K_s:
                    move_player(0, 1)
                elif event.key == pygame.K_d:
                    move_player(1, 0)
                elif event.key == pygame.K_q:
                    in_play = False
                    run = False

        move_enemies()

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
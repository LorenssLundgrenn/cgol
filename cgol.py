import pygame
import random as rnd
import math
import sys

args = [60, 60, 25]
for i in range(len(sys.argv)):
    if i != 0 and i <= len(args):
        args[i-1] = int(sys.argv[i])

def create_grid():
    cells = []
    for y in range(height):
        cells.append([])
        for x in range(width):
            randRange = rnd.randint(1,100)
            if randRange <= lifeProbability:
                cells[y].append(1)
            else:
                cells[y].append(0)
    return cells

def get_screen_info():
    screenSize = screen.get_size()
    xFit = screenSize[0] / width
    yFit = screenSize[1] / height
    tileSize = xFit if xFit < yFit else yFit
    xoffset = (screenSize[0] - tileSize * width) / 2
    yoffset = (screenSize[1] - tileSize * height) / 2
    return screenSize, tileSize, xoffset, yoffset

width = args[0]
height = args[1]
lifeProbability = args[2]
if lifeProbability > 70: lifeProbability = 70
elif lifeProbability < 0: lifeProbability = 0
cells = create_grid()

screen = pygame.display.set_mode((500, 500), pygame.RESIZABLE)
clock = pygame.time.Clock()

screenSize, tileSize, xoffset, yoffset = get_screen_info()

mouseButtonHeld = [False, False, False, False, False]
running = True
paused = False
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.VIDEORESIZE:
            screenSize, tileSize, xoffset, yoffset = get_screen_info()

        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                paused = not paused
            elif event.key == pygame.K_TAB:
                lifeProbability = rnd.randint(5, 70)
                cells = create_grid()
            elif event.key == pygame.K_r:
                lifeProbability = 0
                cells = create_grid()

        elif event.type == (pygame.MOUSEBUTTONDOWN or pygame.MOUSEBUTTONUP):
            mouseButtonHeld = pygame.mouse.get_pressed()
            if mouseButtonHeld[0] or mouseButtonHeld[2]:
                pos = pygame.mouse.get_pos()
                cellY = math.floor(((pos[1]-yoffset)/tileSize)%height)
                cellX = math.floor(((pos[0]-xoffset)/tileSize)%width)
                if mouseButtonHeld[0]:
                    if cells[cellY][cellX] != 1:
                        cells[cellY][cellX] = 1
                elif mouseButtonHeld[2]:
                    if cells[cellY][cellX] != 0:
                        cells[cellY][cellX] = 0

    lifeQuery = []
    deathQuery = []
    for y in range(height):
        for x in range(width):
            type = cells[y][x]

            if not paused:
                neighbors = 0
                for rangey in range(3):
                    for rangex in range(3):
                        if cells[(y-1+rangey)%height][(x-1+rangex)%width] == 1:
                            neighbors += 1
                if type == 0:
                    if neighbors == 3:
                        lifeQuery.append((y, x))
                elif type == 1:
                    if neighbors-1 < 2 or neighbors-1 > 3:
                        deathQuery.append((y, x))

            if cells[y][x] == 1:
                pygame.draw.rect(screen, (167, 167, 167), (x*tileSize+xoffset, y*tileSize+yoffset, math.ceil(tileSize), math.ceil(tileSize)))
            else:
                pygame.draw.rect(screen, (17, 17, 17), (x*tileSize+xoffset, y*tileSize+yoffset, math.ceil(tileSize), math.ceil(tileSize)))

    for life in lifeQuery:
        cells[life[0]][life[1]] = 1
    for death in deathQuery:
        cells[death[0]][death[1]] = 0

    pygame.display.flip()
    screen.fill((0,0,0))
    clock.tick(15)

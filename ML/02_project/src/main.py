import os
import pygame
from math import tan, radians, degrees, copysign
from pygame.math import Vector2
from car import Car

class Game:
    def __init__(self):

        self.RES_DIR = 'res/'
        self.WIDTH = 2400
        self.HEIGHT = 1800
        self.CANVAS_OFFSET = (self.WIDTH/2, self.HEIGHT/2)
        self.MAX_FPS = 60
        self.PPU = 32
        pygame.init()
        pygame.display.set_caption("Smart Car Racing")
        self.screen = pygame.display.set_mode((self.WIDTH, self.HEIGHT))
        self.clock = pygame.time.Clock()
        self.exit = False

        self.car = Car(0,0, self.RES_DIR + "car.png")

    def run(self):

        while not self.exit:
            dt = self.clock.get_time() / 1000

            # Event queue
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.exit = True
            self.handle_input()

            # Logic
            self.car.update(dt)
            sensor_surfaces = self.car.update_sensors(self)

            #print(" {}, {}".format(", ".join(str(x) for x in arrow_pressed), car.angle, car.acceleration))
            #print(car.position)
            # Drawing

            pi=3.14159265359
            self.screen.fill((0, 0, 0))
            self.car.draw(self)
            #self.screen.blit(sensor_surfaces[1], (100,100))
            # Crapton of logic to rotate in-place
            w, h = sensor_surfaces[0].get_size()
            box = [pygame.math.Vector2(p) for p in [(0, 0), (w, 0), (w, -h), (0, -h)]]
            box_rotate = [p.rotate(self.car.get_rotation()) for p in box]
            min_box = (min(box_rotate, key=lambda p: p[0])[0], min(box_rotate, key=lambda p: p[1])[1])
            max_box = (max(box_rotate, key=lambda p: p[0])[0], max(box_rotate, key=lambda p: p[1])[1])
            pivot = Vector2(sensor_surfaces[0].get_rect().width/2, sensor_surfaces[0].get_rect().height/2)
            pivot_rotate = pivot.rotate(self.car.get_rotation())
            pivot_move   = pivot_rotate - pivot
            origin = (0 + min_box[0] - pivot_move[0], 0 - max_box[1] + pivot_move[1])
            for s in sensor_surfaces:
                image = pygame.transform.rotate(s, self.car.get_rotation())
                self.screen.blit(image, self.car.get_position()*self.PPU \
                                        + self.CANVAS_OFFSET \
                                        + origin)
            pygame.draw.circle(self.screen, (255, 0, 0), list(int(x) for x in self.to_world_coordinates( self.car.get_position()*self.PPU )), 10)
            pygame.draw.circle(self.screen, (255, 0, 0), list(int(x) for x in self.to_world_coordinates( origin*self.PPU )), 10)
            pygame.display.flip()

            self.clock.tick(self.MAX_FPS)
        pygame.quit()

    def to_world_coordinates(self, pos):
        return pos + self.CANVAS_OFFSET

    def handle_input(self):
        # Read the pressed keys
        pressed_keys = pygame.key.get_pressed()

        if pressed_keys[pygame.K_SPACE]:
            if not self.car.is_running(): self.car.start()

        if pressed_keys[pygame.K_RIGHT]:
            self.car.steer = -1
        elif pressed_keys[pygame.K_LEFT]:
            self.car.steer = 1
        # Check that both directions are not pressed at the same time
        elif pressed_keys[pygame.K_RIGHT] and pressed_keys[pygame.K_LEFT]:
            self.car.steer = 0
        else:
            self.car.steer = 0

if __name__ == '__main__':
    game = Game()
    game.run()

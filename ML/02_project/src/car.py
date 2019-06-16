import pygame
from pygame.math import Vector2
from math import radians, degrees, tan

class Car:
    def __init__(self, x, y, sprite_path, rotation=0.0, speed = 0.0, steer=0):
        # Car config
        self.WIDTH = 2
        self.LENGTH = 4
        self.MAX_SPEED = 10
        self.STEER_STEP = 40     # Steer step in degrees

        # Sprite
        self.sprite = self.Car_Sprite(sprite_path)

        # Sensor stuff
        self.sensor_length = 4


        # Car state
        self.position = Vector2(x, y)
        self.rotation = rotation
        self.speed = speed
        self.steer = steer

        self.running = False

    def update(self, dt):

        if self.steer:
            turning_radius = self.LENGTH / tan(radians(self.steer*self.STEER_STEP))
            angular_v = self.speed / turning_radius
        else:
            angular_v = 0

        self.position += Vector2(self.speed, 0).rotate(-self.rotation) * dt
        self.rotation += degrees(angular_v) * dt

    def get_image(self):
        image = pygame.transform.rotate(self.sprite.image, self.rotation)
        return image, image.get_rect(), pygame.mask.from_surface(image)

    def update_sensors(self, screen):
        scaled_sensor_length = screen.PPU*self.sensor_length
        sensor_surfaces = []
        for i in range(5):
            sensor_surfaces.append(pygame.Surface((scaled_sensor_length*2, scaled_sensor_length+1), flags=pygame.SRCALPHA))
            sensor_surfaces[i].fill((255,0,0,1))

            start = Vector2(scaled_sensor_length, scaled_sensor_length)
            rotated = Vector2(scaled_sensor_length, 0).rotate(45*i)
            end = (start.x + rotated.x, start.y - rotated.y)
            pygame.draw.line(sensor_surfaces[i], (255, 255, 255), start, end)
            sensor_surfaces[i] = pygame.transform.rotate(sensor_surfaces[i], -90)
        return sensor_surfaces

    def get_position(self):
        return self.position

    def get_rotation(self):
        return self.rotation

    def get_image_offset(self):
        rotated, rect, mask = self.get_image()
        return (rect.width / 2, rect.height / 2)

    def is_running(self):
        return self.running

    def start(self):
        self.running = True
        self.speed = self.MAX_SPEED

    def draw(self, screen):
        rotated, rect, mask = self.get_image()
        screen.screen.blit(rotated, self.position * screen.PPU - (rect.width / 2, rect.height / 2) + screen.CANVAS_OFFSET)
    class Car_Sprite(pygame.sprite.Sprite):
        def __init__(self, path):
            # Call het superclass constructor
            pygame.sprite.Sprite.__init__(self)
            self.image = pygame.image.load(path)
            self.rect = self.image.get_rect()
            self.mask = pygame.mask.from_surface(self.image)

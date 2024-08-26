import pygame as pg


class Render():
    def __init__(self, width, height) -> None:
        pg.init()
        self.WIDTH, self.HEIGTH = width, height
        self.FPS = 60
        self.clock = pg.time.Clock()
        self.screen = pg.display.set_mode((self.WIDTH, self.HEIGTH))

    def run(self) -> None:
        while True:
            self.draw()
            [exit() for i in pg.event.get() if i.type == pg.QUIT]
            pg.display.set_caption(str(self.clock.get_fps()))
            pg.display.flip()
            self.clock.tick(self.FPS)
            
    def draw(self) -> None:
        pg.draw.line(self.screen, (255, 255, 255), (0, 0), (1200, 500))
        


if __name__ == "__main__":
    app = Render(1200, 500)
    app.run()
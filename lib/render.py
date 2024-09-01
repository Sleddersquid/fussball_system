import pygame as pg

class Render():
    def __init__(self, width, height) -> None:
        pg.init()
        self.WIDTH, self.HEIGTH = width, height
        self.FPS = 60
        self.clock = pg.time.Clock()
        self.screen = pg.display.set_mode((self.WIDTH, self.HEIGTH))
        self.dt = self.clock.tick(60) / 1000 # ticks per second
        
        # Positions
        self.circlepos = pg.Vector2(self.WIDTH / 2, self.HEIGTH / 2)

    def run(self) -> None:
        while True:
            self.screen.fill(pg.Color("black"))
            self.drawcircle()
            [exit() for i in pg.event.get() if i.type == pg.QUIT]
            pg.display.set_caption(str(self.clock.get_fps()))
            pg.display.flip()
            self.clock.tick(self.FPS)
            
    def draw(self) -> None:
        self.drawcircle()
        
        # Reset
        
    def drawcircle(self):
        pg.draw.circle(self.screen, (0, 255, 255), self.circlepos, 5)
        
        keys = pg.key.get_pressed()
        if keys[pg.K_w]:
            self.circlepos.y -= 100 * self.dt
        if keys[pg.K_s]:
            self.circlepos.y += 100 * self.dt
        if keys[pg.K_d]:
            self.circlepos.x += 100 * self.dt
        if keys[pg.K_a]:
            self.circlepos.x -= 100 * self.dt
             
        # print(self.circlepos)
        
                  
if __name__ == "__main__":
    app = Render(1200, 500)
    app.run()
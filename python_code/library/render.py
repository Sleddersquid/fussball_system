# Modified version of "simple_moving_ball_ui.py" 

import pygame as pg
from moded_array import Moded_Array
from collections import deque

class Render():
    def __init__(self, width, height) -> None:
        pg.init()
        self.WIDTH, self.HEIGTH = width, height
        self.FPS = 60
        self.clock = pg.time.Clock()
        self.screen = pg.display.set_mode((self.WIDTH, self.HEIGTH))
        self.dt = self.clock.tick(60) / 1000 # ticks per second
        
        self.pts = deque(maxlen=10)
        
        # Positions
        self.circlepos = pg.Vector2(self.WIDTH // 2, self.HEIGTH // 2)

    def run(self) -> None:
        while True:
            self.screen.fill(pg.Color("black"))
            # Draw under this line. Why?
            
            self.drawpoints()
            # self.drawcircle()
            [exit() for i in pg.event.get() if i.type == pg.QUIT]
            pg.display.set_caption(str(self.clock.get_fps()))
            pg.display.flip()
            self.clock.tick(self.FPS)
            
            
    def drawpoints(self):
        if len(self.pts) == 0: return
        for pos in self.pts:
            pg.draw.circle(self.screen, (0, 255, 255), pos, 3)
            
    def addpoints(self, x, y):
        self.pts.append(pg.Vector2(x, y))
        print(self.pts)
        
    def drawcircle(self) -> None:
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
    # app.addpoints(100, 100)
    # app.addpoints(100, 30)
    app.run()
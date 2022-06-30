import pygame as pg
import math as m
import numpy as np

res = 20
cf = 28
res1 = 28;
w, h = res * cf, res * cf

scr = pg.display.set_mode((w, h))

l = [0 for i in range(784)]

timer = pg.time.Clock()


class interp:
    def __init__(self, l):
        self.a = l
        self.a.append(self.a[-1]);
        self.l = len(self.a);
    def __call__(self, x):
        x = x / res1 * res - 1e-8;
        lb = int(x // 1)
        fp = x - lb
        return self.a[lb] + (self.a[lb + 1] - self.a[lb]) * fp;

def int2():
    global l;
    a = [[l[i + res * j] for i in range(res)] for j in range(res)]
    ll = []
    for j in range(res):
        lll = []
        for i in range(res):
            lll.append(a[i][j]);
        ll.append(interp(lll))
    a = []
    for i in range(res1):
        a.append([]);
        for j in range(res):
            a[i].append(ll[j](i));
    ll = [];
    for i in range(res1):
        lll = []
        for j in range(res):
            lll.append(a[i][j]);
        ll.append(interp(lll));
    a = [];
    for i in range(res1):
        a.append([]);
        for j in range(res1):
            a[i].append(int(ll[j](i)));
    aa = [];
    for i in range(res1):
        for j in range(res1):
            aa.append(min(a[j][i], 255));
    return aa



def save():
    a = int2();
    with open("image.bin", "wb") as file:
        s = b''
        for i in a:
            s += i.to_bytes(1, 'big');
        file.write(s) 



cond = True

while cond:
    for i in pg.event.get():
        if i.type == pg.QUIT:
            cond = False
            pg.quit()
        elif i.type == pg.KEYDOWN:
            if i.key == pg.K_c:
                l = [0 for j in range(784)]
                for j in range(res ** 2):
                    x = (j % res) * cf;
                    y = (j // res) * cf
                    pg.draw.rect(scr, (l[j], l[j], l[j]), (x, y, x + cf, y + cf))
                pg.display.flip();
            elif i.key == pg.K_p:
                l = int2();
                cf = 20;
                res = 28;
                for j in range(res ** 2):
                    x = (j % res) * cf;
                    y = (j // res) * cf
                    pg.draw.rect(scr, (l[j], l[j], l[j]), (x, y, x + cf, y + cf))
                pg.display.flip();
            elif i.key == pg.K_b:
                cf = 28;
                res = 20;
                l = [0 for j in range(784)]
                for j in range(res ** 2):
                    x = (j % res) * cf;
                    y = (j // res) * cf
                    pg.draw.rect(scr, (l[j], l[j], l[j]), (x, y, x + cf, y + cf))
                pg.display.flip();

            else:
                save()


        state = pg.mouse.get_pressed(num_buttons = 3)
        if (any(state) == True):
            ps = pg.mouse.get_pos()
            x = ps[0] // cf
            y = ps[1] // cf
            j = y * res + x;
            l[j] = 255;
            x *= cf
            y *= cf
            pg.draw.rect(scr, (l[j], l[j], l[j]), (x, y, cf, cf))
            pg.display.flip();
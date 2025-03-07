import pygame
import time
import random



#NEW AGENDA: MAKE AMMO COUNTER


pygame.init()
current_time = 0
width, height = 800, 600
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("dogass shooter")
plyrtimeswonz = 0
plyr2timeswonz = 0
white = (255, 255, 255)
black = (0, 0, 0)
plyrclr = (46, 43, 227)
plyr2clr = (214, 39, 39)
bulletclr = (219, 202, 9)
plyrsze = 25
plyrpos = [width // 2, height - 2 * plyrsze]
plyrresetpos = [width // 2, height - 2 * plyrsze]
plyrspd = 5
plyrscr = 10
plyrdie = False
plyr2sze = 25
plyr2pos = [width // 2, plyr2sze]
plyr2resetpos = [width // 2, plyr2sze]
plyr2spd = 5
plr2scr = 10
plyr2die = False
bullet_size = 20
bullet_speed = 30
bullets = []
new_player_bullets = []
shoot_delay = 400
last_shot_time = 0
last_shot_time_new_player = 0
barricade_width = 100
barricade_height = 26

player_barricades = [
    pygame.Rect(100, height - 5 * plyrsze, barricade_width, barricade_height),
    pygame.Rect(300, height - 5 * plyrsze, barricade_width, barricade_height),
    pygame.Rect(500, height - 5 * plyrsze, barricade_width, barricade_height)
]

new_player_barricades = [
    pygame.Rect(100, 5 * plyr2sze, barricade_width, barricade_height),
    pygame.Rect(300, 5 * plyr2sze, barricade_width, barricade_height),
    pygame.Rect(500, 5 * plyr2sze, barricade_width, barricade_height)
]

running = True
clock = pygame.time.Clock()
collision_time = 0
collision_display_duration = 500  # milliseconds

while running:
    screen.fill(black)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    keys = pygame.key.get_pressed()
    if keys[pygame.K_LEFT] and plyrpos[0] > 0:
        plyrpos[0] -= plyrspd
        for barricade in player_barricades:
            if barricade.colliderect(plyrpos[0], plyrpos[1], plyrsze, plyrsze):
                plyrpos[0] += plyrspd
                break
        for barricade in new_player_barricades:
            if barricade.colliderect(plyrpos[0], plyrpos[1], plyrsze, plyrsze):
                plyrpos[0] += plyrspd
                break
    if keys[pygame.K_RIGHT] and plyrpos[0] < width - plyrsze:
        plyrpos[0] += plyrspd
        for barricade in player_barricades:
            if barricade.colliderect(plyrpos[0], plyrpos[1], plyrsze, plyrsze):
                plyrpos[0] -= plyrspd
                break
        for barricade in new_player_barricades:
            if barricade.colliderect(plyrpos[0], plyrpos[1], plyrsze, plyrsze):
                plyrpos[0] -= plyrspd
                break
    if keys[pygame.K_UP] and plyrpos[1] > 0:
        plyrpos[1] -= plyrspd
        for barricade in player_barricades:
            if barricade.colliderect(plyrpos[0], plyrpos[1], plyrsze, plyrsze):
                plyrpos[1] += plyrspd
                break
        for barricade in new_player_barricades:
            if barricade.colliderect(plyrpos[0], plyrpos[1], plyrsze, plyrsze):
                plyrpos[1] += plyrspd
                break
    if keys[pygame.K_DOWN] and plyrpos[1] < height - plyrsze:
        plyrpos[1] += plyrspd
        for barricade in player_barricades:
            if barricade.colliderect(plyrpos[0], plyrpos[1], plyrsze, plyrsze):
                plyrpos[1] -= plyrspd
                break
        for barricade in new_player_barricades:
            if barricade.colliderect(plyrpos[0], plyrpos[1], plyrsze, plyrsze):
                plyrpos[1] -= plyrspd
                break
    if keys[pygame.K_a] and plyr2pos[0] > 0:
        plyr2pos[0] -= plyr2spd
        for barricade in new_player_barricades:
            if barricade.colliderect(plyr2pos[0], plyr2pos[1], plyr2sze, plyr2sze):
                plyr2pos[0] += plyr2spd
                break
        for barricade in player_barricades:
            if barricade.colliderect(plyr2pos[0], plyr2pos[1], plyr2sze, plyr2sze):
                plyr2pos[0] += plyr2spd
                break
    if keys[pygame.K_d] and plyr2pos[0] < width - plyr2sze:
        plyr2pos[0] += plyr2spd
        for barricade in new_player_barricades:
            if barricade.colliderect(plyr2pos[0], plyr2pos[1], plyr2sze, plyr2sze):
                plyr2pos[0] -= plyr2spd
                break
        for barricade in player_barricades:
            if barricade.colliderect(plyr2pos[0], plyr2pos[1], plyr2sze, plyr2sze):
                plyr2pos[0] -= plyr2spd
    if keys[pygame.K_w] and plyr2pos[1] > 0:
        plyr2pos[1] -= plyr2spd
        for barricade in new_player_barricades:
            if barricade.colliderect(plyr2pos[0], plyr2pos[1], plyr2sze, plyr2sze):
                plyr2pos[1] += plyr2spd
                break
        for barricade in player_barricades:
            if barricade.colliderect(plyr2pos[0], plyr2pos[1], plyr2sze, plyr2sze):
                plyr2pos[1] += plyr2spd
    if keys[pygame.K_s] and plyr2pos[1] < height - plyr2sze:
        plyr2pos[1] += plyr2spd
        for barricade in new_player_barricades:
            if barricade.colliderect(plyr2pos[0], plyr2pos[1], plyr2sze, plyr2sze):
                plyr2pos[1] -= plyr2spd
                break
        for barricade in player_barricades:
            if barricade.colliderect(plyr2pos[0], plyr2pos[1], plyr2sze, plyr2sze):
                plyr2pos[1] -= plyr2spd
    if keys[pygame.K_SPACE]:
        current_time = pygame.time.get_ticks()
        if current_time - last_shot_time > shoot_delay:
            bullets.append(pygame.Rect(plyrpos[0] + plyrsze // 2, plyrpos[1], bullet_size // 2, bullet_size))
            last_shot_time = current_time
            for barricade in player_barricades:
                if barricade.colliderect(plyrpos[0], plyrpos[1], plyrsze, plyrsze):
                    plyrpos[1] += plyrspd
                    break
    if keys[pygame.K_LSHIFT]:
        current_time = pygame.time.get_ticks()
        if current_time - last_shot_time_new_player > shoot_delay:
            new_player_bullets.append(pygame.Rect(plyr2pos[0] + plyr2sze // 2, plyr2pos[1], bullet_size // 2, bullet_size))
            last_shot_time_new_player = current_time
            for barricade in new_player_barricades:
                if barricade.colliderect(plyr2pos[0], plyr2pos[1], plyr2sze, plyr2sze):
                    plyr2pos[1] -= plyr2spd
                    break

    plyr_rect = pygame.Rect(plyrpos[0], plyrpos[1], plyrsze, plyrsze)
    plyr2_rect = pygame.Rect(plyr2pos[0], plyr2pos[1], plyr2sze, plyr2sze)

    for bullet in bullets[:]:
        bullet.y -= bullet_speed
        if bullet.y < 0:
            bullets.remove(bullet)
        elif bullet.colliderect(plyr2_rect):
            bullets.remove(bullet)
            plr2scr -= 1
        else:
            for barricade in new_player_barricades:
                if bullet.colliderect(barricade):
                    bullets.remove(bullet)
                    break
            for barricade in player_barricades:
                if bullet.colliderect(barricade):
                    bullets.remove(bullet)
                    break

    for bullet in new_player_bullets[:]:
        bullet.y += bullet_speed
        if bullet.y > height:
            new_player_bullets.remove(bullet)
        elif bullet.colliderect(plyr_rect):
            new_player_bullets.remove(bullet)
            plyrscr -= 1
        else:
            for barricade in player_barricades:
                if bullet.colliderect(barricade):
                    new_player_bullets.remove(bullet)
                    break
            for barricade in new_player_barricades:
                if bullet.colliderect(barricade):
                    new_player_bullets.remove(bullet)
                    break

    if plyr_rect.colliderect(plyr2_rect):
        plyrscr -= 1
        plr2scr -= 1
        plyrpos = [width // 2, height - 2 * plyrsze]
        plyr2pos = [width // 2, plyr2sze]
        collision_time = pygame.time.get_ticks()
    personthatdied = "player 1"
    
    pygame.draw.rect(screen, plyrclr, (plyrpos[0], plyrpos[1], plyrsze, plyrsze))
    
    pygame.draw.rect(screen, plyr2clr, (plyr2pos[0], plyr2pos[1], plyr2sze, plyr2sze))
    
    
    dietextlength = 5000
    if plyrscr <= 0:
        plyrdie = True
        personthatdied = "player 1"
    if plr2scr <= 0:
        plyr2die = True
        personthatdied = "player 2"
    if plyrscr <= 0 or plr2scr <= 0:
        
        screen.blit(collision_text, text_rect)
        plyrpos = [width // 2, height - 2 * plyrsze]
        plyr2pos = [width // 2, plyr2sze]
        
        collision_text = font.render(f"YOU SUCK: {personthatdied}", True, white)
        text_rect = collision_text.get_rect(center=(width // 2, height // 2))
        currdietime = 0
        #currdietime= pygame.time.get_ticks()
        if plyrdie == True:
            plyr2timeswonz = plyr2timeswonz + 1
            plyrdie = False
        elif plyr2die == True:
            plyrtimeswonz  = plyrtimeswonz + 1
            plyr2die = False
        plyrscr = 10
        plr2scr = 10
        currdietime += 1
        
        
        #running = False

    """pygame.draw.rect(screen, plyrclr, (plyrpos[0], plyrpos[1], plyrsze, plyrsze))
    plyrresetpos[0] = plyrpos[0]
    plyrresetpos[1] = plyrpos[1]
    pygame.draw.rect(screen, plyr2clr, (plyr2pos[0], plyr2pos[1], plyr2sze, plyr2sze))
    plyr2resetpos[0] = plyr2pos[0]
    plyr2resetpos[1] = plyr2pos[1]"""
    for bullet in bullets:
        pygame.draw.rect(screen, bulletclr, (bullet[0], bullet[1], bullet_size // 2, bullet_size))
    for bullet in new_player_bullets:
        pygame.draw.rect(screen, bulletclr, (bullet[0], bullet[1], bullet_size // 2, bullet_size))

    for barricade in player_barricades:
        pygame.draw.rect(screen, white, barricade)
    for barricade in new_player_barricades:
        pygame.draw.rect(screen, white, barricade)

    font = pygame.font.Font(None, 36)
    player_score_text = font.render(f"plyr 1 life: {plyrscr}", True, white)
    new_player_score_text = font.render(f"plyr 2 life: {plr2scr}", True, white)
    plyrtimeswon = font.render(f"plyr 1 wins: {plyr2timeswonz}", True, white)
    plyr2timeswon = font.render(f"plyr 2 wins: {plyrtimeswonz}", True, white)
    screen.blit(player_score_text, (10, 5))
    screen.blit(plyrtimeswon, (10, 30))
    screen.blit(new_player_score_text, (width - 200, 5))
    screen.blit(plyr2timeswon, (width - 200, 30))

    current_time = pygame.time.get_ticks()
    if current_time - collision_time < collision_display_duration:
        screen.fill(white)
        collision_text = font.render("KNIFED!!", True, black)
        text_rect = collision_text.get_rect(center=(width // 2, height // 2))
        screen.blit(collision_text, text_rect)
        

    pygame.display.flip()
    clock.tick(30)

pygame.quit()



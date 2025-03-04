import pygame
import random
import time

pygame.init()
#niggertron prime:
#screen_width = 1920
#screen_height = 1080
screen_width = 800
screen_height = 600
white = (255, 255, 255)
black = (0, 0, 0)
line_thjickness = 10
line_height = 100
sizerness_ball = 10
speed = 10
ball_speed_x = 4
ball_speed_y = 4

screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption("bruh")

class moveing_lines:
    def __init__(bruh, x, y):
        bruh.rect = pygame.Rect(x, y, line_thjickness, line_height)
        bruh.speed = speed

    def move(bruh, up=True):
        if up:
            bruh.rect.y -= bruh.speed
        else:
            bruh.rect.y += bruh.speed

    def draw(bruh, screen):
        pygame.draw.rect(screen, white, bruh.rect)

class Ball:
    def __init__(bruh, x, y, speed_x=None, speed_y=None, player_shot=False):
        bruh.rect = pygame.Rect(x, y, sizerness_ball, sizerness_ball)
        bruh.speed_x = speed_x if speed_x is not None else ball_speed_x * random.choice((1, -1))
        bruh.speed_y = speed_y if speed_y is not None else ball_speed_y * random.choice((1, -1))
        bruh.player_shot = player_shot

    def move(bruh):
        bruh.rect.x += bruh.speed_x
        bruh.rect.y += bruh.speed_y

    def draw(bruh, screen):
        pygame.draw.rect(screen, white, bruh.rect)

    def reset(bruh):
        if not bruh.player_shot:
            bruh.rect.x = screen_width // 2
            bruh.rect.y = screen_height // 2
            bruh.speed_x = ball_speed_x * random.choice((1, -1))
            bruh.speed_y = ball_speed_y * random.choice((1, -1))

plyerr_paddle = moveing_lines(30, screen_height // 2 - line_height // 2)
opponent_paddle = moveing_lines(screen_width - 40, screen_height // 2 - line_height // 2)
balls = [Ball(screen_width // 2, screen_height // 2) for _ in range(5)]

plyerr_score = 0
opponent_score = 0
font = pygame.font.Font(None, 74)

last_shot_time_plyerr = 0
last_shot_time_opponent = 0
shot_delay = 3  # in fucking secdonds not ms like in unity fopr some fuckass reason???

running = True
clock = pygame.time.Clock()

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    keys = pygame.key.get_pressed()
    if keys[pygame.K_w] and plyerr_paddle.rect.top > 0:
        plyerr_paddle.move(up=True)
    if keys[pygame.K_s] and plyerr_paddle.rect.bottom < screen_height:
        plyerr_paddle.move(up=False)
    if keys[pygame.K_UP] and opponent_paddle.rect.top > 0:
        opponent_paddle.move(up=True)
    if keys[pygame.K_DOWN] and opponent_paddle.rect.bottom < screen_height:
        opponent_paddle.move(up=False)

    current_time = time.time()
    #shot queuing
    if keys[pygame.K_z] and current_time - last_shot_time_plyerr >= shot_delay:
        balls.append(Ball(plyerr_paddle.rect.right, plyerr_paddle.rect.centery, ball_speed_x, random.choice((1, -1)) * ball_speed_y, player_shot=True))
        last_shot_time_plyerr = current_time
    if keys[pygame.K_RCTRL] and current_time - last_shot_time_opponent >= shot_delay:
        balls.append(Ball(opponent_paddle.rect.left - sizerness_ball, opponent_paddle.rect.centery, -ball_speed_x, random.choice((1, -1)) * ball_speed_y, player_shot=True))
        last_shot_time_opponent = current_time

    for ball in balls[:]:
        ball.move()
        if ball.rect.top <= 0 or ball.rect.bottom >= screen_height:
            ball.speed_y *= -1
        if ball.rect.colliderect(plyerr_paddle.rect) or ball.rect.colliderect(opponent_paddle.rect):
            ball.speed_x *= -1
        if ball.rect.left <= 0:
            opponent_score += 1
            balls.remove(ball)
            continue
        if ball.rect.right >= screen_width:
            plyerr_score += 1
            balls.remove(ball)
            continue

    screen.fill(black)
    plyerr_paddle.draw(screen)
    opponent_paddle.draw(screen)
    for ball in balls:
        ball.draw(screen)

    plyerr_text = font.render(str(plyerr_score), True, white)
    screen.blit(plyerr_text, (screen_width // 4, 10))
    opponent_text = font.render(str(opponent_score), True, white)
    screen.blit(opponent_text, (screen_width * 3 // 4, 10))

    pygame.display.flip()
    clock.tick(60)
    if plyerr_score == 10 or opponent_score == 10:
        break

pygame.quit()
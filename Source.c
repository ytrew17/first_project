#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

#define H 10
#define W 30
#define NUM_ITEM 5

char map[H][W] =
{
    { "oooooooooooooooooooooooooooooo" },
    { "o     ooo   oo        oo    oo" },
    { "o           oo              oo" },
    { "o                 oo         o" },
    { "oo       oo       oo         o" },
    { "oo       oo            o   ooo" },
    { "o                      o   ooo" },
    { "o  ooo                       o" },
    { "o  ooo    oo      ooo      ooo" },
    { "oooooooooooooooooooooooooooooo" }
};

struct user {
    int cur_x;
    int cur_y;
    int dx;
    int dy;
    int life;
    int get_point;
    int reset;
    char target;
};

void set_cursor_position(int x, int y);
void hidecursor();
void init_user(struct user* user);
void set_map(struct user* user);
void show_playmap(struct user* user);
void user_input(struct user* user);
void move_user(struct user* user);
void check_for_get_point(struct user* user);
int check_for_end(struct user* user);

int main(void)
{
    struct user* person = (struct user*)malloc(sizeof(struct user));
    int flag = 1;
    init_user(person);
    set_map(person);
    hidecursor();
    while (flag)
    {
        set_cursor_position(0, 0);
        show_playmap(person);
        user_input(person);
        move_user(person);
        check_for_get_point(person);
        flag = check_for_end(person);
    }
    free(person);    
    return 0;
}

void set_cursor_position(int x, int y)
{
    COORD c_pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c_pos);
}

void hidecursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void init_user(struct user* user) 
{
    user->cur_x = 1;
    user->cur_y = 1;
    user->dx = 0;
    user->dy = 0;
    user->life = 5;
    user->get_point = 0;
    user->reset = 3;
    user->target = 'A';
}

void set_map(struct user* user)
{
    for (int i = 0; i < NUM_ITEM; i++)
    {
        int x, y;
        srand(time(NULL));
        do
        {
            x = 1 + rand() % (W - 1);
            y = 1 + rand() % (H - 1);
        } while (map[y][x] != ' ');
        map[y][x] = 'A' + i;
    }
    map[user->cur_y][user->cur_x] = 'U';
}

void show_playmap(struct user* user)
{
    map[user->cur_y][user->cur_x] = 'U';
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 30; j++) printf("%c", map[i][j]);
        printf("\n");
    }
    printf(" Score: %2d, Life: %d, Reset: %d", user->get_point, user->life, user->reset);
}


void user_input(struct user* user)
{

    _kbhit();
    char c1 = _getch();
    if (c1 == -32)
    {
        char c2 = _getch();
        switch (c2)
        {
        case 72: user->dy = -1; break;
        case 80: user->dy = +1; break;
        case 75: user->dx = -1; break;
        case 77: user->dx = +1; break;
        }
    }
    else if ((user->reset > 0)&&(c1 == 'r'))
    {
        user->reset -= 1;
        user->target = 'A' + NUM_ITEM + 1;
        user->dx = user->dy = 0;
    }
    else user->dx = user->dy = 0;
}

void move_user(struct user* user)
{
    if (map[user->cur_y + user->dy][user->cur_x + user->dx] != 'o')
    {
        user->cur_x += user->dx;
        user->cur_y += user->dy;
        map[user->cur_y - user->dy][user->cur_x - user->dx] = ' ';
        user->dy = user->dx = 0;
        if (map[user->cur_y][user->cur_x] == user->target)
        {
            user->get_point += 1;
            user->target += 1;
        }
        else if (map[user->cur_y][user->cur_x] == ' ');
        else if ('A' <= map[user->cur_y][user->cur_x] <= 'A' + NUM_ITEM)
        {
            user->life += -1;
            user->target = 'A' + NUM_ITEM + 1;
        }
    }
    else user->life -= 1, user->dy = user->dx = 0;
}


void check_for_get_point(struct user* user)
{
    if (user->target >= 'A' + NUM_ITEM)
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 30; j++)
                if (map[i][j] != 'o') map[i][j] = ' ';
        }
        if (user->target > 'A' + NUM_ITEM) user->cur_x = 1, user->cur_y = 1;
        user->dx = 0;
        user->dy = 0;
        user->target = 'A';
        set_map(user);
        set_cursor_position(0, 0);
        show_playmap(user);
    }
}

int check_for_end(struct user* user)
{
    if (!(user->life))
    {
        set_cursor_position(0, 0);
        show_playmap(user);
        printf("\n\n\tloser!\n\n");
        return 0;
    }
    else if (user->get_point == 15) 
    {
        set_cursor_position(0, 0);
        show_playmap(user);
        printf("\n\n\twinner!\n\n");
        return 0;
    }
    else return 1;
}
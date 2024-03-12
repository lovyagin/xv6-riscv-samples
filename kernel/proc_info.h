#pragma once

struct procinfo {
    char name[16];
    int state;
    int parent_pid;
};
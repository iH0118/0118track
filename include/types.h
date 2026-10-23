#ifndef _0118TRACK_TYPES_H
#define _0118TRACK_TYPES_H

typedef enum alignment_direction alignment_direction_t;
typedef enum array_direction array_direction_t;

enum alignment_direction {
    TOP_LEFT, TOP_CENTER, TOP_RIGHT,
    CENTER_LEFT, CENTER, CENTER_RIGHT,
    BOTTOM_LEFT, BOTTOM_CENTER, BOTTOM_RIGHT
};

enum array_direction {
    VERTICAL, HORIZONTAL
};


#endif
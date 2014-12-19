#ifndef __MOUSE_STRUCT_H
#define __MOUSE_STRUCT_H

typedef struct
{
	int LB;
	int RB;
	int MB;
	int XOV;
	int YOV;
	int x;
	int y;
	int x_mouse;
	int y_mouse;
} mouse_st;

mouse_st mouse_t;

#endif /* __MOUSE_STRUCT_H */

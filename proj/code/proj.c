/**
 * @brief THE ARTIST VS GUESS GAME
 */

#include "global_variables.h"
#include "lib.h"

#include "bmpfile.h"

#include "rtc_macros.h"
#include "frame.h"
#include "menu_macros.h"
#include "array_keyboard.h"
#include "struct_scores.h"
#include "struct_bmp.h"
#include "serial_port_macros.h"
#include "subscribe_macros.h"
#include "device_interrupts.h"
#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
#include "video_gr.h"
#include "bitmap.h"
#include "rtc.h"
#include "menu.h"
#include "struct_bmp.h"
#include "read_write.h"

extern int RTC_COUNTER;
extern char name[11];
extern char word[11];
extern char guess_ai[11];
extern scores_t top_highscores;

extern int espaco;
extern int length;
extern int length_word;
extern int contador_high;
extern int tentativas;
extern int ai_or_pvp;
extern int contador_c;

extern char day;
extern char year;
extern char month;
extern char hour;
extern char minutes;

int main(int argc, char **argv) {

	// Initialize service
	sef_startup();

	// Enable IO-sensitive operations for ourselves
	sys_enable_iop(SELF);

	// Enables the random number
	srand(time(NULL));

	if (subscribe_all() == -1) {
		printf("Failure to subscribe!! \n\n");
		return -1;
	}


	int pausa = 0;
	// mouse
	unsigned int i = 0;
	unsigned int j = 0;
	char bool1 = 0;
	char bool2 = 0;
	char a[3];
	char byte1;
	char byte2;
	char byte3;
	unsigned int mouse;
	unsigned long key_register;
	mouse_int_handler(SET_STREAM); //define o rato como stream mode
	mouse_int_handler(ESDP); //ativa o envio dos dados por parte do rato

	//timer
	int temp_counter = 0;
	int temp_counter_c = 0;

	//keyboard
	int key = 0;

	//hardware
	int r;
	int ipc_status;
	message msg;

	// graphics mode
	bitmap_struct bitmaps;
	bitmaps_load(&bitmaps); // this operation may require a lot of time


	Bitmap *key_scancode[86];

	fill_key_scancode(key_scancode);

	Bitmap *numbers[10];

	fill_number(numbers);

	// scores E GLOBAL

	//irq_sets
	char irq_set_keyboard = BIT(macro_hook_id_keyboard);
	char irq_set_timer = BIT(macro_hook_id_timer);
	char irq_set_mouse = BIT(macro_hook_id_mouse);
	char irq_set_sp1 = BIT(macro_hook_id_sp1);
	char irq_set_sp2 = BIT(macro_hook_id_sp2);
	char irq_set_rtc = BIT(macro_hook_id_rtc);

	//atributes for the serial port
	unsigned long line_status;
	char send[1];
	int size_send = 1;
	char need_to_write = 0; // 0 = false

	// Initialization of graphics mode
	vg_init(GRAPHIC_MODE_16_BITS);

	// Mouse on the middle of the screen
	mouse_t.x_mouse = 10;
	mouse_t.y_mouse = 10;

	char * mouse_buffer = getMouseBuffer();
	char * screen_buffer = getScreenBuffer();
	char * video_memory = getVideoMem();
	char * human_machine = getHumanMachine();

	// Draw of the background
	drawBitmap(bitmaps.background, INITIAL, INITIAL, ALIGN_LEFT, screen_buffer);

	screen_to_mouse(screen_buffer, mouse_buffer);

	// Draw the mouse
	drawMouse(bitmaps.mouse, mouse_t.x_mouse, mouse_t.y_mouse, ALIGN_LEFT,
			mouse_buffer);

	mouse_to_video(mouse_buffer, video_memory);

	// Draw Human Machine
	drawBitmap(bitmaps.frame, INITIAL, INITIAL, ALIGN_LEFT, human_machine);

	//atributes
	mouse_t.LB = 0; //to prevent the selection of the first menu

	//read highscore
	read_ev();

	//Create 10 white bitmaps
	createBitmapsUndoRedo();

	Bitmap * random = randImage();

	while (1) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_timer) /* subscribed interrupt for timer*/
				{
					if (OPTION == HUMAN_VS_MACHINE)
					{
						if (verdadeiro == 1)
						{
							random = randImage();
							contador_c = CANVAS_Y_F;
							verdadeiro = 0;
							tentativas = 0;
							tries = 0;
						}
						else
						{
							if (contador_c <= CANVAS_Y_I)
							{

							}
							else
							{
								contador_c--;
								drawBitmap(random, CANVAS_X_I,CANVAS_Y_I, ALIGN_LEFT, screen_buffer);
								paintWhiteCanvas(CANVAS_X_I,CANVAS_X_F, CANVAS_Y_I,contador_c);
							}
						}

					}

					screen_to_mouse(screen_buffer, mouse_buffer);
					drawMouse(bitmaps.mouse, mouse_t.x_mouse, mouse_t.y_mouse, ALIGN_LEFT, mouse_buffer);

					mouse_to_video(mouse_buffer, video_memory);


					if ((OPTION == HUMAN_VS_MACHINE) || (OPTION == HEAD_TO_HEAD))
					{
						temp_counter++;

						int score_t = score(contador_high);
						int i = 0;
						int y = 410;
						int a = 0;
						int l = 0;

						human_machine = getHumanMachine();
						screen_buffer = getScreenBuffer();
						drawBitmap(bitmaps.frame, INITIAL, INITIAL, ALIGN_LEFT, human_machine);
						human_machine = human_machine + 274 * 2 + getHRes() * 722 * 2;
						screen_buffer = screen_buffer + 274 * 2 + getHRes() * 722 * 2;
						for(; a < 767 - 722; a++)
						{
							for (; l < 474 - 274;l++)
							{
								*(uint16_t *)screen_buffer = *(uint16_t *)human_machine;
								screen_buffer+=2;
								human_machine+=2;
							}
							l = 0;
							screen_buffer += getHRes() * 2 - 200 * 2;
							human_machine += getHRes() * 2 - 200 * 2;
						}
						human_machine = getHumanMachine();
						screen_buffer = getScreenBuffer();


						while(score_t != 0)
						{
							int temp = score_t % 10;
							score_t = score_t / 10;
							drawMouse(numbers[temp], y, 722, ALIGN_LEFT, screen_buffer);
							y = y - SPACE_KEYS;
						}



						if (temp_counter == 60)
						{

							contador_high++;
							temp_counter = 0;
						}
					}
				}

				if (msg.NOTIFY_ARG & irq_set_keyboard) /* subscribed interrupt for keyboard*/
				{
					kbd_scan_c(&key);

					if (OPTION == GET_NAME) {
						printName(bitmaps, key_scancode, key);
					}
					if (OPTION == HEAD_TO_HEAD)
					{
						printHead(bitmaps, key_scancode, key, numbers);
					}

					if (OPTION == HUMAN_VS_MACHINE)
					{
						printMachine(bitmaps, key_scancode, key, numbers);
					}
					if (key == KEY_ESC) {
						if (OPTION == GET_NAME) {
							name[0] = 'U';
							name[1] = 'P';
							name[2] = 'S';
							name[3] = '\0';
							OPTION = MAIN_MENU;
						}
						OPTION = MAIN_MENU;
						drawBitmap(bitmaps.background, INITIAL, INITIAL, ALIGN_LEFT, screen_buffer);

						espaco = 0;
						tentativas = 0;
						tries = 0;
						RTC_COUNTER = START_RTC_COUNTER;
						verdadeiro = 1;
						cleanName();
					}
				}

				if (msg.NOTIFY_ARG & irq_set_mouse) /* subscribed interrupt for mouse*/
				{
					sys_inb(OUT_BUF, &key_register);
					mouse = (unsigned int) key_register;
					//le o packet
					if (bool1 == 0) {
						if (BIT(3) == (BIT(3) & mouse)) {
							bool1 = 1; //o primeiro byte
							byte1 = mouse;
						} else
							continue;

					} else {
						if (bool2 == 0) {
							bool2 = 1; //o segundo byte
							byte2 = mouse;
						} else {
							//este e o 3 byte
							//estao todos lidos
							//bool3 = 1;
							byte3 = mouse;
							bool1 = 0;
							bool2 = 0;

							//os bytes passam para um array
							a[0] = byte1;
							a[1] = byte2;
							a[2] = byte3;
							fill_struct(a);

							if (0 == menu_handler(bitmaps, numbers, key_scancode)) {
								return 0;
							}

						}
					}
				}

				if (msg.NOTIFY_ARG & irq_set_sp1) /* subscribed interrupt for serial port 1*/
				{
					sys_inb(BASE_ADDRESS_COM1 + INTERRUPT_IDENTIFICATION,
							&line_status);
					if (INTERRUPT_ORIGIN_RECEIVED_DATA & line_status) {
						com1_receive_interrupt(size_send, send);
						/////////////////////////////////////////////////////////
						// need to call the handler (don exist, need to create)//
						/////////////////////////////////////////////////////////
					} else if (INTERRUPT_ORIGIN_TRANSMITTER_EMPTY
							& line_status) {
						if (need_to_write) //need_to_write = 1 = true
						{
							/////////////////////////////////////////////////////////
							// need to call the handler (don exist, need to create)//
							/////////////////////////////////////////////////////////
							need_to_write = 0;
						}
					} else {
						printf("Error on serial port 1!\n");
					}
				}

				if (msg.NOTIFY_ARG & irq_set_sp2) /* subscribed interrupt for serial port 2*/
				{
					sys_inb(BASE_ADDRESS_COM2 + INTERRUPT_IDENTIFICATION,
							&line_status);
					if (INTERRUPT_ORIGIN_RECEIVED_DATA & line_status) {
						com2_receive_interrupt(size_send, send);
						/////////////////////////////////////////////////////////
						// need to call the handler (don exist, need to create)//
						/////////////////////////////////////////////////////////
					} else if (INTERRUPT_ORIGIN_TRANSMITTER_EMPTY
							& line_status) {
						if (need_to_write) //need_to_write = 1 = true
						{
							/////////////////////////////////////////////////////////
							// need to call the handler (don exist, need to create)//
							/////////////////////////////////////////////////////////
							need_to_write = 0;
						}
					} else {
						printf("Error on serial port 1!\n");
					}
				}

				if (msg.NOTIFY_ARG & irq_set_rtc) /* subscribed interrupt for rtc */
				{

					if ((OPTION == HUMAN_VS_MACHINE) || (OPTION == HEAD_TO_HEAD) || (OPTION == ONLINE))
					{
						RTC_COUNTER--;

						year = read_rtc(YEAR);
						month = read_rtc(MONTH);
						day = read_rtc(MONTH_DAY);
						hour = read_rtc(HOURS);
						minutes = read_rtc(MINUTES);

						if (RTC_COUNTER < 10) {
							displayTimer(RTC_COUNTER, numbers, bitmaps);
						} else {
							displayTimer10(RTC_COUNTER, numbers, bitmaps);
						}
					} else {
						screen_abs = 0;
						screen_current = 0;
						RTC_COUNTER = START_RTC_COUNTER;
					}
					int rtc_temp = 0;
					for(; rtc_temp < 10000; rtc_temp++)
					{

					}
					read_rtc(REGISTER_C);
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
		//read_rtc(REGISTER_C);
	}

	//Delete from memory
	deleteBitmap(bitmaps.background);
	deleteBitmap(bitmaps.mouse);
	deleteBitmap(bitmaps.frame);
	deleteBitmap(bitmaps.highscores);
	deleteBitmap(bitmaps.pre_head_to_head);
	deleteBitmap(bitmaps.lost);
	deleteBitmap(bitmaps.win);
	deleteBitmap(random);
	i = 0;
	for(; i < 10; i++)
	{
		deleteBitmap(numbers[i]);
	}
	i = 16;
	for(; i < 50; i++)
	{
		deleteBitmap(key_scancode[i]);
	}

	//Exit the graphic mode
	vg_exit();

	write_all();
	//estas duas opearacoes sao feitas para assegurar o normal funcionamento do rato quando acabar a funcao
	mouse_int_handler(DISABLE_STREAM); //desativa a stream
	mouse_int_handler(SET_STREAM); //volta a ativar a stream, isto foi feito para desativar o envio dos pacotes

	if (unsubscribe_all() == -1) {
		printf("Failure to unsubscribe!! \n\n");
		return -1;
	}

	write_all();

	return 0;
}

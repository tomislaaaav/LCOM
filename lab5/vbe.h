#ifndef __VBE_H
#define __VBE_H

#define VBE_MODE 0x4F02 //4f para escrever na bios, 02 vbe mode
#define VBE_GET_MODE 0x4F01
#define INTERRUPT_VBE 0x10
#define GRAPHIC_MODE 0x105
#define VBE_MODE_SIZE 256

#define BIT(n) 		(0x01<<(n))

#define TIMER0_IRQ 0

#define LINEAR_MODEL_BIT BIT(14)

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

#define STAT_REG	0X64
#define KBC_CMD_REG	0x64
#define OUT_BUF		0x60
#define PAR_ERR		0x80
#define TO_ERR		0x40
#define CBUFFER_EKBD	0xF4
#define LEDS		0xED
#define ACK			0x00FA
#define RESEND		0xFE
#define ERROR		0xFC
#define KBD_ESC_KEY 0x81
#define DELAY_US 	20000
#define KBC_IRQ		0x01

extern int hook_id; //hook_id do timer
extern int khook_id; //hook_id do keyboard

/** @defgroup vbe vbe
 * @{
 *
 * Functions related to the VBE standard
 */

/** @name VBE Mode Info Block */
/**@{
 *
 * Packed VBE Mode Info Block 
 */ 
 
typedef struct {
  /*  Mandatory information for all VBE revisions */
  uint16_t ModeAttributes; 	/**< @brief mode attributes */
  uint8_t WinAAttributes; 		/**< @brief window A attributes */
  uint8_t WinBAttributes; 		/**< @brief window B attributes */
  uint16_t WinGranularity; 	/**< @brief window granularity */
  uint16_t WinSize;		/**< @brief window size */
  uint16_t WinASegment;		/**< @brief window A start segment */
  uint16_t WinBSegment;		/**< @brief window B start segment */
  phys_bytes WinFuncPtr;	/**< @brief real mode/far pointer to window function */
  uint16_t BytesPerScanLine; 	/**< @brief bytes per scan line */

  /* Mandatory information for VBE 1.2 and above */

  uint16_t XResolution;      	/**< @brief horizontal resolution in pixels/characters */
  uint16_t YResolution;      	/**< @brief vertical resolution in pixels/characters */
  uint8_t XCharSize; 		/**< @brief character cell width in pixels */
  uint8_t YCharSize; 		/**< @brief character cell height in pixels */
  uint8_t NumberOfPlanes; 		/**< @brief number of memory planes */
  uint8_t BitsPerPixel; 		/**< @brief bits per pixel */
  uint8_t NumberOfBanks;		/**< @brief number of banks */
  uint8_t MemoryModel;		/**< @brief memory model type */
  uint8_t BankSize;		/**< @brief bank size in KB */
  uint8_t NumberOfImagePages;	/**< @brief number of images */
  uint8_t Reserved1;		/**< @brief reserved for page function */

  /* Direct Color fields (required for direct/6 and YUV/7 memory models) */
  
  uint8_t RedMaskSize;		/* size of direct color red mask in bits */
  uint8_t RedFieldPosition;	/* bit position of lsb of red mask */
  uint8_t GreenMaskSize;		/* size of direct color green mask in bits */
  uint8_t GreenFieldPosition;	/* bit position of lsb of green mask */
  uint8_t BlueMaskSize; 		/* size of direct color blue mask in bits */
  uint8_t BlueFieldPosition;	/* bit position of lsb of blue mask */
  uint8_t RsvdMaskSize;		/* size of direct color reserved mask in bits */
  uint8_t RsvdFieldPosition;	/* bit position of lsb of reserved mask */
  uint8_t DirectColorModeInfo;	/* direct color mode attributes */

  /* Mandatory information for VBE 2.0 and above */
  phys_bytes PhysBasePtr;       /**< @brief physical address for flat memory frame buffer */
  uint8_t Reserved2[4]; 		/**< @brief Reserved - always set to 0 */
  uint8_t Reserved3[2]; 		/**< @brief Reserved - always set to 0 */

  /* Mandatory information for VBE 3.0 and above */
  uint16_t LinBytesPerScanLine;    /* bytes per scan line for linear modes */
  uint8_t BnkNumberOfImagePages; 	/* number of images for banked modes */
  uint8_t LinNumberOfImagePages; 	/* number of images for linear modes */
  uint8_t LinRedMaskSize; 	        /* size of direct color red mask (linear modes) */
  uint8_t LinRedFieldPosition; 	/* bit position of lsb of red mask (linear modes) */
  uint8_t LinGreenMaskSize; 	/* size of direct color green mask (linear modes) */
  uint8_t LinGreenFieldPosition; /* bit position of lsb of green mask (linear  modes) */
  uint8_t LinBlueMaskSize; 	/* size of direct color blue mask (linear modes) */
  uint8_t LinBlueFieldPosition; 	/* bit position of lsb of blue mask (linear modes ) */
  uint8_t LinRsvdMaskSize; 	/* size of direct color reserved mask (linear modes) */
  uint8_t LinRsvdFieldPosition;	 /* bit position of lsb of reserved mask (linear modes) */
  uint32_t MaxPixelClock; 	         /* maximum pixel clock (in Hz) for graphics mode */
  uint8_t Reserved4[190]; 		 /* remainder of ModeInfoBlock */
} __attribute__((packed)) vbe_mode_info_t;

/** @} end of vbe_mode_info_t*/

/**
 * @brief Returns information on the input VBE mode, including screen dimensions, color depth and VRAM physical address
 * 
 * Initializes unpacked vbe_mode__info_t structure passed as an address with
 *  the information of the input mode, by calling VBE function 0x01
 *  Return VBE Mode Information and unpacking the ModeInfoBlock struct
 *  returned by that function.
 * 
 * @param mode mode whose information should be returned
 * @param vmi_p address of vbe_mode_info_t structure to be initialized
 * @return 0 on success, non-zero otherwise
 */
int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p); //NOT COMPLETED


int vbe_set_mode(unsigned short function, unsigned short mode); //sets the mode mode in the function that the user sets

int timer_subscribe_int(void );
int timer_unsubscribe_int();


int kbd_unsubscribe_int(); //Cancela a subscrição das interrupções do keyboard
int kbd_subscribe_int(void ); //Ativa a subscrição das interrupções do keyboard
int kbc_cmd_send(unsigned long cmd); //envia o comando para o input
int kbc_cmd_receive(); //recebe todo o output
int kbd_scan_c(int *apt);


/** @} end of vbe */

#endif /* __VBE_H */

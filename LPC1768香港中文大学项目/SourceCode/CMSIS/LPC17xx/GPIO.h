#ifndef      __GPIO_H__
#define      __GPIO_H__

#define WR  			0		// P0.0
#define RD  			1		// P0.1
#define DATA164			16		// P0.16
#define LOAD165			19	 	// P0.19
#define CLK165			21		// P0.21

#define CS28J60			0		// P1.0
#define CS45			4		// P1.4
#define CSZK			8		// P1.8
#define NCS1            9		// P1.9
#define RSTPD           10		// P1.10
#define CS485			14		// P1.14

#define CLK164			3		// P2.3
#define RSTDSP          10		// P2.10
#define DISP  			11		// P2.11
#define RS  			12		// P2.12
#define CSDSP           13		// P2.13
#define TXD2			8		// P2.8
#define RXD2			9		// P2.9

#define LEDL			25		// P3.25
#define SPK				26		// P3.26 

#define CSKEY			28		// P4.28
/**************************************************************				
*P1口的引脚功能记录
P1.0	~	P1.15 	|	不可用								
P1.16	~	P1.31 	|	总线AD0	~ AD15						IO
****************************************************************/		
#define	AD_PIN_NUM		16		//P1.16~P1.31定义外部16位数据总线D0-D15的引脚起始号

/**************************************************************
*管脚输出
**************************************************************/
#define	SET_WR_PIN			(LPC_GPIO0->FIOSET |= (1<<WR))
#define	CLR_WR_PIN			(LPC_GPIO0->FIOCLR |= (1<<WR))

#define	SET_RD_PIN			(LPC_GPIO0->FIOSET |= (1<<RD))
#define	CLR_RD_PIN			(LPC_GPIO0->FIOCLR |= (1<<RD))

#define SET_DATA164_PIN		(LPC_GPIO0->FIOSET |= (1<<DATA164)) 
#define CLR_DATA164_PIN		(LPC_GPIO0->FIOCLR |= (1<<DATA164))

#define SET_LOAD165_PIN		(LPC_GPIO0->FIOSET |= (1<<LOAD165)) 
#define CLR_LOAD165_PIN		(LPC_GPIO0->FIOCLR |= (1<<LOAD165))

#define SET_CLK165_PIN		(LPC_GPIO0->FIOSET |= (1<<CLK165)) 
#define CLR_CLK165_PIN		(LPC_GPIO0->FIOCLR |= (1<<CLK165))

#define	SET_ENC28J60_PIN	(LPC_GPIO1->FIOSET |= (1<<CS28J60))
#define	CLR_ENC28J60_PIN	(LPC_GPIO1->FIOCLR |= (1<<CS28J60))

#define SET_CS45_PIN		(LPC_GPIO1->FIOSET |= (1<<CS45)) 
#define CLR_CS45_PIN		(LPC_GPIO1->FIOCLR |= (1<<CS45))

#define SET_CSZK_PIN		(LPC_GPIO1->FIOSET |= (1<<CSZK)) 
#define CLR_CSZK_PIN		(LPC_GPIO1->FIOCLR |= (1<<CSZK)) 

#define SET_NCS1_PIN		(LPC_GPIO1->FIOSET |= (1<<NCS1)) 
#define CLR_NCS1_PIN		(LPC_GPIO1->FIOCLR |= (1<<NCS1)) 

#define SET_RSTPD_PIN		(LPC_GPIO1->FIOSET |= (1<<RSTPD)) 
#define CLR_RSTPD_PIN		(LPC_GPIO1->FIOCLR |= (1<<RSTPD)) 

#define	SET_CS485_PIN		(LPC_GPIO1->FIOSET |= (1<<CS485))
#define	CLR_CS485_PIN		(LPC_GPIO1->FIOCLR |= (1<<CS485))

#define SET_CLK164_PIN		(LPC_GPIO2->FIOSET |= (1<<CLK164)) 
#define CLR_CLK164_PIN		(LPC_GPIO2->FIOCLR |= (1<<CLK164))

#define	SET_RST_PIN			(LPC_GPIO2->FIOSET |= (1<<RSTDSP))
#define	CLR_RST_PIN			(LPC_GPIO2->FIOCLR |= (1<<RSTDSP))

#define	SET_DISP_PIN		(LPC_GPIO2->FIOSET |= (1<<DISP))
#define	CLR_DISP_PIN		(LPC_GPIO2->FIOCLR |= (1<<DISP))

#define	SET_RS_PIN			(LPC_GPIO2->FIOSET |= (1<<RS))
#define	CLR_RS_PIN			(LPC_GPIO2->FIOCLR |= (1<<RS))

#define	SET_CS_PIN			(LPC_GPIO2->FIOSET |= (1<<CSDSP))
#define	CLR_CS_PIN			(LPC_GPIO2->FIOCLR |= (1<<CSDSP))

#define	SET_RSTYY_PIN		(LPC_GPIO2->FIOSET |= (1<<TXD2))
#define	CLR_RSTYY_PIN		(LPC_GPIO2->FIOCLR |= (1<<TXD2))
#define	SET_DATAYY_PIN		(LPC_GPIO2->FIOSET |= (1<<RXD2))
#define	CLR_DATAYY_PIN		(LPC_GPIO2->FIOCLR |= (1<<RXD2))

#define	SET_LEDL_PIN		(LPC_GPIO3->FIOSET |= (1<<LEDL))
#define	CLR_LEDL_PIN		(LPC_GPIO3->FIOCLR |= (1<<LEDL))

#define	SET_SPK_PIN			(LPC_GPIO3->FIOSET |= (1<<SPK))
#define	CLR_SPK_PIN			(LPC_GPIO3->FIOCLR |= (1<<SPK))

#define SET_CSKEY_PIN		(LPC_GPIO4->FIOSET |= (1<<CSKEY)) 
#define CLR_CSKEY_PIN		(LPC_GPIO4->FIOCLR |= (1<<CSKEY))
 
#endif
/***************************************************************
*结束
***************************************************************/





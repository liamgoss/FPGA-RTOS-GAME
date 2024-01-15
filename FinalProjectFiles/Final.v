module Final(

	 input CLOCK_50,
	 input [17:0] SW,
	 input [3:0] KEY,
	 output [17:0] LEDR,
	 output [7:0] LEDG,
	 output [8:0] HEX0,
    output [8:0] HEX1,
    output [8:0] HEX2,
    output [8:0] HEX3,
    output [8:0] HEX4,
    output [8:0] HEX5,
    output [8:0] HEX6,
    output [8:0] HEX7,
    output [12:0] DRAM_ADDR,
	 output [1:0] DRAM_BA,
	 output DRAM_CAS_N,DRAM_RAS_N,DRAM_CLK,
	 output DRAM_CKE,DRAM_CS_N,DRAM_WE_N,
	 output [3:0] DRAM_DQM,
	 inout  [31:0] DRAM_DQ,
	 inout [7:0] LCD_DATA,
	 output LCD_EN,
	 output LCD_RS,
	 output LCD_RW, 
	 output LCD_ON,             //            .ON
	 output LCD_BLON        //            .BLON
	
);

	
	GameSystem u0(
		.clk_clk					(CLOCK_50),     //         clk.clk
		.ledg_export         (LEDG),         //        ledg.export
		.leds_export			(LEDR),         //        leds.export
		.reset_reset_n			(KEY[0]),       //      reset.reset_n
		.keys_export		  (KEY[3:1]),     //       push1.export
		.sdram_wire_addr		(DRAM_ADDR),    //    sdram_wire.addr
		.sdram_wire_ba			(DRAM_BA),      //                .ba
		.sdram_wire_cas_n		(DRAM_CAS_N),   //             .cas_n
		.sdram_wire_cke		(DRAM_CKE),     //            	.cke
		.sdram_wire_cs_n		(DRAM_CS_N),    //           	  .cs_n
		.sdram_wire_dq			(DRAM_DQ),      //            	 .dq
		.sdram_wire_dqm		(DRAM_DQM),     //            	.dqm
		.sdram_wire_ras_n		(DRAM_RAS_N),   //             .ras_n
		.sdram_wire_we_n		(DRAM_WE_N),    //              .we_n
		//.sdram_clk_clk(DRAM_CLK),				 //
		.seven_seg_0_export	(HEX0), 			 // seven_seg_0.export
		.seven_seg_1_export	(HEX1),         // seven_seg_1.export
		.seven_seg_2_export	(HEX2),         // seven_seg_2.export
		.seven_seg_3_export	(HEX3),         // seven_seg_3.export
		.seven_seg_4_export	(HEX4),         // seven_seg_4.export
		.seven_seg_5_export	(HEX5),         // seven_seg_5.export
		.seven_seg_6_export	(HEX6),         // seven_seg_6.export
		.seven_seg_7_export	(HEX7),         // seven_seg_7.export
		.switches_export		(SW),            //    switches.export
		.lcd_DATA           (LCD_DATA[7:0]),            	 //         lcd.DATA
		.lcd_EN             (LCD_EN),              //            .EN
		.lcd_RS             (LCD_RS),              //            .RS
		.lcd_RW             (LCD_RW),               //            .RW
		.lcd_ON             (LCD_ON),             //            .ON
		.lcd_BLON           (LCD_BLON)            //            .BLON
	);
	
	assign DRAM_CLK = CLOCK_50;
	
endmodule
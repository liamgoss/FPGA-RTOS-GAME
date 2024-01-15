	GameSystem u0 (
		.clk_clk            (<connected-to-clk_clk>),            //         clk.clk
		.keys_export        (<connected-to-keys_export>),        //        keys.export
		.lcd_DATA           (<connected-to-lcd_DATA>),           //         lcd.DATA
		.lcd_ON             (<connected-to-lcd_ON>),             //            .ON
		.lcd_BLON           (<connected-to-lcd_BLON>),           //            .BLON
		.lcd_EN             (<connected-to-lcd_EN>),             //            .EN
		.lcd_RS             (<connected-to-lcd_RS>),             //            .RS
		.lcd_RW             (<connected-to-lcd_RW>),             //            .RW
		.ledg_export        (<connected-to-ledg_export>),        //        ledg.export
		.leds_export        (<connected-to-leds_export>),        //        leds.export
		.reset_reset_n      (<connected-to-reset_reset_n>),      //       reset.reset_n
		.sdram_wire_addr    (<connected-to-sdram_wire_addr>),    //  sdram_wire.addr
		.sdram_wire_ba      (<connected-to-sdram_wire_ba>),      //            .ba
		.sdram_wire_cas_n   (<connected-to-sdram_wire_cas_n>),   //            .cas_n
		.sdram_wire_cke     (<connected-to-sdram_wire_cke>),     //            .cke
		.sdram_wire_cs_n    (<connected-to-sdram_wire_cs_n>),    //            .cs_n
		.sdram_wire_dq      (<connected-to-sdram_wire_dq>),      //            .dq
		.sdram_wire_dqm     (<connected-to-sdram_wire_dqm>),     //            .dqm
		.sdram_wire_ras_n   (<connected-to-sdram_wire_ras_n>),   //            .ras_n
		.sdram_wire_we_n    (<connected-to-sdram_wire_we_n>),    //            .we_n
		.seven_seg_0_export (<connected-to-seven_seg_0_export>), // seven_seg_0.export
		.seven_seg_1_export (<connected-to-seven_seg_1_export>), // seven_seg_1.export
		.seven_seg_2_export (<connected-to-seven_seg_2_export>), // seven_seg_2.export
		.seven_seg_3_export (<connected-to-seven_seg_3_export>), // seven_seg_3.export
		.seven_seg_4_export (<connected-to-seven_seg_4_export>), // seven_seg_4.export
		.seven_seg_5_export (<connected-to-seven_seg_5_export>), // seven_seg_5.export
		.seven_seg_6_export (<connected-to-seven_seg_6_export>), // seven_seg_6.export
		.seven_seg_7_export (<connected-to-seven_seg_7_export>), // seven_seg_7.export
		.switches_export    (<connected-to-switches_export>),    //    switches.export
		.sd_card_b_SD_cmd   (<connected-to-sd_card_b_SD_cmd>),   //     sd_card.b_SD_cmd
		.sd_card_b_SD_dat   (<connected-to-sd_card_b_SD_dat>),   //            .b_SD_dat
		.sd_card_b_SD_dat3  (<connected-to-sd_card_b_SD_dat3>),  //            .b_SD_dat3
		.sd_card_o_SD_clock (<connected-to-sd_card_o_SD_clock>)  //            .o_SD_clock
	);


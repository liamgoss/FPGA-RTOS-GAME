	component GameSystem is
		port (
			clk_clk            : in    std_logic                     := 'X';             -- clk
			keys_export        : in    std_logic_vector(2 downto 0)  := (others => 'X'); -- export
			lcd_DATA           : inout std_logic_vector(7 downto 0)  := (others => 'X'); -- DATA
			lcd_ON             : out   std_logic;                                        -- ON
			lcd_BLON           : out   std_logic;                                        -- BLON
			lcd_EN             : out   std_logic;                                        -- EN
			lcd_RS             : out   std_logic;                                        -- RS
			lcd_RW             : out   std_logic;                                        -- RW
			ledg_export        : out   std_logic_vector(7 downto 0);                     -- export
			leds_export        : out   std_logic_vector(17 downto 0);                    -- export
			reset_reset_n      : in    std_logic                     := 'X';             -- reset_n
			sdram_wire_addr    : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_wire_ba      : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_wire_cas_n   : out   std_logic;                                        -- cas_n
			sdram_wire_cke     : out   std_logic;                                        -- cke
			sdram_wire_cs_n    : out   std_logic;                                        -- cs_n
			sdram_wire_dq      : inout std_logic_vector(31 downto 0) := (others => 'X'); -- dq
			sdram_wire_dqm     : out   std_logic_vector(3 downto 0);                     -- dqm
			sdram_wire_ras_n   : out   std_logic;                                        -- ras_n
			sdram_wire_we_n    : out   std_logic;                                        -- we_n
			seven_seg_0_export : out   std_logic_vector(6 downto 0);                     -- export
			seven_seg_1_export : out   std_logic_vector(6 downto 0);                     -- export
			seven_seg_2_export : out   std_logic_vector(6 downto 0);                     -- export
			seven_seg_3_export : out   std_logic_vector(6 downto 0);                     -- export
			seven_seg_4_export : out   std_logic_vector(6 downto 0);                     -- export
			seven_seg_5_export : out   std_logic_vector(6 downto 0);                     -- export
			seven_seg_6_export : out   std_logic_vector(6 downto 0);                     -- export
			seven_seg_7_export : out   std_logic_vector(6 downto 0);                     -- export
			switches_export    : in    std_logic_vector(17 downto 0) := (others => 'X')  -- export
		);
	end component GameSystem;

	u0 : component GameSystem
		port map (
			clk_clk            => CONNECTED_TO_clk_clk,            --         clk.clk
			keys_export        => CONNECTED_TO_keys_export,        --        keys.export
			lcd_DATA           => CONNECTED_TO_lcd_DATA,           --         lcd.DATA
			lcd_ON             => CONNECTED_TO_lcd_ON,             --            .ON
			lcd_BLON           => CONNECTED_TO_lcd_BLON,           --            .BLON
			lcd_EN             => CONNECTED_TO_lcd_EN,             --            .EN
			lcd_RS             => CONNECTED_TO_lcd_RS,             --            .RS
			lcd_RW             => CONNECTED_TO_lcd_RW,             --            .RW
			ledg_export        => CONNECTED_TO_ledg_export,        --        ledg.export
			leds_export        => CONNECTED_TO_leds_export,        --        leds.export
			reset_reset_n      => CONNECTED_TO_reset_reset_n,      --       reset.reset_n
			sdram_wire_addr    => CONNECTED_TO_sdram_wire_addr,    --  sdram_wire.addr
			sdram_wire_ba      => CONNECTED_TO_sdram_wire_ba,      --            .ba
			sdram_wire_cas_n   => CONNECTED_TO_sdram_wire_cas_n,   --            .cas_n
			sdram_wire_cke     => CONNECTED_TO_sdram_wire_cke,     --            .cke
			sdram_wire_cs_n    => CONNECTED_TO_sdram_wire_cs_n,    --            .cs_n
			sdram_wire_dq      => CONNECTED_TO_sdram_wire_dq,      --            .dq
			sdram_wire_dqm     => CONNECTED_TO_sdram_wire_dqm,     --            .dqm
			sdram_wire_ras_n   => CONNECTED_TO_sdram_wire_ras_n,   --            .ras_n
			sdram_wire_we_n    => CONNECTED_TO_sdram_wire_we_n,    --            .we_n
			seven_seg_0_export => CONNECTED_TO_seven_seg_0_export, -- seven_seg_0.export
			seven_seg_1_export => CONNECTED_TO_seven_seg_1_export, -- seven_seg_1.export
			seven_seg_2_export => CONNECTED_TO_seven_seg_2_export, -- seven_seg_2.export
			seven_seg_3_export => CONNECTED_TO_seven_seg_3_export, -- seven_seg_3.export
			seven_seg_4_export => CONNECTED_TO_seven_seg_4_export, -- seven_seg_4.export
			seven_seg_5_export => CONNECTED_TO_seven_seg_5_export, -- seven_seg_5.export
			seven_seg_6_export => CONNECTED_TO_seven_seg_6_export, -- seven_seg_6.export
			seven_seg_7_export => CONNECTED_TO_seven_seg_7_export, -- seven_seg_7.export
			switches_export    => CONNECTED_TO_switches_export     --    switches.export
		);


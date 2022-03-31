module oof_hw(

      ///////// ADC /////////
      output             ADC_CONVST,
      output             ADC_DIN,
      input              ADC_DOUT,
      output             ADC_SCLK,

      ///////// AUD /////////
      input              AUD_ADCDAT,
      inout              AUD_ADCLRCK,
      inout              AUD_BCLK,
      output             AUD_DACDAT,
      inout              AUD_DACLRCK,
      output             AUD_XCK,

      ///////// CLOCK2 /////////
      input              CLOCK2_50,

      ///////// CLOCK3 /////////
      input              CLOCK3_50,

      ///////// CLOCK4 /////////
      input              CLOCK4_50,

      ///////// CLOCK /////////
      input              CLOCK_50,

      ///////// DRAM /////////
      output      [12:0] DRAM_ADDR,
      output      [1:0]  DRAM_BA,
      output             DRAM_CAS_N,
      output             DRAM_CKE,
      output             DRAM_CLK,
      output             DRAM_CS_N,
      inout       [15:0] DRAM_DQ,
      output             DRAM_LDQM,
      output             DRAM_RAS_N,
      output             DRAM_UDQM,
      output             DRAM_WE_N,

      ///////// FAN /////////
      output             FAN_CTRL,

      ///////// FPGA /////////
      output             FPGA_I2C_SCLK,
      inout              FPGA_I2C_SDAT,

      ///////// GPIO /////////
      inout     [35:0]         GPIO_0,
      inout     [35:0]         GPIO_1,
 

      ///////// HEX0 /////////
      output      [6:0]  HEX0,
 
      ///////// HEX1 /////////
      output      [6:0]  HEX1,

      ///////// HEX2 /////////
      output      [6:0]  HEX2,

      ///////// HEX3 /////////
      output      [6:0]  HEX3,

      ///////// HEX4 /////////
      output      [6:0]  HEX4,

      ///////// HEX5 /////////
      output      [6:0]  HEX5,

      ///////// HPS /////////
      inout              HPS_CONV_USB_N,
      output      [14:0] HPS_DDR3_ADDR,
      output      [2:0]  HPS_DDR3_BA,
      output             HPS_DDR3_CAS_N,
      output             HPS_DDR3_CKE,
      output             HPS_DDR3_CK_N,
      output             HPS_DDR3_CK_P,
      output             HPS_DDR3_CS_N,
      output      [3:0]  HPS_DDR3_DM,
      inout       [31:0] HPS_DDR3_DQ,
      inout       [3:0]  HPS_DDR3_DQS_N,
      inout       [3:0]  HPS_DDR3_DQS_P,
      output             HPS_DDR3_ODT,
      output             HPS_DDR3_RAS_N,
      output             HPS_DDR3_RESET_N,
      input              HPS_DDR3_RZQ,
      output             HPS_DDR3_WE_N,
      output             HPS_ENET_GTX_CLK,
      inout              HPS_ENET_INT_N,
      output             HPS_ENET_MDC,
      inout              HPS_ENET_MDIO,
      input              HPS_ENET_RX_CLK,
      input       [3:0]  HPS_ENET_RX_DATA,
      input              HPS_ENET_RX_DV,
      output      [3:0]  HPS_ENET_TX_DATA,
      output             HPS_ENET_TX_EN,
      inout       [3:0]  HPS_FLASH_DATA,
      output             HPS_FLASH_DCLK,
      output             HPS_FLASH_NCSO,
      inout              HPS_GSENSOR_INT,
      inout              HPS_I2C1_SCLK,
      inout              HPS_I2C1_SDAT,
      inout              HPS_I2C2_SCLK,
      inout              HPS_I2C2_SDAT,
      inout              HPS_I2C_CONTROL,
      inout              HPS_KEY,
      inout              HPS_LED,
      inout              HPS_LTC_GPIO,
      output             HPS_SD_CLK,
      inout              HPS_SD_CMD,
      inout       [3:0]  HPS_SD_DATA,
      output             HPS_SPIM_CLK,
      input              HPS_SPIM_MISO,
      output             HPS_SPIM_MOSI,
      inout              HPS_SPIM_SS,
      input              HPS_UART_RX,
      output             HPS_UART_TX,
      input              HPS_USB_CLKOUT,
      inout       [7:0]  HPS_USB_DATA,
      input              HPS_USB_DIR,
      input              HPS_USB_NXT,
      output             HPS_USB_STP,

      ///////// IRDA /////////
      input              IRDA_RXD,
      output             IRDA_TXD,

      ///////// KEY /////////
      input       [3:0]  KEY,

      ///////// LEDR /////////
      output      [9:0]  LEDR,

      ///////// PS2 /////////
      inout              PS2_CLK,
      inout              PS2_CLK2,
      inout              PS2_DAT,
      inout              PS2_DAT2,

      ///////// SW /////////
      input       [9:0]  SW,

      ///////// TD /////////
      input              TD_CLK27,
      input      [7:0]  TD_DATA,
      input             TD_HS,
      output             TD_RESET_N,
      input             TD_VS,

      ///////// VGA /////////
      output      [7:0]  VGA_B,
      output             VGA_BLANK_N,
      output             VGA_CLK,
      output      [7:0]  VGA_G,
      output             VGA_HS,
      output      [7:0]  VGA_R,
      output             VGA_SYNC_N,
      output             VGA_VS
);


oof_gpu_datapath u0 (
		.clk_clk                           (CLOCK_50),                           //                    clk.clk
		.hps_ddr_mem_a                     (HPS_DDR3_ADDR),                     //                hps_ddr.mem_a
		.hps_ddr_mem_ba                    (HPS_DDR3_BA),                    //                       .mem_ba
		.hps_ddr_mem_ck                    (HPS_DDR3_CK_P),                    //                       .mem_ck
		.hps_ddr_mem_ck_n                  (HPS_DDR3_CK_N),                  //                       .mem_ck_n
		.hps_ddr_mem_cke                   (HPS_DDR3_CKE),                   //                       .mem_cke
		.hps_ddr_mem_cs_n                  (HPS_DDR3_CS_N),                  //                       .mem_cs_n
		.hps_ddr_mem_ras_n                 (HPS_DDR3_RAS_N),                 //                       .mem_ras_n
		.hps_ddr_mem_cas_n                 (HPS_DDR3_CAS_N),                 //                       .mem_cas_n
		.hps_ddr_mem_we_n                  (HPS_DDR3_WE_N),                  //                       .mem_we_n
		.hps_ddr_mem_reset_n               (HPS_DDR3_RESET_N),               //                       .mem_reset_n
		.hps_ddr_mem_dq                    (HPS_DDR3_DQ),                    //                       .mem_dq
		.hps_ddr_mem_dqs                   (HPS_DDR3_DQS_P),                   //                       .mem_dqs
		.hps_ddr_mem_dqs_n                 (HPS_DDR3_DQS_N),                 //                       .mem_dqs_n
		.hps_ddr_mem_odt                   (HPS_DDR3_ODT),                   //                       .mem_odt
		.hps_ddr_mem_dm                    (HPS_DDR3_DM),                    //                       .mem_dm
		.hps_ddr_oct_rzqin                 (HPS_DDR3_RZQ),                 //                       .oct_rzqin
		.hps_io_hps_io_emac1_inst_TX_CLK   (HPS_ENET_GTX_CLK),   //                 hps_io.hps_io_emac1_inst_TX_CLK
		.hps_io_hps_io_emac1_inst_TXD0     (HPS_ENET_TX_DATA[0]),     //                       .hps_io_emac1_inst_TXD0
		.hps_io_hps_io_emac1_inst_TXD1     (HPS_ENET_TX_DATA[1]),     //                       .hps_io_emac1_inst_TXD1
		.hps_io_hps_io_emac1_inst_TXD2     (HPS_ENET_TX_DATA[2]),     //                       .hps_io_emac1_inst_TXD2
		.hps_io_hps_io_emac1_inst_TXD3     (HPS_ENET_TX_DATA[3]),     //                       .hps_io_emac1_inst_TXD3
		.hps_io_hps_io_emac1_inst_RXD0     (HPS_ENET_RX_DATA[0]),     //                       .hps_io_emac1_inst_RXD0
		.hps_io_hps_io_emac1_inst_MDIO     (HPS_ENET_MDIO),     //                       .hps_io_emac1_inst_MDIO
		.hps_io_hps_io_emac1_inst_MDC      (HPS_ENET_MDC),      //                       .hps_io_emac1_inst_MDC
		.hps_io_hps_io_emac1_inst_RX_CTL   (HPS_ENET_RX_DV),   //                       .hps_io_emac1_inst_RX_CTL
		.hps_io_hps_io_emac1_inst_TX_CTL   (HPS_ENET_TX_EN),   //                       .hps_io_emac1_inst_TX_CTL
		.hps_io_hps_io_emac1_inst_RX_CLK   (HPS_ENET_RX_CLK),   //                       .hps_io_emac1_inst_RX_CLK
		.hps_io_hps_io_emac1_inst_RXD1     (HPS_ENET_RX_DATA[1]),     //                       .hps_io_emac1_inst_RXD1
		.hps_io_hps_io_emac1_inst_RXD2     (HPS_ENET_RX_DATA[2]),     //                       .hps_io_emac1_inst_RXD2
		.hps_io_hps_io_emac1_inst_RXD3     (HPS_ENET_RX_DATA[3]),     //                       .hps_io_emac1_inst_RXD3
		.hps_io_hps_io_qspi_inst_IO0       (HPS_FLASH_DATA[0]),       //                       .hps_io_qspi_inst_IO0
		.hps_io_hps_io_qspi_inst_IO1       (HPS_FLASH_DATA[1]),       //                       .hps_io_qspi_inst_IO1
		.hps_io_hps_io_qspi_inst_IO2       (HPS_FLASH_DATA[2]),       //                       .hps_io_qspi_inst_IO2
		.hps_io_hps_io_qspi_inst_IO3       (HPS_FLASH_DATA[3]),       //                       .hps_io_qspi_inst_IO3
		.hps_io_hps_io_qspi_inst_SS0       (HPS_FLASH_NCSO),       //                       .hps_io_qspi_inst_SS0
		.hps_io_hps_io_qspi_inst_CLK       (HPS_FLASH_DCLK),       //                       .hps_io_qspi_inst_CLK
		.hps_io_hps_io_sdio_inst_CMD       (HPS_SD_CMD),       //                       .hps_io_sdio_inst_CMD
		.hps_io_hps_io_sdio_inst_D0        (HPS_SD_DATA[0]),        //                       .hps_io_sdio_inst_D0
		.hps_io_hps_io_sdio_inst_D1        (HPS_SD_DATA[1]),        //                       .hps_io_sdio_inst_D1
		.hps_io_hps_io_sdio_inst_CLK       (HPS_SD_CLK),       //                       .hps_io_sdio_inst_CLK
		.hps_io_hps_io_sdio_inst_D2        (HPS_SD_DATA[2]),        //                       .hps_io_sdio_inst_D2
		.hps_io_hps_io_sdio_inst_D3        (HPS_SD_DATA[3]),        //                       .hps_io_sdio_inst_D3
		.hps_io_hps_io_usb1_inst_D0        (HPS_USB_DATA[0]),        //                       .hps_io_usb1_inst_D0
		.hps_io_hps_io_usb1_inst_D1        (HPS_USB_DATA[1]),        //                       .hps_io_usb1_inst_D1
		.hps_io_hps_io_usb1_inst_D2        (HPS_USB_DATA[2]),        //                       .hps_io_usb1_inst_D2
		.hps_io_hps_io_usb1_inst_D3        (HPS_USB_DATA[3]),        //                       .hps_io_usb1_inst_D3
		.hps_io_hps_io_usb1_inst_D4        (HPS_USB_DATA[4]),        //                       .hps_io_usb1_inst_D4
		.hps_io_hps_io_usb1_inst_D5        (HPS_USB_DATA[5]),        //                       .hps_io_usb1_inst_D5
		.hps_io_hps_io_usb1_inst_D6        (HPS_USB_DATA[6]),        //                       .hps_io_usb1_inst_D6
		.hps_io_hps_io_usb1_inst_D7        (HPS_USB_DATA[7]),        //                       .hps_io_usb1_inst_D7
		.hps_io_hps_io_usb1_inst_CLK       (HPS_USB_CLKOUT),       //                       .hps_io_usb1_inst_CLK
		.hps_io_hps_io_usb1_inst_STP       (HPS_USB_STP),       //                       .hps_io_usb1_inst_STP
		.hps_io_hps_io_usb1_inst_DIR       (HPS_USB_DIR),       //                       .hps_io_usb1_inst_DIR
		.hps_io_hps_io_usb1_inst_NXT       (HPS_USB_NXT),       //                       .hps_io_usb1_inst_NXT
		.hps_io_hps_io_spim0_inst_CLK      (HPS_SPIM_CLK),      //                       .hps_io_spim0_inst_CLK
		.hps_io_hps_io_spim0_inst_MOSI     (HPS_SPIM_MOSI),     //                       .hps_io_spim0_inst_MOSI
		.hps_io_hps_io_spim0_inst_MISO     (HPS_SPIM_MISO),     //                       .hps_io_spim0_inst_MISO
		.hps_io_hps_io_spim0_inst_SS0      (HPS_SPIM_SS),      //                       .hps_io_spim0_inst_SS0
		.hps_io_hps_io_uart0_inst_RX       (HPS_UART_RX),       //                       .hps_io_uart0_inst_RX
		.hps_io_hps_io_uart0_inst_TX       (HPS_UART_TX),       //                       .hps_io_uart0_inst_TX
		.hps_io_hps_io_i2c0_inst_SDA       (HPS_I2C1_SDAT),       //                       .hps_io_i2c0_inst_SDA
		.hps_io_hps_io_i2c0_inst_SCL       (HPS_I2C1_SCLK),       //                       .hps_io_i2c0_inst_SCL
		.hps_io_hps_io_i2c1_inst_SDA       (HPS_I2C2_SDAT),       //                       .hps_io_i2c1_inst_SDA
		.hps_io_hps_io_i2c1_inst_SCL       (HPS_I2C2_SCLK),       //                       .hps_io_i2c1_inst_SCL
		.hps_io_hps_io_gpio_inst_GPIO09    (HPS_CONV_USB_N),    //                       .hps_io_gpio_inst_GPIO09
		.hps_io_hps_io_gpio_inst_GPIO35    (HPS_ENET_INT_N),    //                       .hps_io_gpio_inst_GPIO35
		.hps_io_hps_io_gpio_inst_GPIO40    (HPS_LTC_GPIO),    //                       .hps_io_gpio_inst_GPIO40
		.hps_io_hps_io_gpio_inst_GPIO48    (HPS_I2C_CONTROL),    //                       .hps_io_gpio_inst_GPIO48
		.hps_io_hps_io_gpio_inst_GPIO53    (HPS_LED),    //                       .hps_io_gpio_inst_GPIO53
		.hps_io_hps_io_gpio_inst_GPIO61    (HPS_GSENSOR_INT),    //                       .hps_io_gpio_inst_GPIO61
		.reset_reset_n                     (1'b1),                     //                  reset.reset_n
		.cond_test_new_signal_8            (LEDR[7:0]),          //   cond_test.new_signal_8
		.vga_b_new_signal_5                (VGA_B),              //       vga_b.new_signal_5
		.vga_g_new_signal_4                (VGA_G),              //       vga_g.new_signal_4
		.vga_r_new_signal_2                (VGA_R),              //       vga_r.new_signal_2
		.vga_hs_new_signal_1               (VGA_HS),             //      vga_hs.new_signal_1
		.vga_clk_new_signal                (VGA_CLK),              //     vga_clk.new_signal
		.vga_sync_n_new_signal_7           (VGA_SYNC_N),         //  vga_sync_n.new_signal_7
		.vga_blank_n_new_signal_6          (VGA_BLANK_N),        // vga_blank_n.new_signal_6
		.vga_vs_new_signal_3               (VGA_VS)              //      vga_vs.new_signal_3
	);

	assign LEDR[9:8] = 2'b11;

endmodule
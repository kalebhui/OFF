module test_keyboard(
	//input
	input                       CLOCK_50,	//clk_50m
	input[3:0]                  KEY,	    //asynchronous active low reset
	//ps2
	input                       PS2_CLK,
	input                       PS2_DAT,
	//vga output
	output                      VGA_CLK,	//pixel_clk   
	output                      VGA_HS, //vga horizontal synchronization         
	output                      VGA_VS, //vga vertical synchronization                  
	output[7:0]                 VGA_R,  //vga red
	output[7:0]                 VGA_G,  //vga green
	output[7:0]                 VGA_B,   //vga blue

	//output[31:0]				vga_row,     //y pixel coordinate; 0 = top row
	//output[31:0]				vga_col,     //x pixel coordinate; 0 = leftmost

	output                      VGA_BLANK_N, //direct blanking
	output                      VGA_SYNC_N  //sync-on-green
	
);

reg                             video_clk;
wire                            video_h_sync;
wire                            video_v_sync;
wire                            video_de;
wire[7:0]                       video_r;
wire[7:0]                       video_g;
wire[7:0]                       video_b;

assign VGA_HS = video_h_sync;
assign VGA_VS = video_v_sync;
assign VGA_R  = video_r[7:0]; 
assign VGA_G  = video_g[7:0]; 
assign VGA_B  = video_b[7:0]; 

assign VGA_BLANK_N = 1'b1; // 1 for no use
assign VGA_CLK = video_clk;
assign VGA_SYNC_N = 1'b0; // 0 for no use

//generate video pixel clock
pll pll_test(
		.refclk(CLOCK_50),   //  refclk.clk
		.rst(~KEY[0]),      //   reset.reset
		.outclk_0(video_clk)  // outclk0.clk
	);

// reg video_clk=1'b0;
// always @(posedge clk)begin
// 	video_clk = ~video_clk;
// end

wire[11:0] h_counter;
wire[11:0] v_counter;

vga_sync video_sync_test1(
	.clk(video_clk),
	.rst(0),
	.h_sync(video_h_sync),
	.v_sync(video_v_sync),
	.de(video_de),
	.h_counter(h_counter),
	.v_counter(v_counter)
);


wire kbd_data_ready;

wire [7:0] kbd_received_ascii_code;

Kbd_scan Kbd_scan_test1(
	.clk(CLOCK_50),
	.rst_n(1'b1),
	.kbd_clk(PS2_CLK),
	.kbd_data(PS2_DAT),
	.kbd_byte(kbd_received_ascii_code),
	.kbd_state(kbd_data_ready)
);


vga_display video_display_test1(
	.clk(video_clk),
	.rst(~KEY[2]),
	.h_counter(h_counter),
	.v_counter(v_counter),
	.video_active(video_de),
	.kbd_signal(kbd_received_ascii_code),
	.kbd_ready(kbd_data_ready),
	.rgb_r(video_r),
	.rgb_g(video_g),
	.rgb_b(video_b)
);

endmodule


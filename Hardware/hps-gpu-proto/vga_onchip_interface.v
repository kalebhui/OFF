// new_component.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module vga_onchip_interface (
		input  wire        clock_sink_clk,         //    clock_sink.clk
		input  wire        reset_sink_reset,       //    reset_sink.reset
		output wire [17:0] avalon_master_address,  // avalon_master.address
		output wire        avalon_master_read,     //              .read
		input  wire [31:0] avalon_master_readdata, //              .readdata
		input  wire 	   avalon_master_waitrequest,

		output wire [7:0]  conduit_test,            //   conduit_end.new_signal
		
		///////   VGA conduit  //////
		output             VGA_CLK,
		
		output             VGA_HS,
		output             VGA_VS,
		output      [7:0]  VGA_R,
		output      [7:0]  VGA_G,
		output      [7:0]  VGA_B,

		output             VGA_BLANK_N,
		output             VGA_SYNC_N
		///////   VGA conduit  //////
	);

	// TODO: Auto-generated HDL template

	assign avalon_master_address = video_address;

	assign avalon_master_read = 1'b1;

	assign conduit_test = avalon_master_readdata[7:0] | 8'b01000000;


	///////////// VGA timing Generator  ////////////

	reg                             video_clk = 1'b0;
	wire                            video_h_sync;
	wire                            video_v_sync;
	wire                            video_de;
	wire[7:0]                       video_r;
	wire[7:0]                       video_g;
	wire[7:0]                       video_b;
	wire[11:0]						x_pos;
	wire[11:0]						y_pos;

	assign VGA_HS = video_h_sync;
	assign VGA_VS = video_v_sync;
	assign VGA_R  = video_de? video_r[7:0] : 8'h00; 
	assign VGA_G  = video_de? video_g[7:0] : 8'h00; 
	assign VGA_B  = video_de? video_b[7:0] : 8'h00; 

	assign VGA_BLANK_N = 1'b1; // 1 for no use
	assign VGA_CLK = video_clk;
	assign VGA_SYNC_N = 1'b0; // 0 for no use

	//clock div for 25Mhz
	always @(posedge clock_sink_clk)begin
	video_clk = ~video_clk;
	end

	vga_timing_module m_0(
	.clk(video_clk),
	.rst(0),
	.h_sync(video_h_sync),
	.v_sync(video_v_sync),
	.de(video_de),
	.x_pos_out(x_pos),     //x position
    .y_pos_out(y_pos)      //Y position
	);

	///////////// VGA color Generator  ////////////
	wire[17:0] 					video_address;
	wire[7:0]					pixel_data;
	assign video_address = {y_pos[9:1] , x_pos[9:1]};	//resulotion in frame buffer is half of display resolution
														//divide position by 2
	assign pixel_data = avalon_master_readdata[7:0];	//8bit color data for pixel

	assign video_r = { { 3 {pixel_data[7]} } , { 3 {pixel_data[6]} } , { 2{pixel_data[5]} } };
	assign video_g = { { 3 {pixel_data[4]} } , { 3 {pixel_data[3]} } , { 2{pixel_data[2]} } };
	assign video_b = { { 4 {pixel_data[1]} } , { 4 {pixel_data[0]} } };

endmodule

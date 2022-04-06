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
		input  wire [7:0] avalon_master_readdata, //              .readdata
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
		output             VGA_SYNC_N,

		input 		[7:0]  avalon_slave_addr,
		input              avalon_slave_write,
		input       [31:0] avalon_slave_writedata
		///////   VGA conduit  //////
	);

	////////////// Character movement //////////////
	wire [8:0] x_1, x_2;
	wire [7:0] y_1, y_2;

	always @(posedge clock_sink_clk) begin
		if(avalon_slave_addr == 8'h1 && avalon_slave_write) p2 <= avalon_slave_writedata;
		if(avalon_slave_addr == 8'h0 && avalon_slave_write) p1 <= avalon_slave_writedata;
	end
	reg [31:0] p1; //0  x first 9 bits y fllowing 8 bits     
	reg [31:0] p2; //4  

	assign x_1 = p1[8:0];
	assign y_1 = p1[16:9];
	assign x_2 = p2[8:0];
	assign y_2 = p2[16:9];

	//Parameters for character movement acceleration
	parameter CHAR_WIDTH = 9'd10;	//Character width and height, in actual resolution, not frame buffer resolution
	parameter CHAR_HEIGHT = 9'd10;
	parameter CHAR2_R        = 8'h00;  //cyan
	parameter CHAR2_G        = 8'hff;  
	parameter CHAR2_B        = 8'hff; 
	parameter CHAR1_R      	 = 8'ha0;  //purple
	parameter CHAR1_G        = 8'h20;
	parameter CHAR1_B        = 8'hf0;
	parameter EYE_R			 = 8'hff;  //eye color, white
	parameter EYE_G			 = 8'hff;
	parameter EYE_B			 = 8'hff;


	/////////////////  logic for rendering character   //////////////////
	always@(posedge VGA_CLK or posedge reset_sink_reset) begin
	if(reset_sink_reset == 1'b1)
		begin
			video_r <= 8'h00;
			video_g <= 8'h00;
			video_b <= 8'h00;
		end
	else if(x_pos >= x_1 * 2
		   && x_pos < x_1 * 2 + CHAR_WIDTH 
		   && y_pos >= y_1 * 2
		   && y_pos < y_1 * 2 + CHAR_HEIGHT)begin
			   if (char_bitmap[y_1_relative][x_1_relative]) begin
				    video_r <= EYE_R;
					video_g <= EYE_G;
					video_b <= EYE_B;
			   end else begin
				   	video_r <= CHAR1_R;
					video_g <= CHAR1_G;
					video_b <= CHAR1_B;
			   end
				
		end
	else if(x_pos >= x_2 * 2
		   && x_pos < x_2 * 2 + CHAR_WIDTH 
		   && y_pos >= y_2 * 2
		   && y_pos < y_2 * 2 + CHAR_HEIGHT)begin
			   if (char_bitmap[y_2_relative][x_2_relative]) begin
				    video_r <= EYE_R;
					video_g <= EYE_G;
					video_b <= EYE_B;
			   end else begin
				   	video_r <= CHAR2_R;
					video_g <= CHAR2_G;
					video_b <= CHAR2_B;
			   end
				
		end	
	else 
		begin
			video_r <= {  {pixel_data[7:5]}  ,  {pixel_data[7:5]}  ,  {pixel_data[7:6]}  };
			video_g <= {  {pixel_data[4:2]}  ,  {pixel_data[4:2]}  ,  {pixel_data[4:3]}  };
			video_b <= {  {pixel_data[1:0]}  ,  {pixel_data[1:0]}  ,  {pixel_data[1:0]}  ,  {pixel_data[1:0]}  };
		end
end

	////////////// Character movement //////////////

	assign avalon_master_address = video_address;

	assign conduit_test = avalon_master_readdata[7:0] | 8'b01000000;

	//Char bitmap:

	wire [8:0] x_1_relative, y_1_relative, x_2_relative, y_2_relative;
	assign x_1_relative = (x_pos[9:1] - x_1); //relative location to each block for bitmap
	assign y_1_relative = (y_pos[9:1] - y_1);
	assign x_2_relative = (x_pos[9:1] - x_2); 
	assign y_2_relative = (y_pos[9:1] - y_2);

	wire[9:0] char_bitmap [9:0]; //10 * 10 bitmap for char
	assign char_bitmap[0] = 10'b0000000000;
	assign char_bitmap[1] = 10'b0010001000;
	assign char_bitmap[2] = 10'b0010001000;
	assign char_bitmap[3] = 10'b0010001000;
	assign char_bitmap[4] = 10'b0010001000;
	assign char_bitmap[5] = 10'b0000000000;
	assign char_bitmap[6] = 10'b0000000000;
	assign char_bitmap[7] = 10'b0001111100;
	assign char_bitmap[8] = 10'b0000000000;
	assign char_bitmap[9] = 10'b0000000000;


	///////////// Edge Detector for read ///////////
	reg[17:0] address_delay;
	always @(posedge clock_sink_clk) begin
		address_delay <= avalon_master_address;
	end

	assign avalon_master_read = (address_delay != avalon_master_address);


	///////////// VGA timing Generator  ////////////

	reg                             video_clk = 1'b0;
	wire                            video_h_sync;
	wire                            video_v_sync;
	wire                            video_de;
	reg[7:0]                       video_r;
	reg[7:0]                       video_g;
	reg[7:0]                       video_b;
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

	assign pixel_data = avalon_master_readdata;

endmodule

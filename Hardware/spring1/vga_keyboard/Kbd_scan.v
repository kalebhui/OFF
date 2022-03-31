`timescale 1ns / 1ps

module Kbd_scan(clk,rst_n,kbd_clk,kbd_data,kbd_byte,kbd_state);

input clk;		
input rst_n;	
input kbd_clk;	
input kbd_data;		
output[7:0] kbd_byte;	//output corresponding ascii code
output kbd_state;		 

//------------------------------------------
reg kbd_clk_r0,kbd_clk_r1,kbd_clk_r2;	//kbd_clk state registers

//wire pos_kbd_clk; 	// kbd_clk posedge
wire neg_kbd_clk;	// kbd_clk negedge

always @ (posedge clk or negedge rst_n) begin
	if(!rst_n) begin
			kbd_clk_r0 <= 1'b0;
			kbd_clk_r1 <= 1'b0;
			kbd_clk_r2 <= 1'b0;
		end
	else begin								
			kbd_clk_r0 <= kbd_clk;
			kbd_clk_r1 <= kbd_clk_r0;
			kbd_clk_r2 <= kbd_clk_r1;
		end
end

assign neg_kbd_clk = ~kbd_clk_r1 & kbd_clk_r2;	//negedge

//------------------------------------------
reg[7:0] kbd_byte_r;		//one byte data save register 
reg[7:0] temp_data;			
reg[3:0] num;				//count 8 bits

always @ (posedge clk or negedge rst_n) begin
	if(!rst_n) begin
			num <= 4'd0;
			temp_data <= 8'd0;
		end
	else if(neg_kbd_clk) begin	
			case (num)
				4'd0:	num <= num+1'b1;
				4'd1:	begin
							num <= num+1'b1;
							temp_data[0] <= kbd_data;	//bit0
						end
				4'd2:	begin
							num <= num+1'b1;
							temp_data[1] <= kbd_data;	//bit1
						end
				4'd3:	begin
							num <= num+1'b1;
							temp_data[2] <= kbd_data;	//bit2
						end
				4'd4:	begin
							num <= num+1'b1;
							temp_data[3] <= kbd_data;	//bit3
						end
				4'd5:	begin
							num <= num+1'b1;
							temp_data[4] <= kbd_data;	//bit4
						end
				4'd6:	begin
							num <= num+1'b1;
							temp_data[5] <= kbd_data;	//bit5
						end
				4'd7:	begin
							num <= num+1'b1;
							temp_data[6] <= kbd_data;	//bit6
						end
				4'd8:	begin
							num <= num+1'b1;
							temp_data[7] <= kbd_data;	//bit7
						end
				4'd9:	begin
							num <= num+1'b1;
						end
				4'd10: begin
							num <= 4'd0;	//reset
						end
				default: ;
				endcase
		end	
end

reg key_f0;		//release statement 
reg kbd_state_r;	//kbd current state, kbd_state_r=1 represent key is pressed 

always @ (posedge clk or negedge rst_n) begin
	if(!rst_n) begin
			key_f0 <= 1'b0;
			kbd_state_r <= 1'b0;
		end
	else if(num==4'd10) begin	//after trans one byte data
			if(temp_data == 8'hf0) key_f0 <= 1'b1;
			else begin
					if(!key_f0) begin	//key is pressed 
							kbd_state_r <= 1'b1;
							kbd_byte_r <= temp_data;	//save current key value
						end
					else begin
							kbd_state_r <= 1'b0;
							key_f0 <= 1'b0;
						end
				end
		end
end

reg[7:0] ascii_code;	

always @ (kbd_byte_r) begin
	case (kbd_byte_r)		
       8'h45: ascii_code <= 8'h30;   // 0
       8'h16: ascii_code <= 8'h31;   // 1
       8'h1e: ascii_code <= 8'h32;   // 2
       8'h26: ascii_code <= 8'h33;   // 3
       8'h25: ascii_code <= 8'h34;   // 4
       8'h2e: ascii_code <= 8'h35;   // 5
       8'h36: ascii_code <= 8'h36;   // 6
       8'h3d: ascii_code <= 8'h37;   // 7
       8'h3e: ascii_code <= 8'h38;   // 8
       8'h46: ascii_code <= 8'h39;   // 9

       8'h1c: ascii_code <= 8'h41;   // A
       8'h32: ascii_code <= 8'h42;   // B
       8'h21: ascii_code <= 8'h43;   // C
       8'h23: ascii_code <= 8'h44;   // D
       8'h24: ascii_code <= 8'h45;   // E
       8'h2b: ascii_code <= 8'h46;   // F
       8'h34: ascii_code <= 8'h47;   // G
       8'h33: ascii_code <= 8'h48;   // H
       8'h43: ascii_code <= 8'h49;   // I
       8'h3b: ascii_code <= 8'h4a;   // J
       8'h42: ascii_code <= 8'h4b;   // K
       8'h4b: ascii_code <= 8'h4c;   // L
       8'h3a: ascii_code <= 8'h4d;   // M
       8'h31: ascii_code <= 8'h4e;   // N
       8'h44: ascii_code <= 8'h4f;   // O
       8'h4d: ascii_code <= 8'h50;   // P
       8'h15: ascii_code <= 8'h51;   // Q
       8'h2d: ascii_code <= 8'h52;   // R
       8'h1b: ascii_code <= 8'h53;   // S
       8'h2c: ascii_code <= 8'h54;   // T
       8'h3c: ascii_code <= 8'h55;   // U
       8'h2a: ascii_code <= 8'h56;   // V
       8'h1d: ascii_code <= 8'h57;   // W
       8'h22: ascii_code <= 8'h58;   // X
       8'h35: ascii_code <= 8'h59;   // Y
       8'h1a: ascii_code <= 8'h5a;   // Z

       8'h0e: ascii_code <= 8'h60;   // `
       8'h4e: ascii_code <= 8'h2d;   // -
       8'h55: ascii_code <= 8'h3d;   // <=
       8'h54: ascii_code <= 8'h5b;   // [
       8'h5b: ascii_code <= 8'h5d;   // ]
       8'h5d: ascii_code <= 8'h5c;   // \
       8'h4c: ascii_code <= 8'h3b;   // ;
       8'h52: ascii_code <= 8'h27;   // '
       8'h41: ascii_code <= 8'h2c;   // ,
       8'h49: ascii_code <= 8'h2e;   // .
       8'h4a: ascii_code <= 8'h2f;   // /

       8'h29: ascii_code <= 8'h20;   // (space)
       8'h5a: ascii_code <= 8'h0d;   // (enter, cr)
       8'h66: ascii_code <= 8'h08;   // (backspace)
       default: ascii_code <= 8'h2a; // *
		endcase
end

assign kbd_byte = ascii_code;	 
assign kbd_state = kbd_state_r;

endmodule

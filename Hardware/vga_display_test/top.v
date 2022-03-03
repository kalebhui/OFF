module top(
	//input
	input                       CLOCK_50,	//clk_50m
	input[3:0]                  KEY,	    //asynchronous active low reset
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

color_bar color_bar_m0(
	.clk(video_clk),
	.rst(0),
	.h_sync(video_h_sync),
	.v_sync(video_v_sync),
	.de(video_de),
	.rgb_r(video_r),
	.rgb_g(video_g),
	.rgb_b(video_b)
);
endmodule


module color_bar(
	input                 clk,           //pixel clock
	input                 rst,           //reset signal high active
	output                h_sync,            //horizontal synchronization
	output                v_sync,            //vertical synchronization
	output                de,            //video valid
	output reg [7:0]           rgb_r,         //video red data
	output reg [7:0]           rgb_g,         //video green data
	output reg [7:0]           rgb_b          //video blue data
);

//define vga mode related resolution parameters

// 640x480  25.175Mhz
// parameter H_ACTIVE = 16'd640;  //horizontal active video
// parameter H_FP = 16'd16;       //horizontal front porch
// parameter H_SYNCP = 16'd96;    //horizontal sync pulse
// parameter H_BP = 16'd48;       //horizontal back porch
// parameter V_ACTIVE = 16'd480;  //vertical active video
// parameter V_FP  = 16'd10;      //vertical front porch
// parameter V_SYNCP  = 16'd2;    //vertical sync pulse
// parameter V_BP  = 16'd33;      //vertical back porch
// parameter HS_POL = 1'b0; 	   //horizontal polarity
// parameter VS_POL = 1'b0;	   //vertical polarity

// 1024*768 65mhz
parameter H_ACTIVE = 16'd1024;
parameter H_FP = 16'd24;      
parameter H_SYNCP = 16'd136;   
parameter H_BP = 16'd160;     
parameter V_ACTIVE = 16'd768; 
parameter V_FP  = 16'd3;      
parameter V_SYNCP  = 16'd6;    
parameter V_BP  = 16'd29;     
parameter HS_POL = 1'b0;
parameter VS_POL = 1'b0;

//800*600 50mhz 72hz refresh rate ->fail
// parameter H_ACTIVE = 16'd800;
// parameter H_FP = 16'd56;      
// parameter H_SYNCP = 16'd120;   
// parameter H_BP = 16'd64;     
// parameter V_ACTIVE = 16'd600; 
// parameter V_FP  = 16'd37;      
// parameter V_SYNCP  = 16'd6;    
// parameter V_BP  = 16'd23;     
// parameter HS_POL = 1'b1;
// parameter VS_POL = 1'b1;

parameter H_TOTAL = H_ACTIVE + H_FP + H_SYNCP + H_BP; //horizontal  (pixels) whole line 
parameter V_TOTAL = V_ACTIVE + V_FP + V_SYNCP + V_BP; //vertical (lines) whole frame

//define the RGB values for 8 colors
parameter WHITE_R       = 8'hff;
parameter WHITE_G       = 8'hff;
parameter WHITE_B       = 8'hff;
parameter RED_R         = 8'hff;
parameter RED_G         = 8'h00;
parameter RED_B         = 8'h00;
parameter ORANGE_R		= 8'hff;
parameter ORANGE_G		= 8'h61;
parameter ORANGE_B		= 8'h00;
parameter YELLOW_R      = 8'hff;
parameter YELLOW_G      = 8'hff;
parameter YELLOW_B      = 8'h00;                                                               
parameter GREEN_R       = 8'h00;
parameter GREEN_G       = 8'hff;
parameter GREEN_B       = 8'h00;
parameter CYAN_R        = 8'h00;
parameter CYAN_G        = 8'hff;
parameter CYAN_B        = 8'hff; 
parameter BLUE_R        = 8'h00;
parameter BLUE_G        = 8'h00;
parameter BLUE_B        = 8'hff;
parameter PURPLE_R      = 8'ha0;
parameter PURPLE_G      = 8'h20;
parameter PURPLE_B      = 8'hf0;
parameter BLACK_R       = 8'h00;
parameter BLACK_G       = 8'h00;
parameter BLACK_B       = 8'h00;


reg hs_reg;                      //horizontal sync register
reg vs_reg;                      //vertical sync register
reg[11:0] h_counter;             //horizontal counter
reg[11:0] v_counter;             //vertical counter
reg[11:0] x_pos;              	 //video x position 
reg[11:0] y_pos;              	 //video y position 
reg h_active;                    //horizontal video active
reg v_active;                    //vertical video active
wire video_active;               //video active(horizontal active and vertical active)
assign h_sync = hs_reg;
assign v_sync = vs_reg;
assign video_active = h_active & v_active;
assign de = video_active;

//Hsync clk generator
always@(posedge clk or posedge rst)
begin
	if(rst == 1'b1)
		h_counter <= 0;
	else if(h_counter == H_TOTAL - 1)
		h_counter <= 0;
	else
		h_counter <= h_counter + 1; //h counter ++ after one pixel done
end

//Vsync clk generator
always@(posedge clk or posedge rst)
begin
	if(rst == 1'b1)
		v_counter <= 0;
	else if(v_counter == V_TOTAL - 1)
		v_counter <= 0;
	else if(h_counter == H_FP - 1) //v counter ++ only after one row done
		v_counter <= v_counter + 1;
	else
		v_counter <= v_counter;
end

//assume h counter start: fp, syncp, bp, active

//horizontal sync zone
always@(posedge clk or posedge rst)
begin
	if(rst == 1'b1)
		hs_reg <= 1'b0;
	else if(h_counter == H_FP - 1)//horizontal sync start
		hs_reg <= HS_POL;
	else if(h_counter == H_FP + H_SYNCP - 1)//horizontal sync end
		hs_reg <= ~hs_reg;
	else
		hs_reg <= hs_reg;
end

//horizontal active zone
always@(posedge clk or posedge rst)
begin
	if(rst == 1'b1)
		h_active <= 1'b0;
	else if(h_counter == H_FP + H_SYNCP + H_BP - 1)//horizontal active start
		h_active <= 1'b1;
	else if(h_counter == H_TOTAL - 1)//horizontal active end
		h_active <= 1'b0;
	else
		h_active <= h_active;
end

//obtain x coordinate
always@(posedge clk or posedge rst)
begin
	if(rst == 1'b1)
		x_pos <= 0;
	else if(h_counter >= H_FP + H_SYNCP + H_BP - 1)//horizontal video active
		x_pos <= h_counter - (H_FP[11:0] + H_SYNCP[11:0] + H_BP[11:0] - 1); //current total counts - blanking counts
	else
		x_pos <= x_pos;
end

//vertical sync zone
always@(posedge clk or posedge rst)
begin
	if(rst == 1'b1)
		vs_reg <= 1'd0;
	else if((h_counter == H_FP - 1)) begin
		if(v_counter == H_FP - 1)
			vs_reg <= VS_POL;
		else if(v_counter == V_FP + V_SYNCP - 1)
			vs_reg <= ~vs_reg;
	end
	else
		vs_reg <= vs_reg;
end

//vertical active zone
always@(posedge clk or posedge rst)
begin
	if(rst == 1'b1)
		v_active <= 1'd0;
	else if(h_counter == H_FP - 1)begin
		if(v_counter == V_FP + V_SYNCP + V_BP - 1)
			v_active <= 1'b1;
		else if(v_counter == V_TOTAL - 1)
			v_active <= 1'b0;
	end 
	else
		v_active <= v_active;
end

//color and pattern settings 

// first try --> rainbow bars
// always@(posedge clk or posedge rst) begin
// 	if(rst == 1'b1)
// 		begin
// 			rgb_r <= 8'h00;
// 			rgb_g <= 8'h00;
// 			rgb_b <= 8'h00;
// 		end
// 	else if(video_active)
// 		if(x_pos == 12'd0)
// 			begin
// 				rgb_r <= WHITE_R;
// 				rgb_g <= WHITE_G;
// 				rgb_b <= WHITE_B;
// 			end
// 		else if(x_pos == (H_ACTIVE/9) * 1)
// 			begin
// 				rgb_r <= RED_R;
// 				rgb_g <= RED_G;
// 				rgb_b <= RED_B;
// 			end   
// 		else if(x_pos == (H_ACTIVE/9) * 2)
// 			begin
// 				rgb_r <= ORANGE_R;
// 				rgb_g <= ORANGE_G;
// 				rgb_b <= ORANGE_B;
// 			end         
// 		else if(x_pos == (H_ACTIVE/9) * 3)
// 			begin
// 				rgb_r <= YELLOW_R;
// 				rgb_g <= YELLOW_G;
// 				rgb_b <= YELLOW_B;
// 			end
// 		else if(x_pos == (H_ACTIVE/9) * 4)
// 			begin
// 				rgb_r <= GREEN_R;
// 				rgb_g <= GREEN_G;
// 				rgb_b <= GREEN_B;
// 			end
// 		else if(x_pos == (H_ACTIVE/9) * 5)
// 			begin
// 				rgb_r <= CYAN_R;
// 				rgb_g <= CYAN_G;
// 				rgb_b <= CYAN_B;
// 			end
// 		else if(x_pos == (H_ACTIVE/9) * 6)
// 			begin
// 				rgb_r <= BLUE_R;
// 				rgb_g <= BLUE_G;
// 				rgb_b <= BLUE_B;
// 			end
// 		else if(x_pos == (H_ACTIVE/9) * 7)
// 			begin				
// 				rgb_r <= PURPLE_R;
// 				rgb_g <= PURPLE_G;
// 				rgb_b <= PURPLE_B;
// 			end 
// 		else if(x_pos == (H_ACTIVE/9) * 8)
// 			begin
// 				rgb_r <= BLACK_R;
// 				rgb_g <= BLACK_G;
// 				rgb_b <= BLACK_B;
// 			end
// 		else
// 			begin
// 				rgb_r <= rgb_r;
// 				rgb_g <= rgb_g;
// 				rgb_b <= rgb_b;
// 			end         
// 	else
// 		begin
// 			rgb_r <= 8'h00;
// 			rgb_g <= 8'h00;
// 			rgb_b <= 8'h00;
// 		end
// end

//sencond try --> square
always@(posedge clk or posedge rst) begin
	if(rst == 1'b1)
		begin
			rgb_r <= 8'h00;
			rgb_g <= 8'h00;
			rgb_b <= 8'h00;
		end
	else if(video_active)
		if(x_pos>=12'd270 && x_pos<12'd370 && v_counter>=12'd190 && v_counter<12'd290)begin
				rgb_r <= PURPLE_R;
				rgb_g <= PURPLE_G;
				rgb_b <= PURPLE_B;
		end
	else
		begin
			rgb_r <= 8'h00;
			rgb_g <= 8'h00;
			rgb_b <= 8'h00;
		end
end

endmodule 
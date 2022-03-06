module vga_display(
	input                 clk,                //pixel clock
	input                 rst,                //reset signal high active
    input[11:0]                h_counter,         //x position
    input[11:0]                v_counter,         //y position
    input                      video_active,  
	output reg [7:0]           rgb_r,         //video red data
	output reg [7:0]           rgb_g,         //video green data
	output reg [7:0]           rgb_b          //video blue data
);
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
parameter PCLK = 24'd650000;

//square parameter and position define
parameter WIDTH = 16'd128;
parameter HEIGHT = 16'd96;

reg[11:0] left_top_x;
reg[11:0] left_top_y;

reg h_dir; //horizontal direction  0 from left to right 
reg v_dir; //vertical direction	   0 from top to bottom

reg[23:0] move_counter;
always@(posedge clk or posedge rst)begin
	if(rst)
		move_counter <= 0;
	else begin
		if(move_counter < PCLK - 1'd1)
			move_counter = move_counter + 1'b1;
		else 
			move_counter <= 0;
	end
end

wire move_active;
assign move_active = (move_counter == (PCLK - 1'd1))?1'b1:1'b0;

//change direction after touching boundries
always@(posedge clk or posedge rst)begin
	if(rst)begin
		h_dir <= 1'b0;
		v_dir <= 1'b0;
	end
	else begin
		if(left_top_x <= 1'b0)
			h_dir <= 1'b0;
		else if(left_top_x >= H_ACTIVE - WIDTH - 10 )
			h_dir <= 1'b1;
		else
			h_dir <= h_dir;
		if(left_top_y <= 1'b0)
			v_dir <= 1'b0;
		else if(left_top_y >= V_ACTIVE - HEIGHT)
			v_dir <= 1'b1;
		else 
			v_dir <= v_dir;
	end
end

//change left_top position
always@(posedge clk or posedge rst)begin
	if(rst)begin
		left_top_x <= 12'd0;
		left_top_y <= 12'd0;
	end
	else if(move_active) begin
		if(!h_dir) //0 right to left
			left_top_x <= left_top_x + 1'b1;
		else
			left_top_x <= left_top_x -1'b1;
		if(!v_dir) //0 top to bottom
			left_top_y <= left_top_y + 1'b1;
		else
			left_top_y <= left_top_y - 1'b1;
	end
	else begin
		left_top_y <= left_top_y;
		left_top_y <= left_top_y;
	end
end

//displaying 
always@(posedge clk or posedge rst) begin
	if(rst == 1'b1)
		begin
			rgb_r <= 8'h00;
			rgb_g <= 8'h00;
			rgb_b <= 8'h00; 
		end
	else if(h_counter>=(left_top_x + H_FP + H_SYNCP + H_BP - 10) && h_counter<(left_top_x + WIDTH + H_FP + H_SYNCP + H_BP - 10)
			&& v_counter>=left_top_y + V_FP + V_SYNCP + V_BP -1  && v_counter<left_top_y + HEIGHT + V_FP + V_SYNCP + V_BP -1)begin
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



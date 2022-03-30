module vga_display(
	input                 clk,                //pixel clock
	input                 rst,                //reset signal high active
    input[11:0]                h_counter,         //x position
    input[11:0]                v_counter,         //y position
    input                      video_active,  
	input[7:0]				   kbd_signal,
	input					   kbd_ready,
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

//define some key values
parameter KBD_W	= 8'h57;
parameter KBD_A	= 8'h41;
parameter KBD_S	= 8'h53;
parameter KBD_D	= 8'h44;

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


reg kbd_ready_press;
reg kbd_ready_release;

//change left_top position
always @(posedge clk or posedge rst) begin
	if(rst == 1) begin
		left_top_x <= 0;
		left_top_y <= 0;
		kbd_ready_press <= 0;
		kbd_ready_release <= 0;
	end else begin
		kbd_ready_press <= kbd_ready;
		kbd_ready_release <= kbd_ready_press;
		if(kbd_signal == KBD_A && !kbd_ready_press && kbd_ready_release)begin
			if(left_top_x <= WIDTH/2)begin
				left_top_x <= 0;
			end else begin
				left_top_x <= left_top_x - WIDTH/2;
			end
		end else if(kbd_signal == KBD_D && !kbd_ready_press && kbd_ready_release)begin
			if(left_top_x >= H_ACTIVE - WIDTH)begin
				left_top_x <= H_ACTIVE - WIDTH;
			end else begin
				left_top_x <= left_top_x + WIDTH/2;
			end
		end else begin
			left_top_x <= left_top_x;
		end

		if(kbd_signal == KBD_W && !kbd_ready_press && kbd_ready_release)begin
			if(left_top_y <= HEIGHT/2)begin
				left_top_y <= 0;
			end else begin
				left_top_y <= left_top_y - HEIGHT/2;
			end
		end else if(kbd_signal == KBD_S && !kbd_ready_press && kbd_ready_release)begin
			if(left_top_y >= V_ACTIVE - HEIGHT)begin
				left_top_y <= V_ACTIVE - HEIGHT;
			end else begin
				left_top_y <= left_top_y + HEIGHT/2;
			end
		end else begin
			left_top_y <= left_top_y;
		end

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



// generate sync signals & display
module vga_sync(
	input                 clk,           //pixel clock
	input                 rst,           //reset signal high active
	output                h_sync,        //horizontal synchronization
	output                v_sync,        //vertical synchronization
	output                de,            //video valid
    output reg [11:0]              h_counter,         //h_counter
    output reg [11:0]              v_counter          //v_counter
	//output reg [7:0]           rgb_r,         //video red data
	//output reg [7:0]           rgb_g,         //video green data
	//output reg [7:0]           rgb_b          //video blue data
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

reg hs_reg;                      //horizontal sync register
reg vs_reg;                      //vertical sync register
//reg[11:0] h_counter;             //horizontal counter
//reg[11:0] v_counter;             //vertical counter
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
	else if(h_counter == H_FP - 1)begin //v counter ++ only after one row done
		if(v_counter == V_TOTAL - 1)begin 
			v_counter <= 0;
		end
		else begin
		v_counter <= v_counter + 1;
		end 
	end
	else begin
		v_counter <= v_counter;
	end
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
		hs_reg <= ~ hs_reg;
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
// always@(posedge clk or posedge rst)
// begin
// 	if(rst == 1'b1)
// 		x_pos <= 0;
// 	else if(h_counter >= H_FP + H_SYNCP + H_BP - 1)//horizontal video active
// 		x_pos <= h_counter - (H_FP[11:0] + H_SYNCP[11:0] + H_BP[11:0] - 1); //current total counts - blanking counts
// 	else
// 		x_pos <= x_pos;
// end

//vertical sync zone
always@(posedge clk or posedge rst)
begin
	if(rst == 1'b1)
		vs_reg <= 1'd0;
	else if((h_counter == H_FP - 1)) begin
		if(v_counter == V_FP - 1)
			vs_reg <= VS_POL;
		else if(v_counter == V_FP + V_SYNCP - 1)
			vs_reg <= ~ vs_reg;
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

//obtain y coordinate
// always@(posedge clk or posedge rst)
// begin
// 	if(rst == 1'b1)
// 		y_pos <= 0;
// 	else if(h_counter == H_FP - 1)begin
// 		if(v_counter >= V_FP + V_SYNCP + V_BP - 1)
// 			y_pos <= v_counter - (V_FP[11:0] + V_SYNCP[11:0] + V_BP[11:0] - 1);
// 	end 
// 	else
// 		y_pos <= y_pos;
// end

endmodule

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module render_accelerator_module (
		input         clock_sink_clk,         //    clock_sink.clk
		input         reset_sink_reset,       //    reset_sink.reset

        ///////    On-chip mem interface    //////
		output    [17:0]   avalon_master_mem_address,  // address to on-chip-memory
		output             avalon_master_mem_write,    //              .write
		output    [7:0]    avalon_master_mem_writedata,    //              .write data
        input              avalon_master_waitrequest,

        ///////    HPS control interface    //////
        input 		[7:0]  avalon_slave_hps_addr,       //address from hps
		input              avalon_slave_hps_write,      //write command
		input       [31:0] avalon_slave_hps_writedata,  //writedata from hps
        input              avalon_slave_hps_read,       //read command
        output reg      [31:0] avalon_slave_hps_readdata    //readdata sent to hps
	);

    //////////  Control Register ///////////
    reg         ready_flag = 1'b1;  //bit [0] of 0x0
    reg         start_flag = 1'b0;  //bit [0] of 0x1
    reg [3:0]   mode_reg;           //bit [4:1] of 0x1
    reg [31:0]  control_reg_1;      //address 0x2
    reg [31:0]  control_reg_2;      //address 0x3

    /////////    HPS slave interface    //////////
    always @(posedge clock_sink_clk) begin
        if(avalon_slave_hps_write) begin
            case (avalon_slave_hps_addr)
                8'h01   :    begin
                    mode_reg <= avalon_slave_hps_writedata[4:1];
                    start_flag <= avalon_slave_hps_writedata[0];
                end
                8'h02   :    control_reg_1 <= avalon_slave_hps_writedata;
                8'h03   :    control_reg_2 <= avalon_slave_hps_writedata; 
                default :    ;//NOP
            endcase
        end

        else if (avalon_slave_hps_read) begin
            case (avalon_slave_hps_addr)
                8'h00   :   avalon_slave_hps_readdata <= {31'b0, ready_flag};
                8'h01   :   avalon_slave_hps_readdata <= {27'b0, mode_reg, start_flag};
                8'h02   :   avalon_slave_hps_readdata <= control_reg_1;
                8'h03   :   avalon_slave_hps_readdata <= control_reg_2;
                default :   avalon_slave_hps_readdata <= 32'b0;  //invalid addr
            endcase
        end
    end


    ////// TODO : do the fsm
    reg [4:0] state = 5'b00000;
    //state bit 4 = write to memory
    assign avalon_master_mem_write      = state[4];
    assign avalon_master_mem_writedata  = color[7:0];

    reg [8:0] x_counter  = 9'b0;
    reg [7:0] y_counter  = 9'b0;
    wire [8:0] x_coord   = control_reg_1[8:0];
    wire [7:0] y_coord   = control_reg_1[16:9];
    wire [7:0] color     = control_reg_1[24:17];
    wire [8:0] width    = control_reg_2[8:0];
    wire [7:0] height   = control_reg_2[16:9];

    wire [8:0] x_pos     = x_coord + x_counter;
    wire [7:0] y_pos     = y_coord + y_counter;   

    parameter idle      = 5'b00000;
    parameter inc_x     = 5'b10001;
    parameter inc_y     = 5'b10010;
    parameter finish    = 5'b00011;
    parameter square    = 4'b0000;

    always @(posedge clock_sink_clk) begin
        if(reset_sink_reset) begin
            state <= idle;
        end else begin
            case (state)
                //Idle state, ready to start new operation
                idle    : begin
                    //Mode: Drawing Square, set up registers
                    if(start_flag && (mode_reg == 4'b0000))begin
                        state       <= inc_x;
                        ready_flag  <= 1'b0;
                        x_counter   <= 9'b0;
                        y_counter   <= 9'b0; 
                    end
                    else begin
                        ready_flag  <= 1'b1;
                        state       <= idle;
                    end
                end

                //Mode: Drawing Square, incrementing x counter
                inc_x   : begin
                    //reach end of line, switch to next line
                    if (x_counter == (width - 1) && y_counter == (height - 1)) begin
                        state       <= finish;
                    end
                    //reach end of block, go to finish
                    else if (x_counter == (width - 1)) begin
                        state       <= inc_y;
                        x_counter   <= 9'b0;
                    end
                    //continue drawing current line
                    else begin
                        state       <= inc_x;
                        x_counter   <= x_counter + 1;
                    end
                end
                //Mode: Drawing Square, incrementing y counter
                inc_y   : begin
                    state   <=  inc_x;
                    y_counter <= y_counter + 1;
                end
                //Finished
                finish  : state <= idle;
                default : state <= idle;
            endcase
        end
        
    end

    assign avalon_master_mem_address = {1'b0, y_pos[7:0], x_pos[8:0]};
    // always @(*) begin
    //     case (mode_reg)
    //         square  : begin
    //             avalon_master_mem_address = {1'b0, y_pos[7:0], x_pos[8:0]};
    //         end
    //         default: avalon_master_mem_address = 18'b0;
    //     endcase
        
    // end


endmodule
